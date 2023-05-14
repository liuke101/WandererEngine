#pragma once
#include "../../TransformationComponent.h"

struct FMeshRenderingData;
class CMaterial;

class CMeshComponent : public CTransformationComponent
{
    CVARIABLE()
    vector<CMaterial*> Materials;
public:
    CMeshComponent();
public:
    virtual void Init();

    virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

    UINT GetMaterialNum() const;
    vector<CMaterial*>* GetMaterials() { return &Materials; }  //  获取材质
};
