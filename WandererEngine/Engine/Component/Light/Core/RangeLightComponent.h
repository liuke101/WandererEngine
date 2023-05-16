#pragma once
#include "LightComponent.h"


class CRangeLightComponent : public CLightComponent
{
    typedef CLightComponent Super;

    CVARIABLE()
        CMeshComponent* RangeLightMesh;

public:
    CRangeLightComponent();

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
