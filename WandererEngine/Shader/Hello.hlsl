#include "Material.hlsl"
#include "Light.hlsl"

cbuffer MeshConstantBuffer : register(b0) //b0-b14
{
    float4x4 M;
}

cbuffer ViewportConstantBuffer : register(b1)
{
    float4 ViewportPosition;
    float4x4 VP;
}

cbuffer MaterialConstantBuffer : register(b2)
{
    int MaterialType;
    int x1;
    int x2;
    int x3;
    
    float4 BaseColor;

    float Roughness;
    float xx1;
    float xx2;
    float xx3;

    float4x4 TransformInfo;
}

cbuffer LightConstantBuffer : register(b3)
{
    float3 LightIntensity;
    float xxx1;

    float3 LightDirection;
    float xxxx1;
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
    float4 worldpos : POSITION;
};

v2f VertexShaderMain(a2v v)
{
    v2f o;
    float4x4 MVP = mul(M, VP);
    o.pos = mul(float4(v.vertex, 1.0), MVP); // ��������任���ü��ռ�
    o.normal = mul(v.normal, (float3x3) M); // �任������ռ�
    o.worldpos = mul(float4(v.vertex, 1.0), M); // �任������ռ�
    o.color = v.color;
    return o;
}

float4 PixelShaderMain(v2f o) : SV_TARGET
{
    // ��Ҫ��Ϊ�����ϲ��ʣ�ֱ����BaseColorҲ����
    FMaterial Material;
    Material.BaseColor = BaseColor;

    float3 Ambient = { 0.1, 0.1, 0.1 }; // ������
    float3 N = normalize(o.normal); // ����ռ䷨��
    float3 L = normalize(LightDirection); // ����ռ��Դλ��
    float3 V = normalize(ViewportPosition.xyz - o.worldpos.xyz); // �������
    float3 R = normalize(reflect(-L, N)); // ��������
    float3 H = normalize(V + L); // �������

    float NL = dot(N, L);
    float VR = dot(V, R);
    float NH = dot(N, H);
    float NV = dot(N, V);

    float3 Diffuse = { 0.0, 0.0, 0.0 };

    float3 Specular = { 0.0, 0.0, 0.0 };
    float SpecularEXP = 32 * (1 - saturate(Roughness));
    float SpecularScale = 10;

    float3 Fresnel = { 0.0, 0.0, 0.0 };
    float F0 = 0.028; // ����ϵ��
    float FresnelEXP = 3; // Fresnelָ��
    

    // Lambert
    if (MaterialType == 0)
    {
        Diffuse = max(0, NL);
    }
    //HalfLambert
    else if (MaterialType == 1)
    {
        Diffuse = (NL * 0.5) + 0.5;
    }
    //phong
    else if (MaterialType == 2)
    {
        Diffuse = (NL * 0.5) + 0.5;
        Specular = pow(max(0, VR), SpecularEXP) * SpecularScale;
    }
    // BlinnPhong
    else if (MaterialType == 3)
    {
        Diffuse = (NL * 0.5) + 0.5;
        Specular = pow(max(0, NH), SpecularEXP) * SpecularScale;
    }
    // WarpLight
    else if (MaterialType == 4)
    {
        float warp = 5;
        Diffuse = max(0, (NL + warp) / (1 + warp));
    }
    // Minnaert
    else if (MaterialType == 5)
    {
        Diffuse = max(0, pow((NL * NV), Roughness) * NL);
    }
    // Banded
    else if (MaterialType == 6)
    {
        float Layer = 5;
        float3 Banded = floor(((NL * 0.5) + 0.5) * Layer) / Layer;
        float3 ColorA = { 1.0, 0.0, 0.0 };
        float3 ColorB = { 0.0, 0.0, 1.0 };
        // ����
        Diffuse = lerp(ColorA, ColorB, Banded);
    }
     // Banded
    else if (MaterialType == 7)
    {
        float Layer = 5;
        float3 Banded = floor(((NL * 0.5) + 0.5) * Layer) / Layer;
        float3 ColorA = { 1.0, 0.0, 0.0 };
        float3 ColorB = { 0.0, 0.0, 1.0 };
        // ����
        Diffuse = lerp(ColorA, ColorB, Banded);
        Fresnel = FresnelSchlick(F0, FresnelEXP, N, V);
    }
    

    
    // Fresnel
    else if (MaterialType == 100)
    {
        Fresnel = FresnelSchlick(F0, FresnelEXP, N, V);
    }
    

    o.color = Material.BaseColor * float4(Ambient + Diffuse + Specular + Fresnel, 1.0);

    return o.color;
}