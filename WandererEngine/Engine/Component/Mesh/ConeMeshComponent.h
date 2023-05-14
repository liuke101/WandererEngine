#pragma once
#include "ShellMeshComponent.h"

class CConeMeshComponent :public CShellMeshComponent
{
public:
	CConeMeshComponent();

    // 创建锥体（底部半径，高度，轴向细分数，高度细分数）
    void CreateMesh(
        FMeshRenderingData& MeshData,
        float InBottomRadius,
        float InHeight,
        uint32_t InAxialSubdivision,
        uint32_t InHeightSubdivision);
};
