#include "RenderingResourcesUpdate.h"
FRenderingResourcesUpdate::FRenderingResourcesUpdate()
{

}

FRenderingResourcesUpdate::~FRenderingResourcesUpdate()
{
    if (UploadBuffer != nullptr)
    {
        UploadBuffer->Unmap(0, NULL); // 在释放映射内存之前先取消映射：使指向资源中指定子资源的 CPU 指针失效
        UploadBuffer = nullptr;       // 释放内存
    }
}

void FRenderingResourcesUpdate::Init(ID3D12Device* InDevice, UINT InElementSize, UINT InElementCount)
{
    // 创建常量缓冲区
    assert(InDevice);
    ElementSize = GetConstantBufferByteSize(InElementSize);

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
    memcpy(&Data[Index * ElementSize], InData, ElementSize);
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

