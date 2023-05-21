#include "ObjectTransformation.h"

FObjectTransformation::FObjectTransformation()
    : M(EngineMath::IdentityMatrix4x4())
    , TextureTransformation(EngineMath::IdentityMatrix4x4())
    , MaterialIndex(0)
    , x1(0)
    , x2(0)
    , x3(0)
{
}

