#include "ConstructBuffer.h"
#include "../../../Debug/EngineDebug.h"


ComPtr<ID3D12Resource> FConstructBuffer::ConstructDefaultBuffer(ComPtr<ID3D12Resource>& UploadBuffer, const void* InData, UINT64 InDataSize)
{
    // 创建默认缓冲区
    ComPtr<ID3D12Resource> DefaultBuffer;
    CD3DX12_RESOURCE_DESC BufferResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(InDataSize);
    CD3DX12_HEAP_PROPERTIES DefaultBufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    ANALYSIS_HRESULT(GetD3dDevice()->CreateCommittedResource(
        &DefaultBufferProperties,
        D3D12_HEAP_FLAG_NONE,
        &BufferResourceDesc,
        D3D12_RESOURCE_STATE_COMMON,
        NULL,
        IID_PPV_ARGS(DefaultBuffer.GetAddressOf())));

    // 创建上传缓冲区，作为中介将CPU端内存中的数据复制到默认缓冲区
    CD3DX12_HEAP_PROPERTIES UploadBufferProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
    ANALYSIS_HRESULT(GetD3dDevice()->CreateCommittedResource(
        &UploadBufferProperties,
        D3D12_HEAP_FLAG_NONE,
        &BufferResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        NULL,
        IID_PPV_ARGS(UploadBuffer.GetAddressOf())));

    // 描述我们希望复制到默认缓冲区中的数据
    D3D12_SUBRESOURCE_DATA SubResourceData = {};
    SubResourceData.pData = InData;
    SubResourceData.RowPitch = InDataSize;
    SubResourceData.SlicePitch = SubResourceData.RowPitch;

    // 资源状态转换为复制目标
    CD3DX12_RESOURCE_BARRIER CopyDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        DefaultBuffer.Get(),
        D3D12_RESOURCE_STATE_COMMON,
        D3D12_RESOURCE_STATE_COPY_DEST);
    GetGraphicsCommandList()->ResourceBarrier(1, &CopyDestBarrier);

    // 将数据从CPU端的内存中复制到位于中介位置的上传堆里
    UpdateSubresources<1>(GetGraphicsCommandList().Get(), DefaultBuffer.Get(), UploadBuffer.Get(), 0, 0, 1, &SubResourceData);

    CD3DX12_RESOURCE_BARRIER ReadDestBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        DefaultBuffer.Get(),
        D3D12_RESOURCE_STATE_COPY_DEST,
        D3D12_RESOURCE_STATE_GENERIC_READ);
    //GetGraphicsCommandList()->ResourceBarrier(1, &ReadDestBarrier);

    // 注意:在调用上述函数后，必须保证UploadBuffer依然存在，而不能对它立即进行销毁。这是因为
    // 命令列表中的复制操作可能尚未执行。待调用者得知复制完成的消息后，方可释放UploadBuffer
    return DefaultBuffer;
}
