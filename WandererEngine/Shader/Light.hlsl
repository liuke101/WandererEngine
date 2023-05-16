struct Light
{
    float3 LightIntensity;
    float FalloffStart; //point&&spot，开始衰减距离

    float3 LightDirection;
    float FalloffEnd;   //point&&spot，最大衰减距离
    
    float3 Position;
    float LightType;
    
    float LightConeInnerRadians; //spot，光锥内角
    float LightConeOuterRadians; //spot，光锥外角
    float x1;
    float x2;
};

// 获取LightDir
float3 GetLightDirection(Light L, float3 WorldVertexPos)
{   
    // 平行光
    if (L.LightType == 0)
    {
        return L.LightDirection;
    }
    //点光源
    else if (L.LightType == 1)
    {
        return L.Position - WorldVertexPos;
    }
    //点光源
    else if (L.LightType == 2)
    {
        return L.Position - WorldVertexPos;
    }
    
    return L.LightDirection;
}

// 线性衰减（硬边）
float LinearAttenuationFactor(Light L, float Distance)
{
    return (Distance / (L.FalloffEnd - L.FalloffStart));
}

// 曲线衰减（更平滑柔和）
float CurveAttenuationFactor(Light L, float Distance, float C, float I, float Q)
{
    return  1.0f / (C + I * Distance + pow(Q, 2.0f) * Distance);
}

// 计算衰减因子
float CalucAttenuationFactor(Light L, float3 WorldNormal, float3 WorldVertexPos, float3 WorldLightPos)
{
    // 平行光不做处理
    if(L.LightType == 0)
    {
        return float3(1.0f, 1.0f, 1.0f);
    }
    
    // 点光源计算衰减
    else if(L.LightType == 1)
    {
        float3 LightVector = L.Position - WorldVertexPos;
        float Distance = length(LightVector);
        if(Distance <= L.FalloffEnd)
        {
            //return LinearAttenuationFactor(L, Distance);
            return CurveAttenuationFactor(L, Distance, 2.0f, 2.0f, 2.0f);
        }
    }
    
    // 聚光灯计算衰减
    else if (L.LightType == 2)
    {
        float3 LightVector = L.Position - WorldVertexPos;
        float Distance = length(LightVector);
        if (Distance <= L.FalloffEnd)
        {
            float SpotFactor = pow(max(0, dot(WorldLightPos, L.LightDirection)), 1.0f);
            return CurveAttenuationFactor(L, Distance, 2.0f, 2.0f, 2.0f) * SpotFactor;
        }
    }
    
    return 0;

}