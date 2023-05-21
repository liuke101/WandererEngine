#pragma once
/* Mesh描述 */
#include "../../EngineMinimal.h"
#include "../../Math/EngineMath.h"

//  顶点数据
struct FVertex
{
    FVertex(const XMFLOAT3 &InPos, const XMFLOAT4 &InColor);
    FVertex(const XMFLOAT3& InPos, const XMFLOAT4& InColor, const XMFLOAT3& InNormal, const XMFLOAT2& InTexCoord = XMFLOAT2(0.0f,0.0f));
    XMFLOAT3 Position;
    XMFLOAT4 Color;
    XMFLOAT3 Normal;
    XMFLOAT3 Tangent;
    XMFLOAT2 TexCoord;
};


struct FMeshRenderingData
{
    std::vector<FVertex> VertexData;
    std::vector<uint16_t> IndexData;

public:
    // VertexData大小（字节）
    UINT GetVertexSizeInBytes() { return VertexData.size() * sizeof(FVertex); }

    // IndexData大小（字节）
    UINT GetIndexSizeInBytes() { return IndexData.size() * sizeof(uint16_t); }

};

