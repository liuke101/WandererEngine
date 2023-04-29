#include "RenderingData.h"
#include "../../../../../Mesh/Core/MeshType.h"

FRenderingData::FRenderingData()
    : VertexCount(0)
    , VertexTypeSize(sizeof(FVertex))
    , VertexOffsetPosition(0)
    , IndexCount(0)
    , IndexTypeSize(sizeof(uint16_t))
    , IndexOffsetPosition(0)
    , IndexFormat(DXGI_FORMAT_R16_UINT)
    , ModelMatrix(EngineMath::IdentityMatrix4x4())
    , Mesh(NULL)
{
}
