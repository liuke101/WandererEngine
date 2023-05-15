#include "DirectXDeviceInterface.h"
#include "../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../Platform/Windows/WindowsEngine.h"

ComPtr<ID3D12Fence> IDirectXDeviceInterface::GetFence()
{
    if (CWindowsEngine* InEngine = GetEngine())
    {
        if (InEngine->GetRenderingEngine())
        {
            return InEngine->GetRenderingEngine()->Fence;
        }
    }
    return nullptr;
}

ComPtr<ID3D12Device> IDirectXDeviceInterface::GetD3dDevice()
{
    if (CWindowsEngine* InEngine = GetEngine())
    {
        if (InEngine->GetRenderingEngine())
        {
            return InEngine->GetRenderingEngine()->D3dDevice;
        }
    }
    return nullptr;
}

CLightManage* IDirectXDeviceInterface::GetLightManage()
{
    if (CWindowsEngine* InEngine = GetEngine())
    {
        if (InEngine->GetRenderingEngine())
        {
            return InEngine->GetRenderingEngine()->GetLightManage();
        }
    }

    return NULL;
}

CMeshManage* IDirectXDeviceInterface::GetMeshManage()
{
    return GetEngine()->GetMeshManage();
}

CWorld* IDirectXDeviceInterface::GetWorld()
{
    if (CWindowsEngine* InEngine = GetEngine())
    {
        return InEngine->GetWorld();
    }

    return NULL;
}

ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterface::GetGraphicsCommandList()
{
    if (CWindowsEngine* InEngine = GetEngine())
    {
        if (InEngine->GetRenderingEngine())
        {
            return InEngine->GetRenderingEngine()->GraphicsCommandList;
        }
    }
    return nullptr;
}

ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterface::GetCommandAllocator()
{
    if (CWindowsEngine* InEngine = GetEngine())
    {
        if (InEngine->GetRenderingEngine())
        {
            return InEngine->GetRenderingEngine()->CommandAllocator;
        }
    }
    return nullptr;
}

ComPtr<ID3D12CommandQueue> IDirectXDeviceInterface::GetCommandQueue()
{
    if(CWindowsEngine* InEngine = GetEngine())
    {
        if(InEngine->GetRenderingEngine())
        {
            return InEngine->GetRenderingEngine()->CommandQueue;
        }
    }
    return nullptr;
}

UINT64 IDirectXDeviceInterface::GetCurrentFenceIndex()
{
    if (CWindowsEngine* InEngine = GetEngine())
    {
        if (InEngine->GetRenderingEngine())
        {
            return InEngine->GetRenderingEngine()->CurrentFenceIndex;
        }
    }
    return UINT64();
}

HWND IDirectXDeviceInterface::GetMainWindowsHandle()
{
    if (CWindowsEngine* InEngine = GetEngine())
    {
        if (InEngine->GetRenderingEngine())
        {
            //return InEngine->GetRenderingEngine()->MainWindowsHandle;
            return InEngine->MainWindowsHandle; //有什么区别？
        }
    }
    return HWND();
}

#if defined(_WIN32)
CWindowsEngine* IDirectXDeviceInterface::GetEngine()
{
    return dynamic_cast<CWindowsEngine*>(Engine);
}
#else
CEngine* IDirectXDeviceInterface::GetEngine()
{
    return Engine;
}
#endif

ComPtr<ID3D12Fence> IDirectXDeviceInterface_Struct::GetFence()
{
    return DirectXDeviceInterface.GetFence();
}

ComPtr<ID3D12Device> IDirectXDeviceInterface_Struct::GetD3dDevice()
{
    return DirectXDeviceInterface.GetD3dDevice();
}

CLightManage* IDirectXDeviceInterface_Struct::GetLightManage()
{
    return DirectXDeviceInterface.GetLightManage();
}

CMeshManage* IDirectXDeviceInterface_Struct::GetMeshManage()
{
    return DirectXDeviceInterface.GetMeshManage();
}

CWorld* IDirectXDeviceInterface_Struct::GetWorld()
{
    return DirectXDeviceInterface.GetWorld();
}

ComPtr<ID3D12GraphicsCommandList> IDirectXDeviceInterface_Struct::GetGraphicsCommandList()
{
    return DirectXDeviceInterface.GetGraphicsCommandList();
}

ComPtr<ID3D12CommandAllocator> IDirectXDeviceInterface_Struct::GetCommandAllocator()
{
    return DirectXDeviceInterface.GetCommandAllocator();
}

ComPtr<ID3D12CommandQueue> IDirectXDeviceInterface_Struct::GetCommandQueue()
{
    return DirectXDeviceInterface.GetCommandQueue();
}

UINT64 IDirectXDeviceInterface_Struct::GetCurrentFenceIndex()
{
    return DirectXDeviceInterface.GetCurrentFenceIndex();
}

HWND IDirectXDeviceInterface_Struct::GetMainWindowsHandle()
{
    return DirectXDeviceInterface.GetMainWindowsHandle();
}

#if defined(_WIN32)
CWindowsEngine* IDirectXDeviceInterface_Struct::GetEngine()
{
    return DirectXDeviceInterface.GetEngine();
}
#else
CEngine* IDirectXDeviceInterfece_Struct::GetEngine()
{
    return Interfece.GetEngine();
}
#endif