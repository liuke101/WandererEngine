#pragma once
#include "Core/Mesh.h"

// 锥体
class CConeMesh : public CMesh
{
    typedef CMesh Super;
public:
    virtual void Init();

    virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

    virtual void Draw(float DeltaTime);

    // 参考Maya模型参数：底部半径，高度，轴向细分数，高度细分数
    static CConeMesh* CreateMesh(float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);
};