#include "SphereMesh.h"
#include "Core/MeshType.h"

void CSphereMesh::Init()
{
    Super::Init();
}

void CSphereMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
    Super::BuildMesh(InRenderingData);
}

void CSphereMesh::Draw(float DeltaTime)
{
    Super::Draw(DeltaTime);
}

// 算法参考龙书
CSphereMesh* CSphereMesh::CreateMesh(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
    FMeshRenderingData MeshData;
    FVertex NorthPoleVertex(XMFLOAT3(0.f, InRadius, 0.f), XMFLOAT4(Colors::Red));     // 北极点（顶面顶点/第一个顶点）
    FVertex SouthPoleVertex(XMFLOAT3(0.f, -InRadius, 0.f), XMFLOAT4(Colors::Red));    // 南极点（底面顶点/最后一个顶点）

    // 北极点
    MeshData.VertexData.push_back(NorthPoleVertex);

    // 保存法线数据
    // int NorthPoleIndex = MeshData.VertexData.size() - 1;        //获取最后一个顶点数据（刚添加的）的索引
    // XMVECTOR NorthPolePos = XMLoadFloat3(&MeshData.VertexData[NorthPoleIndex].Position);
    // XMStoreFloat3(&MeshData.VertexData[NorthPoleIndex].Normal, XMVector3Normalize(NorthPolePos));   // 保存顶点法线数据

    // 每层的顶点（忽略极点所在层）
    float thetaValue = XM_PI / InHeightSubdivision;
    float betaValue = XM_2PI / InAxialSubdivision;
    for (uint32_t i = 1; i <= InHeightSubdivision - 1; ++i)     
    {
        float theta = i * thetaValue;
        
        for (uint32_t j = 0; j <= InAxialSubdivision; ++j)   
        {
            float beta = j * betaValue;

            MeshData.VertexData.push_back(FVertex(
                XMFLOAT3(InRadius * sinf(theta) * cosf(beta), InRadius * cosf(theta), InRadius * sinf(theta) * sinf(beta)),  //球面坐标转换为笛卡尔坐标
                XMFLOAT4(Colors::White)));

            // int VertexIndex = MeshData.VertexData.size() - 1;   
            // XMVECTOR VertexPos = XMLoadFloat3(&MeshData.VertexData[VertexIndex].Position);
            // XMStoreFloat3(&MeshData.VertexData[VertexIndex].Normal, XMVector3Normalize(VertexPos));
        }
    }

    // 南极点
    MeshData.VertexData.push_back(SouthPoleVertex);
    // int SouthPoleIndex = MeshData.VertexData.size() - 1;
    // XMVECTOR SouthPolePos = XMLoadFloat3(&MeshData.VertexData[SouthPoleIndex].Position);
    // XMStoreFloat3(&MeshData.VertexData[SouthPoleIndex].Normal, XMVector3Normalize(SouthPolePos));

    // 绘制北极(围绕第一个顶点，逆时针方向)
    for (uint32_t i = 1; i <= InAxialSubdivision; ++i)
    {
        MeshData.IndexData.push_back(0);
        MeshData.IndexData.push_back(i + 1);
        MeshData.IndexData.push_back(i);
    }
    
    // 绘制侧面
    // 将索引偏移到第一个环中第一个顶点的索引，跳过顶部极点顶点。
    uint32_t BaseIndex = 1;
    uint32_t VertexCircleCount = InAxialSubdivision + 1;           // 一圈的顶点数量
    for(uint32_t i = 0; i < InHeightSubdivision - 2; ++i)
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
    
    
    // 绘制南极（围绕最后一个顶点）
    uint32_t southPoleIndex = (uint32_t)MeshData.VertexData.size() - 1;   // 南极点是最后一个顶点
    BaseIndex = southPoleIndex - VertexCircleCount;                       // 将索引偏移到最后一个环中第一个顶点的索引。
    for (uint32_t i = 0; i < InAxialSubdivision; ++i)
    {
        MeshData.IndexData.push_back(southPoleIndex);
        MeshData.IndexData.push_back(BaseIndex + i);
        MeshData.IndexData.push_back(BaseIndex + i + 1);
    }


    CSphereMesh* SphereMesh = new CSphereMesh;
    SphereMesh->BuildMesh(&MeshData);
    SphereMesh->Init();
    return SphereMesh;
}
