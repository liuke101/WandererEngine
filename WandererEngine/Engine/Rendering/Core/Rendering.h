﻿#pragma once
/* 渲染接口 */
// 需要渲染的地方继承该接口即可
#include "../../Core/CoreObject/GuidInterface.h"
#include "../../Core/Engine.h"

#if defined(_WIN32)
#include "../../Platform/Windows/WindowsEngine.h"
#else
#endif

class IRenderingInterface:public IGuidInterface
{
    friend class CWindowsEngine;
public:
    IRenderingInterface();
    virtual ~IRenderingInterface();

    virtual void Init();

    virtual void PreDraw(float DeltaTime);  
    virtual void Draw(float DeltaTime);
    virtual void PostDraw(float DeltaTime);

protected:
    // 构建默认缓冲区
    ComPtr<ID3D12Resource> ConstructDefaultBuffer(ComPtr<ID3D12Resource> &UploadBuffer, const void* InData,UINT64 InDataSize);
protected:
#if defined(_WIN32)
    CWindowsEngine* GetEngine();
#else
    CEngine* GetEngine();
#endif

    ComPtr<ID3D12Device> GetD3dDevice();
    ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList();
    ComPtr<ID3D12CommandAllocator> GetCommandAllocator();

private:
    static vector<IRenderingInterface*> RenderingInterface;
};

class FRenderingResourcesUpdate : public enable_shared_from_this<FRenderingResourcesUpdate>
{
public:
    FRenderingResourcesUpdate();
    ~FRenderingResourcesUpdate();

    // 初始化常量缓冲区 
    void Init(ID3D12Device* InDevice, UINT InElementSize, UINT InElementCount);

    // 更新常量缓冲区
    void Update(int Index, const void* InData);

    // 通过指定元素类型大小来计算常量缓冲区大小（256B的整数倍）
    UINT GetConstantBufferByteSize(UINT InTypeSize);
    // 根据已知元素大小来计算常量缓冲区大小（256B的整数倍）
    UINT GetConstantBufferByteSize();

    ID3D12Resource* GetBuffer() { return UploadBuffer.Get(); }
private:
    ComPtr<ID3D12Resource> UploadBuffer;    // 上传堆
    UINT ElementSize;                       // 元素大小
    BYTE* Data;
};

