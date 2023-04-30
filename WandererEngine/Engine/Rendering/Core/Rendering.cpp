#include "Rendering.h"
#include "../../Debug/EngineDebug.h"
#include "../../Platform/Windows/WindowsEngine.h"

vector<IRenderingInterface*> IRenderingInterface::RenderingInterface;

IRenderingInterface::IRenderingInterface()
{
    RenderingInterface.push_back(this);
}

IRenderingInterface::~IRenderingInterface()
{
    for(vector<IRenderingInterface*>::const_iterator Iter = RenderingInterface.begin();Iter!=RenderingInterface.end();++Iter)
    {
        if(*Iter == this)
        {
            RenderingInterface.erase(Iter);
            break;
        }
    }
}

void IRenderingInterface::Init()
{
}

void IRenderingInterface::PreDraw(float DeltaTime)
{
    // 重置命令列表
    ANALYSIS_HRESULT(GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), NULL));
}

void IRenderingInterface::Draw(float DeltaTime)
{

}

void IRenderingInterface::PostDraw(float DeltaTime)
{
}

ComPtr<ID3D12Resource> IRenderingInterface::ConstructDefaultBuffer(ComPtr<ID3D12Resource>& UploadBuffer, const void* InData, UINT64 InDataSize)
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

ComPtr<ID3D12Device> IRenderingInterface::GetD3dDevice()
{
     if (CWindowsEngine* InEngine = GetEngine())
     {
         return InEngine->D3dDevice;
     }

     return nullptr;
}

ComPtr<ID3D12GraphicsCommandList> IRenderingInterface::GetGraphicsCommandList()
{
    if(CWindowsEngine* InEngine = GetEngine())
    {
        return InEngine->GraphicsCommandList;
    }

    return nullptr;
}

ComPtr<ID3D12CommandAllocator> IRenderingInterface::GetCommandAllocator()
{
    if (CWindowsEngine* InEngine = GetEngine())
    {
        return InEngine->CommandAllocator;
    }

    return nullptr;
}

#if defined(_WIN32)
CWindowsEngine* IRenderingInterface::GetEngine()
{
    return dynamic_cast<CWindowsEngine*>(Engine);
}
#else
CEngine* IRenderingInterface::GetEngine()
{
    return Engine;
}
#endif

FRenderingResourcesUpdate::FRenderingResourcesUpdate()
{

}

FRenderingResourcesUpdate::~FRenderingResourcesUpdate()
{
    if(UploadBuffer!=nullptr)
    {
        UploadBuffer->Unmap(0, NULL); // 在释放映射内存之前先取消映射：使指向资源中指定子资源的 CPU 指针失效
        UploadBuffer = nullptr;       // 释放内存
    }
}

void FRenderingResourcesUpdate::Init(ID3D12Device* InDevice, UINT InElementSize, UINT InElementCount)
{
    // 创建常量缓冲区
    assert(InDevice);
    ElementSize = InElementSize;
    CD3DX12_HEAP_PROPERTIES HeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);    // 常量缓冲区创建在上传堆，能通过CPU更新常量
    CD3DX12_RESOURCE_DESC ResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(ElementSize * InElementCount);
    ANALYSIS_HRESULT(InDevice->CreateCommittedResource(     // 创建一个资源和一个堆，并把该资源提交到堆中
        &HeapProperties,
        D3D12_HEAP_FLAG_NONE,
        &ResourceDesc,
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&UploadBuffer)));

    ANALYSIS_HRESULT(UploadBuffer->Map(     // 获得指向欲更新资源数据的指针，为更新常量缓冲区做准备
        0,                                  // 子资源的索引，对于缓冲区来说，自身就是唯一的子资源，故设置为0
        nullptr,                            // 内存的映射范围，空指针表示对整个资源进行映射
        reinterpret_cast<void**>(&Data)));  // 双重指针，返回待映射资源数据的目标内存块
}

void FRenderingResourcesUpdate::Update(int Index, const void* InData)
{
    // 将数据从系统内存复制到常量缓冲区
    memcpy(&Data[Index*ElementSize],InData,ElementSize);
}

UINT FRenderingResourcesUpdate::GetConstantBufferByteSize(UINT InTypeSize)
{
    // 龙书p196
    // 常量缓冲区的大小必须是硬件最小分配空间(通常是256B）的整数倍
    // 为此，要将其凑整为满足需求的最小的256的整数倍。我们现在通过输入值bytesize加上255,
    // 再屏蔽求和结果的低2字节(即计算结果中小于256的数据部分）来实现这一点
    // 例如:假设bytesize = 300
    // (300 + 255) & ~255
    // 555 &~255
    // 0x022B &~0x00ff
    // 0x022B &0xff00
    // 0x0200
    // 512
    return (InTypeSize + 255) & ~255;
}

UINT FRenderingResourcesUpdate::GetConstantBufferByteSize()
{
    return GetConstantBufferByteSize(ElementSize);
}

