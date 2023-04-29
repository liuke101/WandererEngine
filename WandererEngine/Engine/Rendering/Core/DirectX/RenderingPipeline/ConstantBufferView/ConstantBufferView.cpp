#include "ConstantBufferView.h"

void FConstantBufferView::CreateConstant(UINT ObjectSize, UINT ObjectCount)
{
    Constant = make_shared<FRenderingResourcesUpdate>();
    Constant->Init(GetD3dDevice().Get(), ObjectSize, ObjectCount);
}

void FConstantBufferView::Update(int Index, const void* InData)
{
    Constant->Update(Index, InData);
}

void FConstantBufferView::BuildCBV(CD3DX12_CPU_DESCRIPTOR_HANDLE InDescriptorHandle, UINT InConstantBufferNum, UINT InHandleOffset)
{
    // 获取指定类型描述符堆的句柄增量的大小
    UINT IncrementSize = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    D3D12_GPU_VIRTUAL_ADDRESS BufferAddress = Constant->GetBuffer()->GetGPUVirtualAddress();    // 缓冲区的起始地址(即索引为0的那个常量缓冲区的地址)

    for(int i = 0; i < InConstantBufferNum; ++i)
    {
        // 每一帧都是起始地址
        CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorHandle = InDescriptorHandle;

        // 描述常量缓冲区视图
        D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc;  
        CBVDesc.BufferLocation = BufferAddress + i * Constant->GetConstantBufferByteSize();
        CBVDesc.SizeInBytes = Constant->GetConstantBufferByteSize();

        // 计算偏移
        DescriptorHandle.Offset(i + InHandleOffset, IncrementSize);   
        // 等价实现：指定要偏移的目标描述符的编号，将它与相邻描述符之间的增量相乘
        // DescriptorHandle.Offset((i + InHandleOffset) * IncrementSize);

        GetD3dDevice()->CreateConstantBufferView(&CBVDesc, DescriptorHandle); // 创建用于访问资源数据的常量缓冲区视图
    }

}


