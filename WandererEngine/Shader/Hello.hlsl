#include "Material.hlsl"
#include "Light.hlsl"

cbuffer MeshConstantBuffer : register(b0) //b0-b14
{
    float4x4 M;
}

cbuffer ViewportConstantBuffer : register(b1) 
{
    float4x4 VP;
}

cbuffer MaterialConstantBuffer : register(b2)
{
    // 4个数为一组，不足要添加占位符,否则颜色异常
    int MaterialType;
    int x1;
    int x2;
    int x3;
    
    float4 BaseColor;
    float4x4 TransformInfo;
}

cbuffer LightConstantBuffer : register(b3)
{
    float3 LightIntensity;
    float3 LightDirection;
}

struct a2v
{
    float3 vertex : POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
};

struct v2f
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
};

v2f VertexShaderMain(a2v v)
{
    v2f o;
    float4x4 MVP = mul(M, VP);
    o.pos = mul(float4(v.vertex, 1.0), MVP);
    o.normal = mul(v.normal, (float3x3)M);
    o.color = v.color;
    return o;
}

float4 PixelShaderMain(v2f o) : SV_TARGET
{
    float4 AmbientLight = { 0.1, 0.1, 0.1, 1.0 };  // 环境光
    
    float NL = dot(normalize(o.normal), normalize(LightDirection));
    
    // Lambert
    if(MaterialType == 0)
    {
        NL = max(0, NL);
    }
    //HalfLambert
    else if(MaterialType == 1)
    {
        NL = (NL * 0.5) + 0.5;
    }
    
    o.color = BaseColor * NL + AmbientLight;

    return o.color;
}