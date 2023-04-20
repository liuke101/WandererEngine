#pragma once
/* 球体 */
#include "Core/Mesh.h"

class CSphereMesh : public CMesh
{
    typedef CMesh Super;
public:
    virtual void Init();

    virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

    virtual void Draw(float DeltaTime);

    static CSphereMesh* CreateMesh();
};