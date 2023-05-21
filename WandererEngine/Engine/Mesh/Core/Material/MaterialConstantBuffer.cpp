#include "MaterialConstantBuffer.h"
#include "MaterialType.h"
#include "../../../Math/EngineMath.h"
FMaterialConstantBuffer::FMaterialConstantBuffer()
    : MateiralType(Lambert)
    , Roughness(1.0f)
    , BasecolorIndex(-1)
    , BaseColor(1.0f, 1.0f, 1.0f, 1.0f)
    , MaterialTransformation(EngineMath::IdentityMatrix4x4())
{

}
