#pragma once
#include "Core/Mesh.h"

// 球体
class CSphereMesh : public CMesh
{
    typedef CMesh Super;
public:
    virtual void Init();

    virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

    virtual void Draw(float DeltaTime);


    // 创建球体（半径，轴向细分数，高度细分数）
    void CreateMesh(FMeshRenderingData& MeshData, float InRadius,uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);
};