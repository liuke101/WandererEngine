#include "Mesh.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Component/TransformationComponent.h"
#include "Material/Material.h"
#include "../../Component/Mesh/ShellMeshComponent.h"

GMesh::GMesh()
    : GActorObject()
{
    ShellMeshComponent = CreateObject<CShellMeshComponent>(new CShellMeshComponent());
}

void GMesh::Init()
{
    if (ShellMeshComponent)
    {
        ShellMeshComponent->Init();
    }
}

void GMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
    if(ShellMeshComponent)
    {
        ShellMeshComponent->BuildMesh(InRenderingData);
    }
    

}

void GMesh::PreDraw(float DeltaTime)
{
    
}

void GMesh::Draw(float DeltaTime)
{
    
}

void GMesh::PostDraw(float DeltaTime)
{
    

}

UINT GMesh::GetMaterialNum() const
{
    return ShellMeshComponent->GetMaterialNum();
}

vector<CMaterial*>* GMesh::GetMaterials()
{
    return ShellMeshComponent->GetMaterials();
}
