#pragma once
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"
#include "../../Actor/Core/ActorObject.h"

class CTransformationComponent;

// Mesh����
class GMesh : public GActorObject, public IRenderingInterface
{
    CVARIABLE()
    CTransformationComponent* TransformationComponent;

public:
    GMesh();

    virtual void Init();

    virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

    virtual void PreDraw(float DeltaTime);
    virtual void Draw(float DeltaTime);
    virtual void PostDraw(float DeltaTime);
    
};