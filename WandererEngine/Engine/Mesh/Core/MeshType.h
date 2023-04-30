#pragma once
/* Mesh描述 */
#include "../../EngineMinimal.h"
#include "../../Math/EngineMath.h"

//  顶点数据
struct FVertex
{
    FVertex(const XMFLOAT3 &InPos, const XMFLOAT4 &InColor);

    XMFLOAT3 Position;
    XMFLOAT4 Color;
    XMFLOAT3 Normal;
};


struct FMeshRenderingData
{
    std::vector<FVertex> VertexData;
    std::vector<uint16_t> IndexData;

public:
    // VertexData大小（字节）
    UINT GetVertexSizeInBytes()
    {
        return VertexData.size() * sizeof(FVertex);
    }

    // IndexData大小（字节）
    UINT GetIndexSizeInBytes()
    {
        return IndexData.size() * sizeof(uint16_t);
    }

};

