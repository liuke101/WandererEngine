#pragma once
#include "../../../EngineMinimal.h"
#include <DirectXMath.h>

struct FMaterialConstantBuffer
{
    FMaterialConstantBuffer();

    //HLSL的常量缓冲区本身以及对它的读写操作需要严格按16字节对齐
    //x1-x3为占位符
    int MateiralType;
    int x1;
    int x2;
    int x3;

    XMFLOAT4 BaseColor;

    float Roughness;
    float xx1;
    float xx2;
    float xx3;

    XMFLOAT4X4 TransformInfo;
};