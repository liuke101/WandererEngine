cbuffer ObjectConstBuffer : register(b0) //b0-b14
{
    float4x4 M;
}

cbuffer ViewportConstBuffer : register(b1) 
{
    float4x4 VP;
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

v2f VertexShaderMain(a2v v)
{
    v2f o;

    float4x4 MVP = mul(M, VP);

    o.pos = mul(float4(v.vertex, 1.0f), MVP);

    o.color = v.color;

    return o;
}

float4 PixelShaderMain(v2f o) : SV_TARGET
{
    return o.color;
}