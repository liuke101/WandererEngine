#pragma once
#include "../../Core/Engine.h"

struct FViewportInfo
{
public:
    XMFLOAT4 ViewportPosition;
    XMFLOAT4X4 ViewMatrix;
    XMFLOAT4X4 ProjectionMatrix;
};
