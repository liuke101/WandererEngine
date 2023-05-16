#pragma once
#include "Light.h"



class GRangeLight : public GLight
{
    typedef GLight Super;

public:
    void SetFalloffStart(float InNewFalloffStart);
    void SetFalloffEnd(float InNewFalloffEnd);
public:
    float GetFalloffStart() const;
    float GetFalloffEnd() const;

};