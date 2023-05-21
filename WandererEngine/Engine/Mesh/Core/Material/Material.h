#pragma once
#include "../../../Core/CoreObject/CoreMinimalObject.h"
#include "MaterialType.h"

class CMaterial : public CCoreMinimalObject
{
public:
    CMaterial();

    void SetMaterialDisplayStatus(EMaterialDisplayStatusType InDisplayStatus);
    void SetBaseColor(const fvector_4d& InBaseColor);
    void SetRoughness(const float& InRoughness);
    void SetMaterialType(const EMaterialType& InMaterialType);
    void SetBaseColor(const std::string& InAssetFilename);
    void SetDirty(bool bNewDirty);
    void SetMaterialIndex(int InMaterialIndex);
public:
    FORCEINLINE fvector_4d GetBaseColor() const { return BaseColor; }
    FORCEINLINE float GetRoughness() const { return Roughness; }
    FORCEINLINE EMaterialType GetMaterialType() const { return MaterialType; }
    FORCEINLINE EMaterialDisplayStatusType GetMaterialDisplayStatus()const { return MaterialDisplayStatus; }
    FORCEINLINE XMFLOAT4X4& GetMaterialTransformation() { return MaterialTransformation; }
    FORCEINLINE const std::string& GetBasecolorIndexKey() const { return BasecolorIndexKey; }
    bool IsDirty() const { return bDirty; }
    FORCEINLINE int GetMaterialIndex() const { return MaterialIndex; }
private:
    fvector_4d BaseColor;           // 基础颜色
    float Roughness;                // 粗糙度
    EMaterialType MaterialType;     // 材质类型
    bool bDirty;                    // 判断材质是否被动态更改，被更改就进行更新
    int MaterialIndex;
    std::string BasecolorIndexKey;
    EMaterialDisplayStatusType MaterialDisplayStatus;
    XMFLOAT4X4 MaterialTransformation;
};
