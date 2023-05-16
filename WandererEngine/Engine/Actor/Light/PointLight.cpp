#include "PointLight.h"

GPointLight::GPointLight()
{
    SetLightComponent(CreateObject<CPointLightComponent>(new CPointLightComponent()));
}

void GPointLight::Tick(float DeltaTime)
{
    // fvector_3d v3 = GetRotation();
    //v3.x += DeltaTime * 30.0f;
    //v3.y += DeltaTime * 30.0f;
    // v3.z += DeltaTime * 10.0f;
    // SetRotation(v3);
}

void GPointLight::SetFalloffStart(float InNewFalloffStart)
{
    if (CPointLightComponent* InPointLightComponent = dynamic_cast<CPointLightComponent*>(const_cast<CLightComponent*> (GetLightComponent())))
    {
        return InPointLightComponent->SetFalloffStart(InNewFalloffStart);
    }
}

void GPointLight::SetFalloffEnd(float InNewFalloffEnd)
{
    if (CPointLightComponent* InPointLightComponent = dynamic_cast<CPointLightComponent*>(const_cast<CLightComponent*> (GetLightComponent())))
    {
        return InPointLightComponent->SetFalloffEnd(InNewFalloffEnd);
    }
}

float GPointLight::GetFalloffStart() const
{
    if(const CPointLightComponent* InPointLightComponent = dynamic_cast<const CPointLightComponent*>(GetLightComponent()))
    {
        return InPointLightComponent->GetFalloffStart();
    }

    return 0;
}

float GPointLight::GetFalloffEnd() const
{
    if (const CPointLightComponent* InPointLightComponent = dynamic_cast<const CPointLightComponent*>(GetLightComponent()))
    {
        return InPointLightComponent->GetFalloffEnd();
    }

    return 0;
}

