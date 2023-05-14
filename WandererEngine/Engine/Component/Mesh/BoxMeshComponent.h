#pragma once
#include "ShellMeshComponent.h"

class CBoxMeshComponent :public CShellMeshComponent
{
public:
	CBoxMeshComponent();

    // 创建Box(高度，宽度，深度)
    void CreateMesh(
        FMeshRenderingData& MeshData,
        float InHeight,
        float InWidth,
        float InDepth);
};
