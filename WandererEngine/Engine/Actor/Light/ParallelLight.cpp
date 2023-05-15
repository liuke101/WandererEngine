#include "ParallelLight.h"

GParallelLight::GParallelLight()
{
    ParallelLightComponent = CreateObject<CParallelLightComponent>(new CParallelLightComponent());
}

void GParallelLight::Tick(float DeltaTime)
{
    // 平行光环绕旋转
    fvector_3d v3 = GetRotation();

    v3.x += DeltaTime * 10.0f;
    //v3.y += DeltaTime * 10.0f;
    // v3.z += DeltaTime * 10.0f;
    SetRotation(v3);
}

void GParallelLight::SetPosition(const XMFLOAT3& InNewPosition)
{
    Super::SetPosition(InNewPosition);

    ParallelLightComponent->SetPosition(InNewPosition);
}

void GParallelLight::SetRotation(const fvector_3d& InNewRotation)
{
    Super::SetRotation(InNewRotation);

    ParallelLightComponent->SetRotation(InNewRotation);
}

void GParallelLight::SetScale(const fvector_3d& InNewScale)
{
    Super::SetScale(InNewScale);

    ParallelLightComponent->SetScale(InNewScale);
}
