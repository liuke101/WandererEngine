#include "GeometryMap.h"
#include "../../../Buffer/ConstructBuffer.h"
bool FGeometry::bRenderingDataExistence(CMesh* InKey)
{
    for(auto &Tmp : DescribeMeshRenderingData)
    {
        if(Tmp.Mesh == InKey)
        {
            return  true;
        }
    }
    return false;
}

void FGeometry::BuildMesh(CMesh* InMesh, const FMeshRenderingData& MeshData)
{
    if(!bRenderingDataExistence(InMesh))
    {
        DescribeMeshRenderingData.push_back(FRenderingData());
        FRenderingData& InRenderingData = DescribeMeshRenderingData[DescribeMeshRenderingData.size() - 1];

        // 属性设置
        InRenderingData.Mesh = InMesh;
        InRenderingData.VertexCount = MeshData.IndexData.size();
        InRenderingData.VertexOffsetPosition = MeshRenderingData.VertexData.size();
        InRenderingData.IndexCount = MeshData.IndexData.size();
        InRenderingData.IndexOffsetPosition = MeshRenderingData.IndexData.size();


        // 顶点/索引合并
        MeshRenderingData.VertexData.insert(MeshRenderingData.VertexData.end(), MeshData.IndexData.begin(), MeshData.IndexData.end());
        MeshRenderingData.IndexData.insert(MeshRenderingData.IndexData.end(), MeshData.IndexData.begin(), MeshData.IndexData.end());
    }
}

void FGeometry::Build()
{
    // 计算顶点/索引缓冲区大小（字节）
    UINT VertexSizeInBytes = MeshRenderingData.GetVertexSizeInBytes();
    UINT IndexSizeInBytes = MeshRenderingData.GetIndexSizeInBytes();

    // 创建CPU顶点/索引缓冲区
    ANALYSIS_HRESULT(D3DCreateBlob(VertexSizeInBytes, &CPUVertexBufferPtr));
    memcpy(CPUVertexBufferPtr->GetBufferPointer(), MeshRenderingData.VertexData.data(), VertexSizeInBytes);
    ANALYSIS_HRESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));
    memcpy(CPUIndexBufferPtr->GetBufferPointer(), MeshRenderingData.IndexData.data(), IndexSizeInBytes);

    // 创建GPU顶点/索引缓冲区
    ConstructBuffer::FConstructBuffer ConstructBuffer;
    GPUVertexBufferPtr = ConstructBuffer.ConstructDefaultBuffer(VertexUploadBufferPtr, MeshRenderingData.VertexData.data(), VertexSizeInBytes);
    GPUIndexBufferPtr = ConstructBuffer.ConstructDefaultBuffer(IndexUploadBufferPtr, MeshRenderingData.IndexData.data(), IndexSizeInBytes);
}

FGeometryMap::FGeometryMap()
{
    Geometrys.insert(pair<int, FGeometry>(0, FGeometry()));
}

void FGeometryMap::BuildMesh(CMesh* InMesh, const FMeshRenderingData& MeshData)
{
    FGeometry& Geometry = Geometrys[0];
    Geometry.BuildMesh(InMesh, MeshData);
}


void FGeometryMap::Build()
{
    for(auto &Tmp : Geometrys)
    {
        Tmp.second.Build();
    }
}
