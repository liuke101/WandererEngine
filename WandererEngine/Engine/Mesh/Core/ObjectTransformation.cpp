﻿#include "ObjectTransformation.h"

FObjectTransformation::FObjectTransformation()
    :MVP(FObjectTransformation::IdentityMatrix4x4())
{
}

// 4x4单位矩阵
XMFLOAT4X4 FObjectTransformation::IdentityMatrix4x4()
{
    return XMFLOAT4X4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);
}
