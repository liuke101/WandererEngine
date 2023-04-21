﻿#pragma once
#include "Core/Mesh.h"

// 平面
class CPlaneMesh : public CMesh
{
    typedef CMesh Super;
public:
    virtual void Init();

    virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

    virtual void Draw(float DeltaTime);

    // (高度，宽度，高度细分，宽度细分)
    static CPlaneMesh* CreateMesh(float InHeight,float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide);
};