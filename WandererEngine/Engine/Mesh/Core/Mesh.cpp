#include "Mesh.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Component/TransformationComponent.h"
#include "Material/Material.h"
#include "../../Component/Mesh/ShellMeshComponent.h"

GMesh::GMesh()
    : GActorObject()
{
}

void GMesh::Init()
{
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

void GMesh::SetPosition(const XMFLOAT3& InNewPosition)
{
    Super::SetPosition(InNewPosition);

    //因为还未做根组件，暂时这么写
    GetMeshComponent()->SetPosition(InNewPosition);
}

void GMesh::SetRotation(const fvector_3d& InRotation)
{

    Super::SetRotation(InRotation);

    //因为还未做根组件，暂时这么写
    GetMeshComponent()->SetRotation(InRotation);
}

void GMesh::SetScale(const fvector_3d& InNewScale)
{
    Super::SetScale(InNewScale);

    //因为还未做根组件，暂时这么写
    GetMeshComponent()->SetScale(InNewScale);
}

void GMesh::SetMeshComponent(CMeshComponent* InMeshComponent)
{
    MeshComponent = InMeshComponent;
}

UINT GMesh::GetMaterialNum() const
{
    return MeshComponent->GetMaterialNum();
}

vector<CMaterial*>* GMesh::GetMaterials()
{
    return MeshComponent->GetMaterials();
}
