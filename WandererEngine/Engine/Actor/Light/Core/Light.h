#pragma once
#include "../../Core/ActorObject.h"


class CLightComponent;

class GLight : public GActorObject
{
    typedef GActorObject Super;

    CVARIABLE()
        CLightComponent* LightComponent;

public:
    GLight();

    virtual void Tick(float DeltaTime);
public:
    virtual void SetPosition(const XMFLOAT3& InNewPosition);
    virtual void SetRotation(const fvector_3d& InNewRotation);
    virtual void SetScale(const fvector_3d& InNewScale);

public:
    void SetLightintensity(const fvector_3d& InNewLightintensity);

protected:
    virtual void SetLightComponent(CLightComponent* InNewComponent);

public:
    const CLightComponent* GetLightComponent() const { return LightComponent; }
};