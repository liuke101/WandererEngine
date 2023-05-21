#pragma once
#include "../../../EngineMinimal.h"
#include <DirectXMath.h>

struct FMaterialConstantBuffer
{
    FMaterialConstantBuffer();

    //HLSL的常量缓冲区本身以及对它的读写操作需要严格按16字节对齐
    //x1-x3为占位符
    int MateiralType;
    float Roughness;
    int BasecolorIndex;
    int x3;

    XMFLOAT4 BaseColor;

    XMFLOAT4X4 MaterialTransformation;
};