#include "BoxMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CBoxMeshComponent::CBoxMeshComponent()
{

}

void CBoxMeshComponent::CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, float InDepth)
{
    // 除以2便于将模型设置为以世界坐标原点为中心
    float Height = 0.5f * InHeight;
    float Width = 0.5f * InWidth;
    float Depth = 0.5f * InDepth;

    /*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/
    // 【构建顶点】
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(-Width, -Height, -Depth), XMFLOAT4(Colors::White)));
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(-Width, Height, -Depth), XMFLOAT4(Colors::White)));
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(Width, Height, -Depth), XMFLOAT4(Colors::White)));
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(Width, -Height, -Depth), XMFLOAT4(Colors::White)));
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(-Width, -Height, Depth), XMFLOAT4(Colors::White)));
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(-Width, Height, Depth), XMFLOAT4(Colors::White)));
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(Width, Height, Depth), XMFLOAT4(Colors::White)));
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(Width, -Height, Depth), XMFLOAT4(Colors::White)));

    /*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/
    // 【构建索引】
    // 前
    MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(2);
    MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(2); MeshData.IndexData.push_back(3);

    // 后
    MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(5);
    MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(7); MeshData.IndexData.push_back(6);

    // 左
    MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(5); MeshData.IndexData.push_back(1);
    MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(0);

    // 右
    MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(2); MeshData.IndexData.push_back(6);
    MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(7);

    // 上
    MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(5); MeshData.IndexData.push_back(6);
    MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(2);

    // 下
    MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(3);
    MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(7);
}
