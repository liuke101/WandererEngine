#pragma once
#include "../../TransformationComponent.h"
#include "../../../Interface/DirectXDeviceInterface.h"
#include "LightType.h"

class CMeshComponent;

class CLightComponent : public CTransformationComponent, public IDirectXDeviceInterface
{
    typedef CTransformationComponent Super;

    CVARIABLE()
    CMeshComponent* LightMesh;

public:
    CLightComponent();
    virtual ~CLightComponent();
    void SetLightintensity(const fvector_3d& InNewLightintensity) { Lightintensity = InNewLightintensity; }

public:
    CMeshComponent* GetLightMesh() { return LightMesh; }
    const fvector_3d& GetLightintensity() { return Lightintensity; }
    ELightType& GetLightType() { return LightType; }
public:
    virtual void SetPosition(const XMFLOAT3& InNewPosition);
    virtual void SetRotation(const fvector_3d& InNewRotation);
    virtual void SetScale(const fvector_3d& InNewScale);

    virtual void SetRightVector(const XMFLOAT3& InRightVector);
    virtual void SetUpVector(const XMFLOAT3& InUpVector);
    virtual void SetLookatVector(const XMFLOAT3& InLookatVector);
protected:
    void SetLightMesh(CMeshComponent* InLightMesh);
protected:
    fvector_3d Lightintensity;
    ELightType LightType;
};
