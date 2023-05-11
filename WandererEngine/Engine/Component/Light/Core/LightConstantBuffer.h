#pragma once
#include "../../../EngineMinimal.h"
struct FLightConstantBuffer
{
    FLightConstantBuffer();

    XMFLOAT3 LightIntensity;
    float x1;

    XMFLOAT3 LightDirection;
    float xx1;
};

