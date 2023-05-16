#include "PointLightComponent.h"
#include "../../Mesh/Core/MeshManage.h"
#include "../Mesh/Core/MeshComponent.h"
#include "../../Mesh/Core/Material/Material.h"

CPointLightComponent::CPointLightComponent()
    : Super()
    , FalloffStart(0.0f)
    , FalloffEnd(10.0f)
{
    // 读取模型资源
    string MeshPath = "../WandererEngine/Asset/PointLight.obj";
    SetLightMesh(GetMeshManage()->CreateCustomMeshComponent(MeshPath));

    if (GetLightMesh())
    {
        if (CMaterial* InMaterial = (*GetLightMesh()->GetMaterials())[0])
        {
            InMaterial->SetMaterialType(BaseColor);
            InMaterial->SetMaterialDisplayStatus(WireframeDisplay);
            InMaterial->SetBaseColor(fvector_4d(1.0f, 1.0f, 1.0f, 1.0f));
        }
    }

    LightType = PointLight;
}
