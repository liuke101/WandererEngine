struct FMaterial
{
    //通用材质
    float4 BaseColor;
};

// Schlick Fresnel近似
float3 FresnelSchlick(float F0, float FresnelEXP, float3 viewDir, float3 normal)
{
    return F0 + (1 - F0) * pow(1 - dot(viewDir, normal), FresnelEXP);
}