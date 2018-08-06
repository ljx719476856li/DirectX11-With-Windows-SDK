#include "LightHelper.hlsli"

Texture2D tex : register(t0);
SamplerState samLinear : register(s0);


cbuffer CBChangesEveryDrawing : register(b0)
{
	row_major matrix gWorld;
	row_major matrix gWorldInvTranspose;
	row_major matrix gTexTransform;
	Material gMaterial;
}

cbuffer CBChangesEveryFrame : register(b1)
{
	row_major matrix gView;
	float3 gEyePosW;
}

cbuffer CBChangesOnResize : register(b2)
{
	row_major matrix gProj;
}

cbuffer CBNeverChange : register(b3)
{
	DirectionalLight gDirLight[10];
	PointLight gPointLight[10];
	SpotLight gSpotLight[10];
	int gNumDirLight;
	int gNumPointLight;
	int gNumSpotLight;
}


struct VertexIn
{
	float3 Pos : POSITION;
    float3 Normal : NORMAL;
	float2 Tex : TEXCOORD;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;
    float3 PosW : POSITION;     // �������е�λ��
    float3 NormalW : NORMAL;    // �������������еķ���
	float2 Tex : TEXCOORD;
};









