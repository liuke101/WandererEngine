#pragma once

#include "Core/Mesh.h"
// 方体
class GBoxMesh : public GMesh
{
    typedef GMesh Super;
public:
    virtual void Init();

    virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

    virtual void Draw(float DeltaTime);

    // 创建Box(高度，宽度，深度)
    void CreateMesh(
        FMeshRenderingData& MeshData,
        float InHeight,
        float InWidth,
        float InDepth);
};