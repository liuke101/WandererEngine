#pragma once
#include "Core/Light.h"
#include "../../Component/Light/PointLightComponent.h"

class GPointLight : public GLight
{
    typedef GActorObject Super;

    CVARIABLE()
        CPointLightComponent* PointlLightComponent;

public:
    GPointLight();

    virtual void Tick(float DeltaTime);
public:
    void SetFalloffStart(float InNewFalloffStart);
    void SetFalloffEnd(float InNewFalloffEnd);
public:
    float GetFalloffStart() const;
    float GetFalloffEnd() const;
};
