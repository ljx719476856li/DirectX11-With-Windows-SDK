#include "SkyRender.h"
#include "Geometry.h"
#include "d3dUtil.h"
using namespace DirectX;
using namespace Microsoft::WRL;

SkyRender::SkyRender(
	ComPtr<ID3D11Device> device, 
	ComPtr<ID3D11DeviceContext> deviceContext, 
	const std::wstring & cubemapFilename, 
	float skySphereRadius,
	bool generateMips)
{
	// 天空盒纹理加载
	if (cubemapFilename.substr(cubemapFilename.size() - 3) == L"dds")
	{
		HR(CreateDDSTextureFromFile(
			device.Get(),
			cubemapFilename.c_str(),
			nullptr,
			mTextureCubeSRV.GetAddressOf()
		));
	}
	else
	{
		mTextureCubeSRV = CreateWICTextureCubeFromFile(
			device,
			deviceContext,
			cubemapFilename,
			generateMips
		);
	}

	InitResource(device, skySphereRadius);
}

SkyRender::SkyRender(ComPtr<ID3D11Device> device, 
	ComPtr<ID3D11DeviceContext> deviceContext, 
	const std::vector<std::wstring>& cubemapFilenames, 
	float skySphereRadius,
	bool generateMips)
{
	// 天空盒纹理加载

	mTextureCubeSRV = CreateWICTextureCubeFromFile(
		device,
		deviceContext,
		cubemapFilenames,
		generateMips
	);

	InitResource(device, skySphereRadius);
}

ComPtr<ID3D11ShaderResourceView> SkyRender::GetTextureCube()
{
	return mTextureCubeSRV;
}

void SkyRender::Draw(ComPtr<ID3D11DeviceContext> deviceContext, SkyEffect & skyEffect, const Camera & camera)
{
	UINT strides[1] = { sizeof(XMFLOAT3) };
	UINT offsets[1] = { 0 };
	deviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), strides, offsets);
	deviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	XMFLOAT3 pos = camera.GetPosition();
	skyEffect.SetWorldViewProjMatrix(XMMatrixTranslation(pos.x, pos.y, pos.z) * camera.GetViewProjXM());
	skyEffect.SetTextureCube(mTextureCubeSRV);
	skyEffect.Apply(deviceContext);
	deviceContext->DrawIndexed(mIndexCount, 0, 0);
}

void SkyRender::InitResource(ComPtr<ID3D11Device> device, float skySphereRadius)
{
	Geometry::MeshData sphere = Geometry::CreateSphere(skySphereRadius);
	size_t size = sphere.vertexVec.size();
	std::vector<XMFLOAT3> vertices(size);
	for (size_t i = 0; i < size; ++i)
	{
		vertices[i] = sphere.vertexVec[i].pos;
	}

	// 顶点缓冲区创建
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(XMFLOAT3) * (UINT)vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices.data();

	HR(device->CreateBuffer(&vbd, &InitData, &mVertexBuffer));

	// 索引缓冲区创建
	mIndexCount = (UINT)sphere.indexVec.size();

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(WORD) * mIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.StructureByteStride = 0;
	ibd.MiscFlags = 0;

	InitData.pSysMem = sphere.indexVec.data();

	HR(device->CreateBuffer(&ibd, &InitData, &mIndexBuffer));

}
