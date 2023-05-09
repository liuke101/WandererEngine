#pragma once
#include "../../../Core/CoreObject/CoreMinimalObject.h"
#include "MaterialType.h"

class CMaterial : public CCoreMinimalObject
{
public:
    CMaterial();

    void SetBaseColor(const fvector_4d& InBaseColor) { BaseColor = InBaseColor; }
    void SetMaterialType(const EMaterialType& InMaterialType){ MaterialType = InMaterialType; }

public:
    FORCEINLINE fvector_4d GetBaseColor() const { return BaseColor; }
    FORCEINLINE EMaterialType GetMaterialType() const { return MaterialType; }

private:
    fvector_4d BaseColor;           // 基础颜色
    EMaterialType MaterialType;     // 材质类型
};
