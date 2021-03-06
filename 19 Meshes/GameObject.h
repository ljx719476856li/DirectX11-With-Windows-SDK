#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Model.h"


class GameObject
{
public:
	// 使用模板别名(C++11)简化类型名
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	GameObject();

	// 获取位置
	DirectX::XMFLOAT3 GetPosition() const;
	
	//
	// 获取包围盒
	//

	DirectX::BoundingBox GetLocalBoundingBox() const;
	DirectX::BoundingBox GetBoundingBox() const;
	//
	// 设置模型
	//
	
	void SetModel(Model&& model);
	void SetModel(const Model& model);

	//
	// 设置矩阵
	//

	void SetWorldMatrix(const DirectX::XMFLOAT4X4& world);
	void SetWorldMatrix(DirectX::FXMMATRIX world);


	// 绘制对象
	void Draw(ComPtr<ID3D11DeviceContext> deviceContext, BasicEffect& effect);

private:
	Model mModel;												// 模型
	DirectX::XMFLOAT4X4 mWorldMatrix;							// 世界矩阵

};


#endif


