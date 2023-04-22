#pragma once
#include "Core/Mesh.h"

// 自定义Mesh
class CCustomMesh : public CMesh
{
    typedef CMesh Super;
public:
    virtual void Init();

    virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

    virtual void Draw(float DeltaTime);

    static CCustomMesh* CreateMesh(string &InPath);

    // 加载OBJ模型，建模导出时必须进行面三角化并展UV
    static bool LoadObjFromBuffer(char* InBuffer, uint32_t InBufferSize, FMeshRenderingData &InMeshData);
};