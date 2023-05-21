#include "Material.h"
#include "../../../Math/EngineMath.h"
CMaterial::CMaterial()
    : BaseColor(1.0f,1.0f,1.0f,1.0f)
    , Roughness(1.0f)
    , MaterialType(Lambert)
    , bDirty(true)
    , MaterialIndex(-1)
    , MaterialDisplayStatus(TriangleDisplay)
    , MaterialTransformation(EngineMath::IdentityMatrix4x4())
{

}

void CMaterial::SetMaterialDisplayStatus(EMaterialDisplayStatusType InDisplayStatus)
{
    MaterialDisplayStatus = InDisplayStatus;
    SetDirty(true);
}

void CMaterial::SetBaseColor(const fvector_4d& InBaseColor)
{
    BaseColor = InBaseColor;
    SetDirty(true);
}

void CMaterial::SetRoughness(const float& InRoughness)
{
    Roughness = InRoughness;
    SetDirty(true);
}

void CMaterial::SetMaterialType(const EMaterialType& InMaterialType)
{
    MaterialType = InMaterialType;
    SetDirty(true);
}

void CMaterial::SetBaseColor(const std::string& InAssetFilename)
{
    BasecolorIndexKey = InAssetFilename;
    SetDirty(true);
}

void CMaterial::SetDirty(bool bNewDirty)
{
    bDirty = bNewDirty;
}

void CMaterial::SetMaterialIndex(int InMaterialIndex)
{
    MaterialIndex = InMaterialIndex;
    SetDirty(true);
}


