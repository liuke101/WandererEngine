#pragma once
#include "Core/Mesh.h"

// 自定义Mesh
class GCustomMesh : public GMesh
{
    typedef GMesh Super;
public:
    virtual void Init();

    virtual void Draw(float DeltaTime);

    void CreateMesh(string &InPath);

};