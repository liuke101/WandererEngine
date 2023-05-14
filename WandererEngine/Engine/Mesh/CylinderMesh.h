#pragma once
#include "Core/Mesh.h"

// 柱体
class GCylinderMesh : public GMesh
{
    typedef GMesh Super;
public:
    virtual void Init();

    virtual void Draw(float DeltaTime);

    // 创建柱体（顶部半径，底部半径，高度，轴向细分数，高度细分数）
    void CreateMesh(
        float InTopRadius, 
        float InBottomRadius, 
        float InHeight,
        uint32_t InAxialSubdivision, 
        uint32_t InHeightSubdivision);
};