#pragma once
#include "Core/RangeLight.h"

class GPointLight : public GRangeLight
{
    typedef GActorObject Super;


public:
    GPointLight();

    virtual void Tick(float DeltaTime);

};
