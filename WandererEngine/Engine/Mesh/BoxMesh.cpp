#include "BoxMesh.h"
#include "Core/MeshType.h"

void CBoxMesh::Init()
{
    Super::Init();
}

void CBoxMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
    Super::BuildMesh(InRenderingData);
}

void CBoxMesh::Draw(float DeltaTime)
{
    Super::Draw(DeltaTime);
}

CBoxMesh* CBoxMesh::CreateMesh(float InHeight, float InWidth, float InDepth)
{
    FMeshRenderingData MeshData;

    // ����2���ڽ�ģ������Ϊ����������ԭ��Ϊ����
    float Height = 0.5f * InHeight;
    float Width = 0.5f * InWidth;
    float Depth = 0.5f * InDepth;

    /*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/
    // ���������㡿
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(-Width, -Height, -Depth), XMFLOAT4(Colors::White)));
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(-Width, Height, -Depth), XMFLOAT4(Colors::White)));
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(Width, Height, -Depth), XMFLOAT4(Colors::White)));
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(Width, -Height, -Depth), XMFLOAT4(Colors::White)));
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(-Width, -Height, Depth), XMFLOAT4(Colors::White)));
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(-Width, Height, Depth), XMFLOAT4(Colors::White)));
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(Width, Height, Depth), XMFLOAT4(Colors::White)));
    MeshData.VertexData.push_back(FVertex(XMFLOAT3(Width, -Height, Depth), XMFLOAT4(Colors::White)));

    /*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/
    // ������������
    // ǰ
    MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(2);
    MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(2); MeshData.IndexData.push_back(3);

    // ��
    MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(5);
    MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(7); MeshData.IndexData.push_back(6);

    // ��
    MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(5); MeshData.IndexData.push_back(1);
    MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(0);

    // ��
    MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(2); MeshData.IndexData.push_back(6);
    MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(7);

    // ��
    MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(5); MeshData.IndexData.push_back(6);
    MeshData.IndexData.push_back(1); MeshData.IndexData.push_back(6); MeshData.IndexData.push_back(2);

    // ��
    MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(0); MeshData.IndexData.push_back(3);
    MeshData.IndexData.push_back(4); MeshData.IndexData.push_back(3); MeshData.IndexData.push_back(7);

    /*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/
    // ������ģ�͡�
    CBoxMesh* BoxMesh= new CBoxMesh;
    BoxMesh->BuildMesh(&MeshData);
    BoxMesh->Init();

    return BoxMesh;
}
