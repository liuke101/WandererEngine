#pragma once
#include "../../../EngineMinimal.h"

struct FLight
{
    FLight();
    XMFLOAT3 LightIntensity;
    float FalloffStart;        //point&&spot，开始衰减距离

    XMFLOAT3 LightDirection;
    float FalloffEnd;          //point&&spot，最大衰减距离
    
    XMFLOAT3 Position;
    float LightType;

    float LightConeInnerRadians; //spot，光锥内角（弧度）
    float LightConeOuterRadians; //spot，光锥外角（弧度）
    float x1;
    float x2;
};

struct FLightConstantBuffer
{
    FLightConstantBuffer();
    FLight SceneLights[16];
};

