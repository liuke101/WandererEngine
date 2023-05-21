#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"

struct FConstantBufferView : public IDirectXDeviceInterface_Struct
{
    // 创建常量缓冲区
    void CreateConstant(UINT ObjectSize,UINT ObjectCount,bool bConstanBuffe = true);

    void Update(int Index, const void* InData);

    // 构建CBV
    void BuildCBV(
        CD3DX12_CPU_DESCRIPTOR_HANDLE InDescriptorHandle, 
        UINT InConstantBufferNum, 
        UINT InHandleOffset = 0);

    ID3D12Resource* GetUploadBuffer() { return Constant->GetUploadBuffer(); }
protected:
    shared_ptr<FRenderingResourcesUpdate> Constant;	 
};
