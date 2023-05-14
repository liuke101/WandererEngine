#pragma once
#include "ShellMeshComponent.h"

class CCustomMeshComponent :public CShellMeshComponent
{
public:
	CCustomMeshComponent();

	void CreateMesh(FMeshRenderingData& MeshData, string& InPath);

	// 加载OBJ模型，建模导出时必须进行面三角化并展UV
	static bool LoadObjFromBuffer(char* InBuffer, uint32_t InBufferSize, FMeshRenderingData &InMeshData);
};
