#include "BoxMesh.h"
#include "Core/MeshType.h"

void GBoxMesh::Init()
{
    Super::Init();
}

void GBoxMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
    Super::BuildMesh(InRenderingData);
}

void GBoxMesh::Draw(float DeltaTime)
{
    Super::Draw(DeltaTime);
}

void GBoxMesh::CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, float InDepth)
{
    // 茅參2宴噐繍庁侏譜崔葎參弊順恫炎圻泣葎嶄伉
    float Height = 0.5f * InHeight;
    float Width = 0.5f * InWidth;
    float Depth = 0.5f * InDepth;

    /*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/
    // ‐更秀競泣／
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(-Width, -Height, -Depth), XMFLOAT4(Colors::White)));
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(-Width, Height, -Depth), XMFLOAT4(Colors::White)));
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(Width, Height, -Depth), XMFLOAT4(Colors::White)));
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(Width, -Height, -Depth), XMFLOAT4(Colors::White)));
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(-Width, -Height, Depth), XMFLOAT4(Colors::White)));
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(-Width, Height, Depth), XMFLOAT4(Colors::White)));
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(Width, Height, Depth), XMFLOAT4(Colors::White)));
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(Width, -Height, Depth), XMFLOAT4(Colors::White)));

    /*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/
    // ‐更秀沫哈／
    // 念
    MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(2);
    MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(2); MeshData.IndexData.push_back(3);

    // 朔
    MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(5);
    MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(7); MeshData.IndexData.push_back(6);

    // 恣
    MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(5); MeshData.IndexData.push_back(1);
    MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(0);

    // 嘔
    MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(2); MeshData.IndexData.push_back(6);
    MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(7);

    // 貧
    MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(5); MeshData.IndexData.push_back(6);
    MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(2);

    // 和
    MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(3);
    MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(7);
}
