#include "RangeLight.h"
#include "../../../Component/Light/Core/RangeLightComponent.h"


void GRangeLight::SetFalloffStart(float InNewFalloffStart)
{
    if (CRangeLightComponent* InRangeLightComponent = dynamic_cast<CRangeLightComponent*>(const_cast<CLightComponent*> (GetLightComponent())))
    {
        return InRangeLightComponent->SetFalloffStart(InNewFalloffStart);
    }
}

void GRangeLight::SetFalloffEnd(float InNewFalloffEnd)
{
    if (CRangeLightComponent* InRangeLightComponent = dynamic_cast<CRangeLightComponent*>(const_cast<CLightComponent*> (GetLightComponent())))
    {
        return InRangeLightComponent->SetFalloffEnd(InNewFalloffEnd);
    }
}

float GRangeLight::GetFalloffStart() const
{
    if (const CRangeLightComponent* InRangeLightComponent = dynamic_cast<const CRangeLightComponent*>(GetLightComponent()))
    {
        return InRangeLightComponent->GetFalloffStart();
    }

    return 0;
}

float GRangeLight::GetFalloffEnd() const
{
    if (const CRangeLightComponent* InRangeLightComponent = dynamic_cast<const CRangeLightComponent*>(GetLightComponent()))
    {
        return InRangeLightComponent->GetFalloffEnd();
    }

    return 0;
}

