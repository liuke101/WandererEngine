#include "ViewportTransformation.h"

FViewportTransformation::FViewportTransformation()
    : ViewportPosition(0.0f, 0.0f, 0.0f, 1.0f)
    , VP(EngineMath::IdentityMatrix4x4())
{
}
