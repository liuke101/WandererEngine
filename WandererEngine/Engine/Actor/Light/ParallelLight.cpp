#include "ParallelLight.h"

GParallelLight::GParallelLight()
{
    SetLightComponent(CreateObject<CParallelLightComponent>(new CParallelLightComponent()));
}

void GParallelLight::Tick(float DeltaTime)
{
    // ƽ�й⻷����ת
    fvector_3d v3 = GetRotation();

    v3.x += DeltaTime * 30.0f;
    v3.y += DeltaTime * 30.0f;
    // v3.z += DeltaTime * 10.0f;
    SetRotation(v3);
}
