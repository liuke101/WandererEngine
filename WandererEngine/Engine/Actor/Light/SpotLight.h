#pragma once
#include "Core/Light.h"
#include "../../Component/Light/SpotLightComponent.h"

class GSpotLight : public GLight
{
    typedef GActorObject Super;

    CVARIABLE()
        CSpotLightComponent* SpotlLightComponent;

public:
    GSpotLight();

    virtual void Tick(float DeltaTime);
public:
    void SetFalloffStart(float InNewFalloffStart);
    void SetFalloffEnd(float InNewFalloffEnd);
public:
    float GetFalloffStart() const;
    float GetFalloffEnd() const;
};
