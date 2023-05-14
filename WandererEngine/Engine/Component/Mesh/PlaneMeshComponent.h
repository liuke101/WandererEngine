#pragma once
#include "ShellMeshComponent.h"

class CPlaneMeshComponent :public CShellMeshComponent
{
public:
	CPlaneMeshComponent();

    // 创建平面(高度，宽度，高度细分，宽度细分)
    void CreateMesh(
        FMeshRenderingData& MeshData,
        float InHeight,
        float InWidth,
        uint32_t InHeightSubdivide,
        uint32_t InWidthSubdivide);
};
