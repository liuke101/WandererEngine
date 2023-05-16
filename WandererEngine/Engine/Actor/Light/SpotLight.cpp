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
