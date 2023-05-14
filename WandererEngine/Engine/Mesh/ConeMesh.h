#pragma once
#include "Core/Mesh.h"

// 锥体
class GConeMesh : public GMesh
{
    typedef GMesh Super;
public:
    virtual void Init();

    virtual void Draw(float DeltaTime);

    // 创建锥体（底部半径，高度，轴向细分数，高度细分数）
    void CreateMesh(
        float InBottomRadius, 
        float InHeight, 
        uint32_t InAxialSubdivision, 
        uint32_t InHeightSubdivision);
};