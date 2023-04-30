#include "ActorObject.h"
#include "../../Component/TransformationComponent.h"

GActorObject::GActorObject()
{
    TransformationComponent = CreateObject<CTransformationComponent>(new CTransformationComponent());
}

void GActorObject::SetPosition(const XMFLOAT3& InNewPosition)
{
    TransformationComponent->SetPosition(InNewPosition);
}

void GActorObject::SetRotation(const fvector_3d& InNewRotation)
{
    TransformationComponent->SetRotation(InNewRotation);
}

void GActorObject::SetScale(const fvector_3d& InNewScale)
{
    TransformationComponent->SetScale(InNewScale);
}

XMFLOAT3& GActorObject::GetPosition()
{
    return TransformationComponent->GetPosition();
}

fvector_3d GActorObject::GetRotation() const
{
    return TransformationComponent->GetRotation();
}

fvector_3d GActorObject::GetScale() const
{
    return TransformationComponent->GetScale();
}

XMFLOAT3& GActorObject::GetRightVector()
{
    return TransformationComponent->GetRightVector();
}

XMFLOAT3& GActorObject::GetUpVector()
{
    return TransformationComponent->GetUpVector();
}

XMFLOAT3& GActorObject::GetLookatVector()
{
    return TransformationComponent->GetLookatVector();
}


