#include "Material.hlsl"
#include "Light.hlsl"

SamplerState SimpleTextureState : register(s0);

Texture2D SimpleTexture2DMap[TEXTURE2D_MAP_NUM] : register(t4);

cbuffer MeshConstantBuffer : register(b0) //b0-b14
{
    float4x4 M;
    float4x4 TextureTransformation;

    uint MaterialIndex;
    uint x1;    // 占位
    uint x2;
    uint x3;
}

cbuffer ViewportConstantBuffer : register(b1)
{
    float4 ViewportPosition;
    float4x4 VP;
}

struct MaterialConstantBuffer
{
    int MaterialType;
    float Roughness;
    int BasecolorIndex;
    int x3;
    
    float4 BaseColor;

    float4x4 MaterialTransformation;
};

StructuredBuffer<MaterialConstantBuffer> Materials : register(t0, space1);



cbuffer LightConstantBuffer : register(b3)
{
    Light SceneLights[16];
}

struct a2v
{
    float3 vertex : POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD;
};

struct v2f
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float4 worldpos : POSITION;
    float3 tangent : TANGENT;
    float2 uv : TEXCOORD;
};



v2f VertexShaderMain(a2v v)
{
    MaterialConstantBuffer MaterialCB = Materials[MaterialIndex];

    v2f o;
    float4x4 MVP = mul(M, VP);
    o.pos = mul(float4(v.vertex, 1.0), MVP); // 顶点坐标变换到裁剪空间

    
    if (MaterialCB.MaterialType == 13) // LocalNormal
    {
        o.normal = v.normal;
    }
    else
    {
        o.normal = mul(v.normal, (float3x3) M); // 变换到世界空间
    }

    o.worldpos = mul(float4(v.vertex, 1.0), M); // 变换到世界空间
    o.tangent = v.tangent;
    o.color = v.color;

    float4 uv = mul(float4(v.uv, 0.0, 1.0), TextureTransformation);
    o.uv = mul(uv, MaterialCB.MaterialTransformation).xy;

    return o;
}

float4 PixelShaderMain(v2f o) : SV_TARGET
{
    MaterialConstantBuffer MaterialCB = Materials[MaterialIndex];
    // 主要是为了整合材质，直接用BaseColor也可以
    FMaterial Material;

    if(MaterialCB.BasecolorIndex == -1)
    {
        Material.BaseColor = MaterialCB.BaseColor;
    }
    else
    {
        Material.BaseColor = MaterialCB.BaseColor * SimpleTexture2DMap[MaterialCB.BasecolorIndex].Sample(SimpleTextureState, o.uv); //纹理采样
    }

    

    // BaseColor
    if (MaterialCB.MaterialType == 12)
    {
        return Material.BaseColor;
    }
    // LocalNormal或WorldNormal
    else if (MaterialCB.MaterialType == 13 || MaterialCB.MaterialType == 14)
    {
        return float4(o.normal, 1.0);
    }
    

    float3 Ambient = { 0.1, 0.1, 0.1 }; // 环境光
    float3 AllLightsIntensity = { 0.0f, 0.0f, 0.0f }; // 总光强度

    float3 N = normalize(o.normal); // 世界空间法线
    float3 V = normalize(ViewportPosition.xyz - o.worldpos.xyz); // 相机方向
    float NV = dot(N, V);

    float3 Diffuse = { 0.0, 0.0, 0.0 };

    float3 Specular = { 0.0, 0.0, 0.0 };
    float SpecularEXP = 32 * (1 - saturate(MaterialCB.Roughness));
    float SpecularScale = 10;

    float3 Fresnel = { 0.0, 0.0, 0.0 };
    float F0 = 0.028; // 反射系数
    float FresnelEXP = 3; // Fresnel指数

    // 多光源
    for (int i = 0; i < 16; i++)
    {
        if (length(SceneLights[i].LightIntensity.xyz) > 0.0f)
        {
            
            float3 L = normalize(GetLightDirection(SceneLights[i], o.worldpos.xyz)); // 世界空间光源位置
            float3 R = normalize(reflect(-L, N)); // 反射向量
            float3 H = normalize(V + L); // 半程向量
            float NL = dot(N, L);
            float VR = dot(V, R);
            float NH = dot(N, H);

            // 衰减因子
            float AttenuationFactor = CalucAttenuationFactor(SceneLights[i], o.worldpos.xyz, L);

            // Lambert
            if (MaterialCB.MaterialType == 0)
            {
                Diffuse = max(0, NL);
            }
            //HalfLambert
            else if (MaterialCB.MaterialType == 1)
            {
                Diffuse = (NL * 0.5) + 0.5;
            }
            //phong
            else if (MaterialCB.MaterialType == 2)
            {
                Diffuse = (NL * 0.5) + 0.5;
                Specular = pow(max(0, VR), SpecularEXP) * SpecularScale;
            }
            // BlinnPhong
            else if (MaterialCB.MaterialType == 3)
            {
                Diffuse = (NL * 0.5) + 0.5;
                Specular = pow(max(0, NH), SpecularEXP) * SpecularScale;
            }
            // WarpLight
            else if (MaterialCB.MaterialType == 4)
            {
                float warp = 5;
                Diffuse = max(0, (NL + warp) / (1 + warp));
            }
            // Minnaert
            else if (MaterialCB.MaterialType == 5)
            {
                Diffuse = max(0, pow((NL * NV), MaterialCB.Roughness) * NL);
            }
            // Banded
            else if (MaterialCB.MaterialType == 6)
            {
                float Layer = 5;
                float3 Banded = floor(((NL * 0.5) + 0.5) * Layer) / Layer;
                float3 ColorA = { 1.0, 0.0, 0.0 };
                float3 ColorB = { 0.0, 0.0, 1.0 };
            // 渐变
                Diffuse = lerp(ColorA, ColorB, Banded);
            }
            // Fresnel
            else if (MaterialCB.MaterialType == 100)
            {
                Fresnel = FresnelSchlick(F0, FresnelEXP, N, V);
            }

            AllLightsIntensity += AttenuationFactor * Diffuse * SceneLights[i].LightIntensity;
        }
    }

    o.color = float4(AllLightsIntensity, 1.0) * (Material.BaseColor + float4(Specular + Fresnel, 1.0)) + (Material.BaseColor * float4(Ambient, 1.0));

    return o.color;
}