#pragma once
#include "Core/RangeLight.h"

class GSpotLight : public GRangeLight
{
    typedef GRangeLight Super;

public:
    GSpotLight();

    virtual void Tick(float DeltaTime);

    void SetLightConeInnerDegrees(float InLightConeInnerDegrees);
    void SetLightConeOuterDegrees(float InLightConeOuterDegrees);

    float GetLightConeInnerDegrees() const;
    float GetLightConeOuterDegrees() const;
};


