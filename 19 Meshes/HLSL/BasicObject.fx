#include "LightHelper.hlsli"

Texture2D texA : register(t0);
Texture2D texD : register(t1);
SamplerState sam : register(s0);


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
    DirectionalLight gDirLight;
    PointLight gPointLight;
}



struct VertexPosNormalTex
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float2 Tex : TEXCOORD;
};

struct VertexPosHWNormalTex
{
    float4 PosH : SV_POSITION;
    float3 PosW : POSITION; // �������е�λ��
    float3 NormalW : NORMAL; // �������������еķ���
    float2 Tex : TEXCOORD;
};





