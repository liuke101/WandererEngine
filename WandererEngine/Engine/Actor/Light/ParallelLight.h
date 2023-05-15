#pragma once
#include "../Core/ActorObject.h"
#include "../../Component/Light/ParallelLightComponent.h"

class GParallelLight : public GActorObject
{
    typedef GActorObject Super;

    CVARIABLE()
    CParallelLightComponent* ParallelLightComponent;

public:
    GParallelLight();

    virtual void Tick(float DeltaTime);
public:
    virtual void SetPosition(const XMFLOAT3& InNewPosition);
    virtual void SetRotation(const fvector_3d& InNewRotation);
    virtual void SetScale(const fvector_3d& InNewScale);

};
