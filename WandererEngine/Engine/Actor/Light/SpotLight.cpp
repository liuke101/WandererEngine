#include "SpotLight.h"
#include "../../Component/Light/SpotLightComponent.h"

GSpotLight::GSpotLight()
{
    SetLightComponent(CreateObject<CSpotLightComponent>(new CSpotLightComponent()));
}

void GSpotLight::Tick(float DeltaTime)
{
    fvector_3d v3 = GetRotation();
    //v3.x += DeltaTime * 30.0f;
    v3.y += DeltaTime * 30.0f;
    //v3.z += DeltaTime * 10.0f;
    SetRotation(v3);
}

void GSpotLight::SetLightConeInnerDegrees(float InLightConeInnerDegrees)
{
    if (CSpotLightComponent* InSpotLightComponent = dynamic_cast<CSpotLightComponent*>(const_cast<CLightComponent*> (GetLightComponent())))
    {
        return InSpotLightComponent->SetLightConeInnerDegrees(InLightConeInnerDegrees);
    }
}

void GSpotLight::SetLightConeOuterDegrees(float InLightConeOuterDegrees)
{
    if (CSpotLightComponent* InSpotLightComponent = dynamic_cast<CSpotLightComponent*>(const_cast<CLightComponent*> (GetLightComponent())))
    {
        return InSpotLightComponent->SetLightConeOuterDegrees(InLightConeOuterDegrees);
    }
}

float GSpotLight::GetLightConeInnerDegrees() const
{
    if (const CSpotLightComponent* InSpotLightComponent = dynamic_cast<const CSpotLightComponent*>(GetLightComponent()))
    {
        return InSpotLightComponent->GetLightConeInnerDegrees();
    }
}

float GSpotLight::GetLightConeOuterDegrees() const
{
    if (const CSpotLightComponent* InSpotLightComponent = dynamic_cast<const CSpotLightComponent*>(GetLightComponent()))
    {
        return InSpotLightComponent->GetLightConeOuterDegrees();
    }
}
