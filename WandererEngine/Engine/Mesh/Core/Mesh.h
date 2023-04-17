#pragma once
/* Mesh基类 */
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"

struct FObjectTransformation
{
    FObjectTransformation();
    XMFLOAT4X4 World;
};

class FMesh : public IRenderingInterface
{
public:
    FMesh();

    virtual void Init();

    virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

    virtual void Draw(float DeltaTime);

    static FMesh* CreateMesh(const FMeshRenderingData * InRenderingData);

    
    D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView(); // 创建顶点缓冲区视图，与RTV不同，无需创建描述符堆
    D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();   // 创建索引缓冲区视图
protected:
    ComPtr<ID3DBlob> CPUVertexBufferPtr;            // CPU顶点缓冲区（存储在系统内存中）
    ComPtr<ID3DBlob> CPUIndexBufferPtr;             // CPU索引缓冲区（存储在系统内存中）

    ComPtr<ID3D12Resource> GPUVertexBufferPtr;      // GPU顶点缓冲区，存在默认堆，只允许GPU访问
    ComPtr<ID3D12Resource> GPUIndexBufferPtr;       // GPU索引缓冲区，存在默认堆，只允许GPU访问

    ComPtr<ID3D12Resource> VertexUploadBufferPtr;   // 上传缓冲区，顶点数据从内存复制到该缓冲区，再从该缓冲区复制到GPU顶点缓冲区
    ComPtr<ID3D12Resource> IndexUploadBufferPtr;    // 上传缓冲区

    ComPtr<ID3D12DescriptorHeap> CBVHeap;           // CBV描述符堆：常量缓冲区视图（Constant Buffer View）
    shared_ptr<FRenderingResourceUpdate> objectConstants;   // 对象常量

    UINT VertexSizeInBytes;                         // 顶点缓冲区视图，视图的顶点缓冲区大小（字节)                                         
    UINT VertexStrideInBytes;                       // 顶点缓冲区视图，每个顶点元素所占的字节数
    UINT IndexSizeInBytes;                          // 索引缓冲区视图，视图的顶点缓冲区大小（字节)
    DXGI_FORMAT IndexFormat;                                    // 索引缓冲区视图，索引格式
};