#pragma once
#include "ShellMeshComponent.h"

class CCylinderMeshComponent :public CShellMeshComponent
{
public:
	CCylinderMeshComponent();

    // 创建柱体（顶部半径，底部半径，高度，轴向细分数，高度细分数）
    void CreateMesh(
        FMeshRenderingData& MeshData,
        float InTopRadius,
        float InBottomRadius,
        float InHeight,
        uint32_t InAxialSubdivision,
        uint32_t InHeightSubdivision);
};
