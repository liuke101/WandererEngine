#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"

/* 描述符堆 */
struct FDescriptorHeap : public IDirectXDeviceInterface_Struct
{
public:
    void Build(UINT InNumDescriptor);

    void PreDraw(float DeltaTime);

    ID3D12DescriptorHeap* GetHeap() const { return CBVHeap.Get(); }

protected:
    // 视图(view)和描述符(descriptor)是一个概念
    // CBV = constant buffer view = constant buffer descriptor
    ComPtr<ID3D12DescriptorHeap> CBVHeap;   // CBV描述符堆
};