﻿#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"

class FRenderingResourcesUpdate;

struct FConstantBufferView : public IDirectXDeviceInterface_Struct
{
    // 创建常量缓冲区
    void CreateConstant(UINT ObjectSize,UINT ObjectCount);

    // 构建CBV
    void BuildCBV(CD3DX12_CPU_DESCRIPTOR_HANDLE InDescriptorHandle, UINT InConstantBufferNum, UINT InHandleOffset = 0);

    void Update(int Index, const void* InData);
protected:
    shared_ptr<FRenderingResourcesUpdate> Constant;	 
};
