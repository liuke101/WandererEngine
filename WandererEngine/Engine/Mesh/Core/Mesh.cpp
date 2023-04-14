#include "Mesh.h"

void FMesh::Init()
{

}

void FMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{

}

void FMesh::Draw(float DeltaTime)
{
}

FMesh* FMesh::CreateMesh(const FMeshRenderingData* InRenderingData)
{
    FMesh* InMesh = new FMesh(); 
        InMesh->BuildMesh(InRenderingData);
    return nullptr;
}


