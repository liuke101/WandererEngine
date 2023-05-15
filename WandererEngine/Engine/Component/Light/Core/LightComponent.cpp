#include "LightComponent.h"
#include "../../../Manage/LightManage.h"
#include "../../Mesh/Core/MeshComponent.h"
CLightComponent::CLightComponent()
{
    GetLightManage()->AddLight(this);
}

CLightComponent::~CLightComponent()
{
    // 以后再说
}

void CLightComponent::SetPosition(const XMFLOAT3& InNewPosition)
{
    Super::SetPosition(InNewPosition);
    LightMesh->SetPosition(InNewPosition);
}

void CLightComponent::SetRotation(const fvector_3d& InNewRotation)
{
    Super::SetRotation(InNewRotation);
    LightMesh->SetRotation(InNewRotation * (-1.f));
}

void CLightComponent::SetScale(const fvector_3d& InNewScale)
{
    Super::SetScale(InNewScale);
    LightMesh->SetScale(InNewScale);
}

void CLightComponent::SetRightVector(const XMFLOAT3& InRightVector)
{
    Super::SetRightVector(InRightVector);
    LightMesh->SetRightVector(InRightVector);
}

void CLightComponent::SetUpVector(const XMFLOAT3& InUpVector)
{
    Super::SetUpVector(InUpVector);
    LightMesh->SetUpVector(InUpVector);
}

void CLightComponent::SetLookatVector(const XMFLOAT3& InLookatVector)
{
    Super::SetLookatVector(InLookatVector);
    LightMesh->SetLookatVector(InLookatVector);
}

void CLightComponent::setLightMesh(CMeshComponent* InLightMesh)
{
    LightMesh = InLightMesh;
}
