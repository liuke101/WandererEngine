#pragma once
#include "../Core/ActorObject.h"
#include "../../Component/Light/ParallelLightComponent.h"

class GParallelLight : public GActorObject
{
    CVARIABLE()
    CParallelLightComponent* ParallelLightComponent;
};
