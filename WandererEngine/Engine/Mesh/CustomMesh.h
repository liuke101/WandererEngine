#pragma once
#include "Core/Mesh.h"

// 自定义Mesh
class CCustomMesh : public CMesh
{
    typedef CMesh Super;
public:
    virtual void Init();

    virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

    virtual void Draw(float DeltaTime);

    static CCustomMesh* CreateMesh();

};