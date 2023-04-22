#pragma once
/* Mesh基类 */
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"

// Mesh基类
class CMesh : public CCoreMinimalObject, public IRenderingInterface
{
public:
    CMesh();

    virtual void Init();

    virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

    virtual void PreDraw(float DeltaTime);
    virtual void Draw(float DeltaTime);
    virtual void PostDraw(float DeltaTime);
    
};