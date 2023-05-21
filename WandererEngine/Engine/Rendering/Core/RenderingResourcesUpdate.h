#pragma once
#include "../../EngineMinimal.h"

// 用于创建常量缓冲区
class FRenderingResourcesUpdate : public enable_shared_from_this<FRenderingResourcesUpdate>
{
public:
    FRenderingResourcesUpdate();
    ~FRenderingResourcesUpdate();

    // 初始化常量缓冲区 
    void Init(ID3D12Device* InDevice, UINT InElementSize, UINT InElementCount, bool bConstanBuffe = true);

    // 更新常量缓冲区
    void Update(int Index, const void* InData);

    // 通过指定元素类型大小来计算常量缓冲区大小（256B的整数倍）
    UINT GetConstantBufferByteSize(UINT InTypeSize);
    // 根据已知元素大小来计算常量缓冲区大小（256B的整数倍）
    UINT GetConstantBufferByteSize();

    ID3D12Resource* GetUploadBuffer() { return UploadBuffer.Get(); }
private:
    ComPtr<ID3D12Resource> UploadBuffer;    // 上传堆
    UINT ElementSize;                       // 元素大小
    BYTE* Data;
};

