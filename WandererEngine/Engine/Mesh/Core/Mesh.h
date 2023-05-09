#pragma once
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"
#include "../../Actor/Core/ActorObject.h"

class CTransformationComponent;
class CMaterial;

// Mesh基类
class GMesh : public GActorObject, public IRenderingInterface
{
    CVARIABLE()
    CTransformationComponent* TransformationComponent;

    CVARIABLE()
    vector<CMaterial*> Materials;
public:
    GMesh();

    virtual void Init();

    virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

    virtual void PreDraw(float DeltaTime);
    virtual void Draw(float DeltaTime);
    virtual void PostDraw(float DeltaTime);

public:
    UINT GetMaterialNum() const;
    vector<CMaterial*>* GetMaterials() { return &Materials; }  //  获取材质

};