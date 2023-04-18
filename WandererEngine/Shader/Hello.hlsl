// ÉùÃ÷³£Á¿
cbuffer ConstBuffer : register(b0) //b0-b14
{
    float4x4 World;
}

struct a2v
{
    float3 vertex : POSITION;
    float4 color : COLOR;
};

struct v2f
{
    float4 pos : SV_POSITION;
    float4 color : COLOR;
};

v2f VertexShader(a2v v)
{
    v2f o;
    o.pos = mul(float4(v.vertex, 1.0f), World);
    o.color = v.color;

    return o;
}

float4 PixelShader(v2f o) : SV_TARGET
{
    return o.color;
}