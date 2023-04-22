#include "CylinderMesh.h"
#include "Core/MeshType.h"

void CCylinderMesh::Init()
{
    Super::Init();
}

void CCylinderMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
    Super::BuildMesh(InRenderingData);
}

void CCylinderMesh::Draw(float DeltaTime)
{
    Super::Draw(DeltaTime);
}

void CCylinderMesh::CreateMesh(FMeshRenderingData& MeshData, float InTopRadius, float InBottomRadius, float InHeight,
    uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
    float RadiusInterval = (InTopRadius - InBottomRadius) / static_cast<float>(InHeightSubdivision); // 相邻环的半径间隔
    float HeightInterval = InHeight / static_cast<float>(InHeightSubdivision);                       // 相邻环的高度间隔
    float betaValue = XM_2PI / static_cast<float>(InAxialSubdivision);                               // 原点到目标点的连线在xz平面的投影线与正x轴之间的“方位角”

    /*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/
    // 构建侧面顶点
    // 从底面开始，从下至上计算每个环上的顶点坐标
    for(uint32_t i = 0; i <= InHeightSubdivision; ++i)
    {
        float iRadius = InBottomRadius + i * RadiusInterval;      // 第i环的半径
        float iHeight = -0.5f * InHeight + i * HeightInterval;    // 第i环的高度

        for(uint32_t j = 0; j <= InAxialSubdivision; ++j)
        {
            //球面坐标转换为笛卡尔坐标，y是确定的，计算xz时theta固定为90度
            MeshData.VertexData.push_back(FVertex(
                XMFLOAT3(
                    iRadius  * cosf(static_cast<float>(j) * betaValue),   // x
                    iHeight,                                              // y
                    iRadius * sinf(static_cast<float>(j) * betaValue)),   // z
                XMFLOAT4(Colors::White)));
        }
    }

    // 构建侧面索引
    uint32_t VertexCircleCount = InAxialSubdivision + 1;    // 环一圈的顶点数量，+1是希望让每环的第一个顶点和最后一个顶点重合，这是因为他们的纹理坐标不同
    for (uint32_t i = 0; i < InHeightSubdivision; ++i)
    {
        for (uint32_t j = 0; j < InAxialSubdivision; ++j)
        {
            // 绘制四边形（由两个三角形组成） 龙书P246
            // 三角形1
            MeshData.IndexData.push_back(i * VertexCircleCount + j);
            MeshData.IndexData.push_back((i + 1) * VertexCircleCount + j);
            MeshData.IndexData.push_back((i + 1) * VertexCircleCount + j + 1);
            // 三角形2
            MeshData.IndexData.push_back(i * VertexCircleCount + j);
            MeshData.IndexData.push_back((i + 1) * VertexCircleCount + j + 1);
            MeshData.IndexData.push_back(i * VertexCircleCount + j + 1);
        }
    }

    /*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/

    // 构建顶面面顶点
    // 使圆台端面环上的首尾顶点重合，因为这两个顶点的纹理坐标和法线是不同的
    uint32_t BaseIndex = static_cast<uint32_t>(MeshData.VertexData.size());  //这个BaseIndex很重要，不然索引顺序会发生错误
    float TopHeight = 0.5f * InHeight;

    for (uint32_t i = 0; i <= InHeightSubdivision; ++i)
    {
        //球面坐标转换为笛卡尔坐标，y是确定的，计算xz时theta固定为90度
        MeshData.VertexData.push_back(FVertex(
            XMFLOAT3(
                InTopRadius * cosf(i * betaValue),   // x
                TopHeight,                           // y
                InTopRadius * sinf(i * betaValue)),  // z
            XMFLOAT4(Colors::White)));
    }

    // 顶面的中心顶点
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.0f, TopHeight, 0.0f), XMFLOAT4(Colors::White)));
    uint32_t CenterIndex = static_cast<uint32_t>(MeshData.VertexData.size()) - 1;

    // 构建顶面索引
    for (uint32_t i = 0; i < InAxialSubdivision; ++i)
    {
        MeshData.IndexData.push_back(CenterIndex);
        MeshData.IndexData.push_back(BaseIndex + i + 1);
        MeshData.IndexData.push_back(BaseIndex + i);
    }

    /*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/

    // 构建底面顶点
    BaseIndex = static_cast<uint32_t>(MeshData.VertexData.size());
    float BottomHeight = -0.5f * InHeight;
    for (uint32_t i = 0; i <= InHeightSubdivision; ++i)
    {
        MeshData.VertexData.push_back(FVertex(
            XMFLOAT3(
                InBottomRadius * cosf(i * betaValue),   // x
                BottomHeight,                     // y
                InBottomRadius * sinf(i * betaValue)),  // z
            XMFLOAT4(Colors::White)));
    }

    MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.0f, BottomHeight, 0.0f), XMFLOAT4(Colors::White)));
    CenterIndex = static_cast<uint32_t>(MeshData.VertexData.size()) - 1;

    // 构建顶面索引
    for (uint32_t i = 0; i < InAxialSubdivision; ++i)
    {
        MeshData.IndexData.push_back(CenterIndex);
        MeshData.IndexData.push_back(BaseIndex + i + 1);
        MeshData.IndexData.push_back(BaseIndex + i);
    }
}

