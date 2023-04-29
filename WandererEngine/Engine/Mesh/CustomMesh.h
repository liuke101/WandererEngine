#pragma once
#include "Core/Mesh.h"

// 自定义Mesh
class GCustomMesh : public GMesh
{
    typedef GMesh Super;
public:
    virtual void Init();

    virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

    virtual void Draw(float DeltaTime);

    void CreateMesh(FMeshRenderingData& MeshData, string &InPath);

    // 加载OBJ模型，建模导出时必须进行面三角化并展UV
    static bool LoadObjFromBuffer(char* InBuffer, uint32_t InBufferSize, FMeshRenderingData &InMeshData);
};