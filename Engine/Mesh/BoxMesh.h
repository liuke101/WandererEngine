#pragma once

#include "Core/Mesh.h"
// ����
class CBoxMesh : public CMesh
{
    typedef CMesh Super;
public:
    virtual void Init();

    virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

    virtual void Draw(float DeltaTime);

    // ����Box(�߶ȣ���ȣ����)
    void CreateMesh(
        FMeshRenderingData& MeshData,
        float InHeight,
        float InWidth,
        float InDepth);
};