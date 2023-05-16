#include "PointLight.h"
#include "../../Component/Light/PointLightComponent.h"
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
