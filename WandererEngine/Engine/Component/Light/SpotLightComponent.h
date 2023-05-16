#pragma once
#include "Core/LightComponent.h"
#include "Core/RangeLightComponent.h"

class CSpotLightComponent : public CRangeLightComponent
{
    typedef CRangeLightComponent Super;
public:
    CSpotLightComponent();


    void SetLightConeInnerDegrees(float InLightConeInnerDegrees);
    void SetLightConeOuterDegrees(float InLightConeOuterDegrees);
    
    float GetLightConeInnerDegrees() { return LightConeInnerDegrees; }
    float GetLightConeOuterDegrees() { return LightConeOuterDegrees; }
protected:
    float LightConeInnerDegrees; //spot，光锥内角(角度)
    float LightConeOuterDegrees; //spot，光锥外角(角度)
};