#pragma once
#include "../../../EngineMinimal.h"

struct FLight
{
    FLight();
    XMFLOAT3 LightIntensity;
    float FalloffStart;

    XMFLOAT3 LightDirection;
    float FalloffEnd;
    
    XMFLOAT3 Position;
    float LightType;
};

struct FLightConstantBuffer
{
    FLightConstantBuffer();
    FLight SceneLight[16];
};

