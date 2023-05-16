#include "Light.h"
#include "../../../Component/Light/Core/LightComponent.h"
GLight::GLight()
    : Super()
    , LightComponent(NULL)
{
}

void GLight::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void GLight::SetPosition(const XMFLOAT3& InNewPosition)
{
    Super::SetPosition(InNewPosition);

    if (LightComponent)
    {
        LightComponent->SetPosition(InNewPosition);
    }
}

void GLight::SetRotation(const fvector_3d& InNewRotation)
{
    Super::SetRotation(InNewRotation);

    if (LightComponent)
    {
        LightComponent->SetRotation(InNewRotation);
    }
}
void GLight::SetScale(const fvector_3d& InNewScale)
{
    Super::SetScale(InNewScale);

    if (LightComponent)
    {
        LightComponent->SetScale(InNewScale);
    }
}

void GLight::SetLightintensity(const fvector_3d& InNewLightintensity)
{
    if (LightComponent)
    {
        LightComponent->SetLightintensity(InNewLightintensity);
    }
}

void GLight::SetLightComponent(CLightComponent* InNewComponent)
{
    LightComponent = InNewComponent;
}
