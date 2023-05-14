#pragma once
#include "../Core/Engine.h"

#if defined(_WIN32)
class CMeshManage;
class CWorld;
class CWindowsEngine;
#else
class CEngine;
#endif

// 为类提供渲染内容的接口
class IDirectXDeviceInterface
{
public:
    ComPtr<ID3D12Fence> GetFence();
    ComPtr<ID3D12Device> GetD3dDevice();
    CMeshManage* GetMeshManage();
    CWorld* GetWorld();

    ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList();
    ComPtr<ID3D12CommandAllocator> GetCommandAllocator();
    ComPtr<ID3D12CommandQueue> GetCommandQueue();

    UINT64 GetCurrentFenceIndex();
    HWND GetMainWindowsHandle();


#if defined(_WIN32)
    CWindowsEngine* GetEngine();
#else
    CEngine* GetEngine();
#endif
};

// 为结构体提供渲染内容的接口
struct IDirectXDeviceInterface_Struct
{
public:
    ComPtr<ID3D12Fence> GetFence();
    ComPtr<ID3D12Device> GetD3dDevice();
    CMeshManage* GetMeshManage();
    CWorld* GetWorld();

    ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList();
    ComPtr<ID3D12CommandAllocator> GetCommandAllocator();
    ComPtr<ID3D12CommandQueue> GetCommandQueue();

    UINT64 GetCurrentFenceIndex();
    HWND GetMainWindowsHandle();


#if defined(_WIN32)
    CWindowsEngine* GetEngine();
#else
    CEngine* GetEngine();
#endif

private:
    IDirectXDeviceInterface DirectXDeviceInterface;
};