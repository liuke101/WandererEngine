#pragma once
#include "Core/Mesh.h"

// 球体
class GSphereMesh : public GMesh
{
    typedef GMesh Super;
public:
    virtual void Init();

    virtual void Draw(float DeltaTime);


    // 创建球体（半径，轴向细分数，高度细分数）
    void CreateMesh(float InRadius,uint32_t InAxialSubdivision, uint32_t InHeightSubdivision);
};