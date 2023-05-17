struct Light
{
    float3 LightIntensity;
    float FalloffStart; //point&&spot����ʼ˥������

    float3 LightDirection;
    float FalloffEnd;   //point&&spot�����˥������
    
    float3 Position;
    float LightType;
    
    float LightConeInnerRadians; //spot����׶�ڽ�
    float LightConeOuterRadians; //spot����׶���
    float x1;
    float x2;
};

// ��ȡLightDir
float3 GetLightDirection(Light L, float3 WorldVertexPos)
{   
    // ƽ�й�
    if (L.LightType == 0)
    {
        return L.LightDirection;
    }
    //���Դ
    else if (L.LightType == 1)
    {
        return L.Position - WorldVertexPos;
    }
    //���Դ
    else if (L.LightType == 2)
    {
        return L.Position - WorldVertexPos;
    }
    
    return L.LightDirection;
}

// ����˥����Ӳ�ߣ�
float LinearAttenuationFactor(Light L, float Distance)
{
    return (Distance / (L.FalloffEnd - L.FalloffStart));
}

// ����˥������ƽ����ͣ�
float CurveAttenuationFactor(Light L, float Distance, float C, float I, float Q)
{
    return  1.0f / (C + I * Distance + pow(Q, 2.0f) * Distance);
}

// ����˥������
float CalucAttenuationFactor(Light L, float3 WorldNormal, float3 WorldVertexPos, float3 WorldLightPos)
{
    // ƽ�йⲻ������
    if(L.LightType == 0)
    {
        return float3(1.0f, 1.0f, 1.0f);
    }
    
    // ���Դ����˥��
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
    
    // �۹�Ƽ���˥��
    else if (L.LightType == 2)
    {
        float3 LightVector = L.Position - WorldVertexPos;
        float Distance = length(LightVector);
        float LL = max(0, dot(WorldLightPos, L.LightDirection));
        
        float theta1 = acos(LL);  
        
        if (theta1 <= L.LightConeInnerRadians)
        {
            return L.LightIntensity.x;
        }
        else if (L.LightConeInnerRadians < theta1 <= L.LightConeOuterRadians)
        {
            float OuterInnerDistance = L.LightConeOuterRadians - L.LightConeInnerRadians;
            float CurrentDistance = OuterInnerDistance - (theta1 - OuterInnerDistance);
            return CurrentDistance / OuterInnerDistance;

        }
        //if (Distance <= L.FalloffEnd)
        //{
        //    float SpotFactor = pow(LL, 1.0f);
        //    return CurveAttenuationFactor(L, Distance, 2.0f, 2.0f, 2.0f) * SpotFactor;
        //}
    }
    
    return 0;

}