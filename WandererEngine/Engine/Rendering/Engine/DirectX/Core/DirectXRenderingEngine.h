#pragma once
#include "../../Core/RenderingEngine.h"
#include "../../../../Core/Viewport/ViewportInfo.h"

class CLightManage;
class CMeshManage;
class CWorld;
class CDirectXRenderingEngine : public CRenderingEngine
{
	friend class IDirectXDeviceInterface;
	friend class CWindowsEngine;
public:
	CDirectXRenderingEngine();
	~CDirectXRenderingEngine();

	virtual int PreInit(FWinMainCommandParameters InParameters);
	virtual int Init(FWinMainCommandParameters InParameters);
	virtual int PostInit();

	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

	virtual void Tick(float DeltaTime);

	virtual int PreExit();
	virtual int Exit();
	virtual int PostExit();

public:
	// 获取当前的Buffer
	ID3D12Resource* GetCurrentSwapBuffer() const;

	// 获取当前Buffer的描述符句柄
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentSwapBufferView() const;

	// 获取当前深度模板缓冲区的描述符句柄
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentDepthStencilView() const;

public:
	// 获取后台缓冲区纹理数据格式
	DXGI_FORMAT GetBackBufferFormat() const { return BackBufferFormat; }
	// 获取深度模板缓冲区纹理数据格式
	DXGI_FORMAT GetDepthStencilFormat() const { return DepthStencilFormat; }
	// 获取MSAA采样数
	UINT GetMSAASampleCount() const;
	// 获取MSAA采样质量
	UINT GetMSAASampleQuality() const;

	// 获取Manage
	CLightManage* GetLightManage() { return LightManage; }
	CMeshManage* GetMeshManage() { return MeshManage; }
	
protected:
	// 使用围栏Fence来刷新命令队列
	void WaitGPUCommandQueueComplete();

	// 初始化Direct3D 
	bool InitDirect3D();
	void PostInitDirect3D();

protected:
	CLightManage* LightManage;
	CMeshManage* MeshManage;
	CWorld* World;

protected:
	UINT64 CurrentFenceIndex;       // 围栏值
	int CurrentSwapBufferIndex;		// 当前Buffer的索引

protected:
	ComPtr<IDXGIFactory4> DXGIFactory;	// 创建DirectX 图形基础结构(DXGI)对象
	ComPtr<ID3D12Device> D3dDevice;     // 显示适配器; 它用于创建命令分配器、命令列表、命令队列、围栏Fence、资源、管道状态对象、堆、根签名、采样器和许多资源视图。
	ComPtr<ID3D12Fence> Fence;		    // 围栏，用于实现 GPU 和 CPU 间的同步

	// 命令对象
	ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList;	// 命令列表：由CPU控制将命令列表中的命令提交到命令队列
	ComPtr<ID3D12CommandAllocator> CommandAllocator;		// 命令分配器：命令列表内的命令存储在命令分配器中 
	ComPtr<ID3D12CommandQueue> CommandQueue;				// 命令队列：存储在GPU上，通过`ExecuteCommandLists`引用分配器里的命令

	// 交换链
	ComPtr<IDXGISwapChain> SwapChain;	// 交换链

	// 描述符堆
	ComPtr<ID3D12DescriptorHeap> RTVHeap;					// RTV描述符堆：渲染目标视图(render target view)
	ComPtr<ID3D12DescriptorHeap> DSVHeap;					// DSV描述符堆：深度模板视图(depth/stencil view)

	// 缓冲区
	std::vector<ComPtr<ID3D12Resource>> SwapChainBuffer;	// 交换链缓冲区
	ComPtr<ID3D12Resource> DepthStencilBuffer;				// 深度/模板缓冲区

	// 视口
	// 后台缓冲区大小为整个窗口，有时只是希望把场景绘制到后台缓冲区的某个矩形子区域中，这个子区域称作视口。坐标系以缓冲区左上角为原点
	D3D12_VIEWPORT ViewportInfo;

	// 裁剪矩形
	D3D12_RECT ViewportRect;

protected:
	UINT MSAA4XQualityLevels;		// 4X MSAA质量级别
	bool bMSAA4XEnable;				// 4X MSAA是否开启
	DXGI_FORMAT BackBufferFormat;   // 后台缓冲区纹理数据格式
	DXGI_FORMAT DepthStencilFormat; // 深度模板缓冲区纹理数据格式
	UINT RTVDescriptorSize;			// RTV描述符所占字节的大小
};