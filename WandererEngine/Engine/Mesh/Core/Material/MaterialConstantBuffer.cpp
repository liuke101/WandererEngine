#include "MaterialConstantBuffer.h"
#include "MaterialType.h"
#include "../../../Math/EngineMath.h"
FMaterialConstantBuffer::FMaterialConstantBuffer()
    : MateiralType(Lambert)
    , BaseColor(1.0f, 1.0f, 1.0f, 1.0f)
    , Roughness(1.0f)
    , TransformInfo(EngineMath::IdentityMatrix4x4())
{

}
