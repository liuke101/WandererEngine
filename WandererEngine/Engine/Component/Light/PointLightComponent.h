#pragma once
#include "Core/LightComponent.h"

class CMeshComponent;
class CPointLightComponent : public CLightComponent
{
    typedef CLightComponent Super;
public:
    CPointLightComponent();

public:
    void SetFalloffStart(float InNewFalloffStart) { FalloffStart = InNewFalloffStart; }
    void SetFalloffEnd(float InNewFalloffEnd) { FalloffEnd = InNewFalloffEnd; }
public:
    float GetFalloffStart() const { return FalloffStart; }
    float GetFalloffEnd() const { return FalloffEnd; }
protected:
    float FalloffStart;
    float FalloffEnd;
};