#pragma once
#include "../../../EngineMinimal.h"
#include <DirectXMath.h>

struct FMaterialConstantBuffer
{
    FMaterialConstantBuffer();

    //x1-x3为占位符
    // 4个数为一组，不足要添加占位符,否则颜色异常
    int MateiralType;
    int x1;
    int x2;
    int x3;

    XMFLOAT4 BaseColor;
    float Roughness;
    XMFLOAT4X4 TransformInfo;
    
};