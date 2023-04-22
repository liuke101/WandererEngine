#include "ConeMesh.h"
#include "Core/MeshType.h"

void CConeMesh::Init()
{
    Super::Init();
}

void CConeMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
    Super::BuildMesh(InRenderingData);
}

void CConeMesh::Draw(float DeltaTime)
{
    Super::Draw(DeltaTime);
}

CConeMesh* CConeMesh::CreateMesh(float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
    FMeshRenderingData MeshData;

    float RadiusInterval = (0 - InBottomRadius) / static_cast<float>(InHeightSubdivision); // 相邻环的半径间隔(类比柱体算法，顶部半径视为0)
    float HeightInterval = InHeight / static_cast<float>(InHeightSubdivision);             // 相邻环的高度间隔
    float betaValue = XM_2PI / static_cast<float>(InAxialSubdivision);                     // 原点到目标点的连线在xz平面的投影线与正x轴之间的“方位角”

    /*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/
    // 【构建顶点】
    // 锥点
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.0f, 0.5f * InHeight, 0.0f), XMFLOAT4(Colors::Red)));

    // 从顶点开始，从上至下计算每个环上的顶点坐标
    for (uint32_t i = 0; i < InHeightSubdivision; ++i)
    {
        float iRadius = i * RadiusInterval;      // 第i环的半径
        float iHeight = 0.5f * InHeight - i * HeightInterval;    // 第i环的高度

        for (uint32_t j = 0; j <= InAxialSubdivision; ++j)
        {
            //球面坐标转换为笛卡尔坐标，y是确定的，计算xz时theta固定为90度
            MeshData.VertexData.push_back(FVertex(
                XMFLOAT3(
                    iRadius * cosf(static_cast<float>(j) * betaValue),    // x
                    iHeight,                                 // y
                    iRadius * sinf(static_cast<float>(j) * betaValue)),   // z
                XMFLOAT4(Colors::White)));
        }
    }

    // 底面中点
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(0.f, -0.5f * InHeight, 0.f), XMFLOAT4(Colors::Red)));

    /*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/
    //【构建索引】
    // 围绕锥点
    for (uint32_t i = 0; i <= InAxialSubdivision; ++i)
    {
        MeshData.IndexData.push_back(0);
        MeshData.IndexData.push_back(i + 1);
        MeshData.IndexData.push_back(i);
    }

    // 侧面
    // 将索引偏移到第一个环中第一个顶点的索引，跳过顶部极点顶点。
    uint32_t BaseIndex = 1;
    uint32_t VertexCircleCount = InAxialSubdivision + 1;    // 环一圈的顶点数量，+1是希望让每环的第一个顶点和最后一个顶点重合，这是因为他们的纹理坐标不同
    for (uint32_t i = 0; i < InHeightSubdivision - 1; ++i)
    {
        for (uint32_t j = 0; j < InAxialSubdivision; ++j)
        {
            // 绘制四边形（由两个三角形组成）
            // 三角形1
            MeshData.IndexData.push_back(BaseIndex + i * VertexCircleCount + j);
            MeshData.IndexData.push_back(BaseIndex + i * VertexCircleCount + j + 1);
            MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleCount + j);
            // 三角形2
            MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleCount + j);
            MeshData.IndexData.push_back(BaseIndex + i * VertexCircleCount + j + 1);
            MeshData.IndexData.push_back(BaseIndex + (i + 1) * VertexCircleCount + j + 1);
        }
    }
    
    // 底面
    uint32_t BottomCenterIndex = static_cast<uint32_t>(MeshData.VertexData.size()) - 1;   // 底面中点是最后一个顶点
    BaseIndex = BottomCenterIndex - VertexCircleCount;                       // 将索引偏移到最后一个环中第一个顶点的索引。
    for (uint32_t i = 0; i < InAxialSubdivision; ++i)
    {
        MeshData.IndexData.push_back(BottomCenterIndex);
        MeshData.IndexData.push_back(BaseIndex + i);
        MeshData.IndexData.push_back(BaseIndex + i + 1);
    }

    /*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/
    //【构建模型】
    CConeMesh* ConeMesh = new CConeMesh;
    ConeMesh->BuildMesh(&MeshData);
    ConeMesh->Init();

    return ConeMesh;
}