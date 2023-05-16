#include "SpotLight.h"

GSpotLight::GSpotLight()
{
    SetLightComponent(CreateObject<CSpotLightComponent>(new CSpotLightComponent()));
}

void GSpotLight::Tick(float DeltaTime)
{
    // fvector_3d v3 = GetRotation();
    //v3.x += DeltaTime * 30.0f;
    //v3.y += DeltaTime * 30.0f;
    // v3.z += DeltaTime * 10.0f;
    // SetRotation(v3);
}

void GSpotLight::SetFalloffStart(float InNewFalloffStart)
{
    if (CSpotLightComponent* InSpotLightComponent = dynamic_cast<CSpotLightComponent*>(const_cast<CLightComponent*> (GetLightComponent())))
    {
        return InSpotLightComponent->SetFalloffStart(InNewFalloffStart);
    }
}

void GSpotLight::SetFalloffEnd(float InNewFalloffEnd)
{
    if (CSpotLightComponent* InSpotLightComponent = dynamic_cast<CSpotLightComponent*>(const_cast<CLightComponent*> (GetLightComponent())))
    {
        return InSpotLightComponent->SetFalloffEnd(InNewFalloffEnd);
    }
}

float GSpotLight::GetFalloffStart() const
{
    if (const CSpotLightComponent* InSpotLightComponent = dynamic_cast<const CSpotLightComponent*>(GetLightComponent()))
    {
        return InSpotLightComponent->GetFalloffStart();
    }

    return 0;
}

float GSpotLight::GetFalloffEnd() const
{
    if (const CSpotLightComponent* InSpotLightComponent = dynamic_cast<const CSpotLightComponent*>(GetLightComponent()))
    {
        return InSpotLightComponent->GetFalloffEnd();
    }

    return 0;
}

