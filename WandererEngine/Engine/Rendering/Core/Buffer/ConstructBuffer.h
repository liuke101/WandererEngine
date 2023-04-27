#pragma once
#include "../../../Core/Engine.h"
#include "../../../Interface/DirectXDeviceInterface.h"

namespace ConstructBuffer
{
    struct FConstructBuffer : public IDirectXDeviceInterface_Struct
    {
        // 构建默认缓冲区
        ComPtr<ID3D12Resource> ConstructDefaultBuffer(ComPtr<ID3D12Resource>& UploadBuffer, const void* InData, UINT64 InDataSize);
    };
}

    