
#include "CustomMesh.h"
#include "Core/MeshType.h"

void CCustomMesh::Init()
{
    Super::Init();
}

void CCustomMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
    Super::BuildMesh(InRenderingData);
}

void CCustomMesh::Draw(float DeltaTime)
{
    Super::Draw(DeltaTime);
}

CCustomMesh* CCustomMesh::CreateMesh()
{
    FMeshRenderingData MeshData;

    CCustomMesh* CustomMesh = new CCustomMesh;
    CustomMesh->BuildMesh(&MeshData);

    CustomMesh->Init();

    return CustomMesh;
}

