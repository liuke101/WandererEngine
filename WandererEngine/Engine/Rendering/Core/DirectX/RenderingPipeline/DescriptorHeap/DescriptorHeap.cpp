#include "DescriptorHeap.h"

void FDescriptorHeap::Build(UINT InNumDescriptor)
{
    // 创建CBV描述符堆
    D3D12_DESCRIPTOR_HEAP_DESC CBVHeapDesc; 
    CBVHeapDesc.NumDescriptors = InNumDescriptor;  // ⚠描述符数量
    CBVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    CBVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    CBVHeapDesc.NodeMask = 0;
    GetD3dDevice()->CreateDescriptorHeap(&CBVHeapDesc, IID_PPV_ARGS(&CBVHeap));
}

void FDescriptorHeap::PreDraw(float DeltaTime)
{
    ID3D12DescriptorHeap* DescriptorHeap[] = { CBVHeap.Get() };
    GetGraphicsCommandList()->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap);
}
