#pragma once
#include "Core/Mesh.h"

// 柱体
class CCylinderMesh : public CMesh
{
    typedef CMesh Super;
public:
    virtual void Init();

    virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

    virtual void Draw(float DeltaTime);

    // 参考Maya模型参数：顶部半径，底部半径，高度，轴向细分数，高度细分数
    static CCylinderMesh* CreateMesh(float InTopRadius, float InBottomRadius, float InHeight, 
        uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);

};