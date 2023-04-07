#pragma once
/* Windows平台引擎 */
#if defined(_WIN32)
#include "../../Core/Engine.h"

// Windows平台引擎类
class FWindowsEngine :public FEngine
{
public:
	virtual int PreInit(FWinMainCommandParameters InParameters);
	virtual int Init();
	virtual int PostInit();

	virtual void Tick();

	virtual int PreExit();
	virtual int Exit();
	virtual int PostExit();

private:
	// 初始化Windows窗口 
	bool InitWindows(FWinMainCommandParameters InParameters);
	// 初始化Direct3D 
	bool InitDirect3D();

protected:
	ComPtr<IDXGIFactory4> DXGIFactory; // 创建DirectX 图形基础结构(DXGI)对象
	ComPtr<ID3D12Device> D3dDevice;    // 显示适配器; 它用于创建命令分配器、命令列表、命令队列、围栏Fence、资源、管道状态对象、堆、根签名、采样器和许多资源视图。
	ComPtr<ID3D12Fence> Fence;		   // 围栏，用于实现 GPU 和 CPU 间的同步

	ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList;	// 命令列表，由CPU控制将命令列表中的命令提交到命令队列
	ComPtr<ID3D12CommandAllocator> CommandAllocator;		// 命令分配器：命令列表内的命令存储在命令分配器中 ，命令队列通过`ExecuteCommandLists`引用分配器里的命令
	ComPtr<ID3D12CommandQueue> CommandQueue;				// 命令队列，存储在GPU上
	
	ComPtr<IDXGISwapChain> SwapChain;

protected:
	HWND MainWindowsHandle;	// 主窗口句柄
};

#endif