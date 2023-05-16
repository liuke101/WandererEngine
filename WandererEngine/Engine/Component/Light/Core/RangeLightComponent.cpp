#include "RangeLightComponent.h"
#include "../../../Manage/LightManage.h"
#include "../../Mesh/Core/MeshComponent.h"
CRangeLightComponent::CRangeLightComponent()
    : Super()
    , FalloffStart(0.0f)
    , FalloffEnd(10.0f)
{
    GetLightManage()->AddLight(this);
}
