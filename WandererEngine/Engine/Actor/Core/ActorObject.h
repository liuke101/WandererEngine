#pragma once
#include "../../EngineMinimal.h"
#include "../../Core/CoreObject/CoreMinimalObject.h"

class CTransformationComponent;

//相当于UE的Actor，G = Game 
class GActorObject :public CCoreMinimalObject
{
    CVARIABLE()
    CTransformationComponent* TransformationComponent;

public:
    GActorObject();
public:
    FORCEINLINE CTransformationComponent* GetTransformationComponent() const { return TransformationComponent; }
public:
    virtual void SetPosition(const XMFLOAT3& InNewPosition);
    virtual void SetRotation(const fvector_3d& InNewRotation);
    virtual void SetScale(const fvector_3d& InNewScale);

    XMFLOAT3& GetPosition();
    fvector_3d GetRotation() const;
    fvector_3d GetScale() const;

    XMFLOAT3& GetRightVector();
    XMFLOAT3& GetUpVector();
    XMFLOAT3& GetLookatVector();

};