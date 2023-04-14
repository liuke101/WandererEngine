#pragma once
/* */
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"
class FMesh : public IRenderingInterface
{
public:
    virtual void Init();

    virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

    virtual void Draw(float DeltaTime);

    static FMesh *CreateMesh(const FMeshRenderingData * InRenderingData);

protected:
    ComPtr<ID3DBlob> CPUVertexBufferPrt;            // CPU顶点缓冲区
    ComPtr<ID3DBlob> CPUIndexBufferPrt;             // CPU索引缓冲区

    ComPtr<ID3D12Resource> GPUVertexBufferPrt;      // GPU顶点缓冲区，存在默认堆，只允许GPU访问
    ComPtr<ID3D12Resource> GPUIndexBufferPrt;       // GPU索引缓冲区，存在默认堆，只允许GPU访问

    ComPtr<ID3D12Resource> GPUVertexBufferTmpPrt;   // 上传缓冲区，顶点数据从内存复制到该缓冲区，再从该缓冲区复制到GPU顶点缓冲区
    ComPtr<ID3D12Resource> GPUIndexBufferTmpPrt;    // 上传缓冲区
};