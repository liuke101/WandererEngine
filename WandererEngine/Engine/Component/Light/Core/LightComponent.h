#pragma once
#include "../../TransformationComponent.h"
#include "../../../Interface/DirectXDeviceInterface.h"
class CMeshComponent;

class CLightComponent : public CTransformationComponent, public IDirectXDeviceInterface
{
    typedef CTransformationComponent Super;

    CVARIABLE()
    CMeshComponent* LightMesh;

public:
    CLightComponent();
    virtual ~CLightComponent();

    CMeshComponent* GetLightMesh() { return LightMesh; }

    virtual void SetPosition(const XMFLOAT3& InNewPosition);
    virtual void SetRotation(const fvector_3d& InNewRotation);
    virtual void SetScale(const fvector_3d& InNewScale);

    virtual void SetRightVector(const XMFLOAT3& InRightVector);
    virtual void SetUpVector(const XMFLOAT3& InUpVector);
    virtual void SetLookatVector(const XMFLOAT3& InLookatVector);
protected:
    void setLightMesh(CMeshComponent* InLightMesh);
};
