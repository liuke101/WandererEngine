#include "ConstantBufferView.h"

void FConstantBufferView::CreateConstant(UINT ObjectSize, UINT ObjectCount)
{
    Constant = make_shared<FRenderingResourcesUpdate>();
    Constant->Init(GetD3dDevice().Get(), ObjectSize, ObjectCount);
}

void FConstantBufferView::BuildCBV(CD3DX12_CPU_DESCRIPTOR_HANDLE InDescriptorHandle, UINT InConstantBufferNum, UINT InHandleOffset)
{
    UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);//获取指定类型描述符堆的句柄增量的大小
    D3D12_GPU_VIRTUAL_ADDRESS BufferAddress = Constant->GetBuffer()->GetGPUVirtualAddress();    // 缓冲区的起始地址(即索引为0的那个常量缓冲区的地址)

    for(int i =0;i<InConstantBufferNum;++i)
    {
        //每一帧都是起始地址
        CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorHandle = InDescriptorHandle;

        D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc;  // 描述常量缓冲区视图
        CBVDesc.BufferLocation = BufferAddress + i * Constant->GetConstantBufferByteSize();
        CBVDesc.SizeInBytes = Constant->GetConstantBufferByteSize();

        InDescriptorHandle.Offset(i + InHandleOffset, DescriptorOffset);   //⚠从0开始偏移
        GetD3dDevice()->CreateConstantBufferView(&CBVDesc, DescriptorHandle); // 创建用于访问资源数据的常量缓冲区视图
    }

}

void FConstantBufferView::Update(int Index, const void* InData)
{
    //Constant->Update(Index, InData);
}
