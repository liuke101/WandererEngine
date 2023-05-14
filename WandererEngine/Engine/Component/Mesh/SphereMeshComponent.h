#pragma once
#include "ShellMeshComponent.h"

class CSphereMeshComponent :public CShellMeshComponent
{
public:
	CSphereMeshComponent();

	// 创建球体（半径，轴向细分数，高度细分数）
	void CreateMesh(FMeshRenderingData& MeshData, float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);
};
