#pragma once
#include "../../../EngineMinimal.h"
#include <DirectXMath.h>

struct FMaterialConstantBuffer
{
    FMaterialConstantBuffer();

    //x1-x3为占位符
    int MateiralType;
    int x1;
    int x2;
    int x3;

    XMFLOAT4 BaseColor;
    XMFLOAT4X4 TransformInfo;
    
};