#pragma once
#include "Core/Light.h"
#include "../../Component/Light/ParallelLightComponent.h"

class GParallelLight : public GLight
{
    typedef GActorObject Super;

public:
    GParallelLight();

    virtual void Tick(float DeltaTime);
};
