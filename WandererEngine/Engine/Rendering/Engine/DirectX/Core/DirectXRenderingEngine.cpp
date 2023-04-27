#include "DirectXRenderingEngine.h"

#include "../../../../Debug/EngineDebug.h"
#include "../../../../Config/EngineRenderConfig.h"
#include "../../../../Rendering/Core/Rendering.h"
#include "../../../../Mesh/BoxMesh.h"
#include "../../../../Mesh/SphereMesh.h"
#include "../../../../Mesh/CylinderMesh.h"
#include "../../../../Mesh/ConeMesh.h"
#include "../../../../Mesh/PlaneMesh.h"
#include "../../../../Mesh/CustomMesh.h"
#include "../../../../Core/CoreObject/CoreMinimalObject.h"
#include "../../../../Core/World.h"
#include "../../../../Mesh/Core/MeshManage.h"

#if defined(_WIN32)
#include "../../../../Core/WinMainCommandParameters.h"

//class FVector
//{
//	unsigned char r;//255 ->[0,1]
//	unsigned char g;//255
//	unsigned char b;//255
//	unsigned char a;//255
//};

CDirectXRenderingEngine::CDirectXRenderingEngine()
	: CurrentFenceIndex(0)
	, CurrentSwapBufferIndex(0)
	, MSAA4XQualityLevels(0)
	, bMSAA4XEnable(false)
	, BackBufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM) //UNORM： 表示归一化处理的无符号整数，范围[0,1]
	, DepthStencilFormat(DXGI_FORMAT_D24_UNORM_S8_UINT)
{
	// 创建交换链缓冲区
	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChainBuffer.push_back(ComPtr<ID3D12Resource>());
	}

	//关闭Tick
	bTick = false;

	MeshManage = new CMeshManage();
}

CDirectXRenderingEngine::~CDirectXRenderingEngine()
{
	delete MeshManage;
}

int CDirectXRenderingEngine::PreInit(FWinMainCommandParameters InParameters)
{
	Engine_Log("DirectXRenderingEngine pre-initialization complete.");
	return 0;
}

int CDirectXRenderingEngine::Init(FWinMainCommandParameters InParameters)
{
	InitDirect3D();

	PostInitDirect3D();

	MeshManage->Init();

	Engine_Log("DirectXRenderingEngine initialization complete.");

	return 0;
}

int CDirectXRenderingEngine::PostInit()
{
	

	ANALYSIS_HRESULT(GraphicsCommandList->Reset(CommandAllocator.Get(), NULL));

	{
		//构建Mesh
		//MeshManage->CreateBoxMesh(2.0f, 2.0f, 2.0f);
		//MeshManage->CreateSphereMesh(2.f, 20, 20);
		//MeshManage->CreatePlaneMesh(4.f, 3.f, 20, 20);
		//MeshManage->CreateConeMesh(1.f, 5.f, 20, 20);
		string MeshObjPath = "../WandererEngine/Monkey.obj";  // 路径为对应exe程序的相对位置
		MeshManage->CreateMesh(MeshObjPath);
		
	}

	ANALYSIS_HRESULT(GraphicsCommandList->Close());
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	WaitGPUCommandQueueComplete();

	Engine_Log("DirectXRenderingEngine post-initialization complete.");
	return 0;
}

void CDirectXRenderingEngine::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	MeshManage->UpdateCalculations(DeltaTime, ViewportInfo);
}

void CDirectXRenderingEngine::Tick(float DeltaTime)
{
	// 重置命令分配器，为下一帧做准备
	ANALYSIS_HRESULT(CommandAllocator->Reset());

	MeshManage->PreDraw(DeltaTime);

	// 转换状态
	CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(
		GetCurrentSwapBuffer(),
		D3D12_RESOURCE_STATE_PRESENT,			//和D3D12_RESOURCE_STATE_COMMON同义
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	GraphicsCommandList->ResourceBarrier(1, &ResourceBarrierPresent);

	// 需要每帧执行
	// 绑定视口/裁剪矩形
	GraphicsCommandList->RSSetViewports(1, &ViewportInfo);
	GraphicsCommandList->RSSetScissorRects(1, &ViewportRect);

	// 清除RenderTarget
	GraphicsCommandList->ClearRenderTargetView(
		GetCurrentSwapBufferView(),		// CPU 描述符句柄,表示要清除的RT的堆的开始
		DirectX::Colors::Black,			// 填充RT的颜色
		0,								// 指定的结构数组中的矩形数
		nullptr);						// 要清除的资源视图中矩形 的D3D12_RECT 结构数组。如果为NULL，将清除整个资源视图

	// 清除深度/模板缓冲区
	GraphicsCommandList->ClearDepthStencilView(
		GetCurrentDepthStencilView(),						// CPU 描述符句柄,表示要清除的深度模板的堆的开始
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,	// 要清除的数据类型
		1.0f,												// 用于清除深度缓冲区的值。此值将限制在 0 和 1 之间。
		0,													// 清除模板缓冲区的值。
		0,													// 指定的结构数组中的矩形数
		NULL);												// 要清除的资源视图中矩形 的D3D12_RECT 结构数组。如果为NULL，将清除整个资源视图

	/*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/

	// 输出的合并阶段
	// 设置RT和深度模板的 CPU 描述符句柄
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentSwapBufferView = GetCurrentSwapBufferView();
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentDepthStencilView = GetCurrentDepthStencilView();
	GraphicsCommandList->OMSetRenderTargets(1, &CurrentSwapBufferView, true, &CurrentDepthStencilView);

	MeshManage->Draw(DeltaTime);
	MeshManage->PostDraw(DeltaTime);

	// 资源转换
	CD3DX12_RESOURCE_BARRIER ResourceBarrierPresentRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
		GetCurrentSwapBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);

	GraphicsCommandList->ResourceBarrier(1, &ResourceBarrierPresentRenderTarget);

	// 关闭命令列表
	ANALYSIS_HRESULT(GraphicsCommandList->Close());

	/*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/

	// 命令提交
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	// 交换两个Buffer缓冲区
	ANALYSIS_HRESULT(SwapChain->Present(0, 0));								// 交换两个Buffer缓冲区
	CurrentSwapBufferIndex = !(bool)CurrentSwapBufferIndex;

	//CPU等GPU
	WaitGPUCommandQueueComplete();
}

int CDirectXRenderingEngine::PreExit()
{
	
	Engine_Log("DirectXRenderingEngine pre-exit complete.");
	return 0;
}

int CDirectXRenderingEngine::Exit()
{

	Engine_Log("DirectXRenderingEngine exit complete.");
	return 0;
}

int CDirectXRenderingEngine::PostExit()
{

	Engine_Log("DirectXRenderingEngine post-exit complete.");
	return 0;
}

ID3D12Resource* CDirectXRenderingEngine::GetCurrentSwapBuffer() const
{
	return SwapChainBuffer[CurrentSwapBufferIndex].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE CDirectXRenderingEngine::GetCurrentSwapBufferView() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		RTVHeap->GetCPUDescriptorHandleForHeapStart(),	//获取表示堆开始的 CPU 描述符句柄
		CurrentSwapBufferIndex,
		RTVDescriptorSize);
}

D3D12_CPU_DESCRIPTOR_HANDLE CDirectXRenderingEngine::GetCurrentDepthStencilView() const
{
	return DSVHeap->GetCPUDescriptorHandleForHeapStart();
}

UINT CDirectXRenderingEngine::GetMSAASampleCount() const
{
	return bMSAA4XEnable ? 4 : 1; ;
}

UINT CDirectXRenderingEngine::GetMSAASampleQuality() const
{
	return bMSAA4XEnable ? (MSAA4XQualityLevels - 1) : 0;
}

void CDirectXRenderingEngine::WaitGPUCommandQueueComplete()
{
	// 增加围栏值，接下来将命令标记到此围栏点
	CurrentFenceIndex++;

	// 向命令队列中添加一条用来设置新围栏点的命令
	// 由于这条命令要交由GPU处理(即由GPU端来修改围栏值)，所以在GPU处理完命令队列中此signal()的所有命令之前,它并不会设置新的围栏点
	ANALYSIS_HRESULT(CommandQueue->Signal(Fence.Get(), CurrentFenceIndex));

	//在CPU端等待GPU，直到后者执行完这个围栏点之前的所有命令
	if (Fence->GetCompletedValue() < CurrentFenceIndex)
	{
		// 创建或打开时间对象，并返回句柄，
		HANDLE EventEX = CreateEventEx(NULL, NULL, 0, EVENT_ALL_ACCESS);

		// 若GPU命中当前的矩形（即执行Siganl()命令，修改了围栏值），则激发预定事件
		ANALYSIS_HRESULT(Fence->SetEventOnCompletion(CurrentFenceIndex, EventEX));

		// 等待GPU命中围栏，激发事件
		WaitForSingleObject(EventEX, INFINITE);
		CloseHandle(EventEX);
	}

}

bool CDirectXRenderingEngine::InitDirect3D()
{
	// 开启DX12的调试层
	ComPtr<ID3D12Debug> D3D12Debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&D3D12Debug))))
	{
		D3D12Debug->EnableDebugLayer();
	}

	/*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/

	// 1. 创建DXGI对象
	ANALYSIS_HRESULT(CreateDXGIFactory1(IID_PPV_ARGS(&DXGIFactory))); //IID_PPV_ARGS宏用于获取COM ID(GUID)

	/*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/

	// 2. 创建DX12设备（显示适配器），通常使用的设备为硬件适配器（独立显卡）
	HRESULT D3dDeviceResult = D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice));
	if (FAILED(D3dDeviceResult))
	{
		// 若创建失败，使用软件适配器WARP（Windows高级光栅化平台)
		ComPtr<IDXGIAdapter> WARPAdapter;
		ANALYSIS_HRESULT(DXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&WARPAdapter)));
		ANALYSIS_HRESULT(D3D12CreateDevice(WARPAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice)));
	}

	/*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/

	// 3. 创建围栏Fence
	// 实现刷新命令队列功能：强制CPU等待，直到GPU完成所有命令的处理，达到某个指定的围栏点为止（效率不高的同步方法）
	// 除了用于CPU与GPU的同步之外，还可以在重置命令分配器之前，先刷新命令队列来确定GPU的命令已执行完毕。
	/*
	渲染流程：
		Fence->SetEventOnCompletion
		执行命令
		提交呈现
		Queue->Signal
		wait
	*/
	ANALYSIS_HRESULT(D3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));

	/*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/

	// 4. 创建命令队列、命令分配器、命令列表
	D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
	QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;	// 存储的是一系列可供 GPU 直接执行的命令
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;	// 指示默认命令队列
	ANALYSIS_HRESULT(D3dDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue)));

	ANALYSIS_HRESULT(D3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(CommandAllocator.GetAddressOf()))); // GetAddressof(): 此方法可利用函数参数返回 COM接口的指针

	ANALYSIS_HRESULT(D3dDevice->CreateCommandList(
		0,									// 单 GPU 操作
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		CommandAllocator.Get(),				// 将命令列表关联到命令分配器	//Get():此方法常用于把原始的 COM 接口指针作为参数传递给函数
		nullptr,							// 如果是nullptr，则运行时设置虚拟初始管道状态， 开销较低
		IID_PPV_ARGS(GraphicsCommandList.GetAddressOf())));

	// 首先要将命令列表置于关闭状态，因为第一次引用命令列表时，我们要对它进行重置，在调用重置方法之前又需先将他关闭
	ANALYSIS_HRESULT(GraphicsCommandList->Close());

	/*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/

	// 5. 检测对4X MSAA质量级别的支持
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS QualityLevels;
	QualityLevels.Format = BackBufferFormat;							// 纹理数据格式
	QualityLevels.SampleCount = 4;										// 采样次数
	QualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;	// 用于控制质量级别的标志
	QualityLevels.NumQualityLevels = 0;									// 质量级别数

	ANALYSIS_HRESULT(D3dDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &QualityLevels, sizeof(QualityLevels)));
	MSAA4XQualityLevels = QualityLevels.NumQualityLevels;				// 4X MSAA质量级别

	/*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/

	// 6. 描述并创建交换链
	SwapChain.Reset(); // 释放之前创建的交换链，随后进行重建  
	//Reset():将此 ComPtr 实例设置为 nullptr 释放与之相关的所有引用（同时减少其底层 COM 接口的引用计数)。此方法的功能与将 ComPtr 目标实例赋值为 nullptr 的效果相同。

	DXGI_SWAP_CHAIN_DESC SwapChainDesc;																	 // 交换链描述
	SwapChainDesc.BufferDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;				 // 缓冲区分辨率宽度
	SwapChainDesc.BufferDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;				 // 缓冲区分辨率高度
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = FEngineRenderConfig::GetRenderConfig()->RefreshRate;// 刷新率分子
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;												 // 刷新率分母
	SwapChainDesc.BufferDesc.Format = BackBufferFormat;													 // 纹理数据格式
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;					 // 扫描线命令，标识如何在表面上绘制扫描线的值。

	SwapChainDesc.SampleDesc.Count = GetMSAASampleCount();												 // MSAA采样数量
	SwapChainDesc.SampleDesc.Quality = GetMSAASampleQuality();											 // MSAA采样质量级别

	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;										 // 将数据渲染到渲染目标（Render Target）
	SwapChainDesc.BufferCount = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;					 // 缓冲区数量，默认为2即采用双缓冲
	SwapChainDesc.OutputWindow = MainWindowsHandle;														 // 渲染窗口的句柄
	SwapChainDesc.Windowed = true;																		 // ture：窗口模式 false：全屏模式
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;											 // 缓冲区交换时，丢弃后台缓冲区的内容
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;										 // 使能够通过调用 ResizeTarget 来切换窗口/全屏模式

	ANALYSIS_HRESULT(DXGIFactory->CreateSwapChain(CommandQueue.Get(), &SwapChainDesc, SwapChain.GetAddressOf()));  //创建交换链

	/*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/

	// 7. 创建描述符堆
	// RTV堆
	D3D12_DESCRIPTOR_HEAP_DESC RTVDescriptorHeapDesc;												// 描述符堆描述
	RTVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;									// 堆中描述符的类型
	RTVDescriptorHeapDesc.NumDescriptors = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;	// 堆中的描述符数量，RTV数量等应该等于交换链缓冲区数量
	RTVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;									// 指示堆的默认用法
	RTVDescriptorHeapDesc.NodeMask = 0;																// 对于单适配器操作，设置为零
	ANALYSIS_HRESULT(D3dDevice->CreateDescriptorHeap(&RTVDescriptorHeapDesc, IID_PPV_ARGS(RTVHeap.GetAddressOf())));
	// DSV堆
	D3D12_DESCRIPTOR_HEAP_DESC DSVDescriptorHeapDesc;
	DSVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DSVDescriptorHeapDesc.NumDescriptors = 1;		// 堆中的描述符数量，DSA数量为1
	DSVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DSVDescriptorHeapDesc.NodeMask = 0;
	ANALYSIS_HRESULT(D3dDevice->CreateDescriptorHeap(&DSVDescriptorHeapDesc, IID_PPV_ARGS(DSVHeap.GetAddressOf())));

	/*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/


	return false;
}

void CDirectXRenderingEngine::PostInitDirect3D()
{
	// CPU等GPU
	WaitGPUCommandQueueComplete();

	// 初始化命令列表
	ANALYSIS_HRESULT(GraphicsCommandList->Reset(CommandAllocator.Get(), NULL));

	/*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/

	// 初始化交换链缓冲区
	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChainBuffer[i].Reset();
	}
	DepthStencilBuffer.Reset();

	// 更改交换链的后台缓冲区大小、格式和缓冲区数量。 应在调整应用程序窗口大小时调用此函数。
	SwapChain->ResizeBuffers(
		FEngineRenderConfig::GetRenderConfig()->SwapChainCount,
		FEngineRenderConfig::GetRenderConfig()->ScreenWidth,
		FEngineRenderConfig::GetRenderConfig()->ScreenHeight,
		BackBufferFormat,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	/*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/

	// 创建RTV
	RTVDescriptorSize = D3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV); // 获取RTV描述符大小
	CD3DX12_CPU_DESCRIPTOR_HANDLE HeapHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());		 // 获取描述符堆中第一个描述符的句柄
	// 描述符大小作为偏移量，使用偏移量找到当前后台缓冲区的RTV
	for (UINT i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChain->GetBuffer(i, IID_PPV_ARGS(&SwapChainBuffer[i]));						  // 获得交换链中的缓冲区资源
		D3dDevice->CreateRenderTargetView(SwapChainBuffer[i].Get(), nullptr, HeapHandle); // 为获取到的缓冲区创建RTV
		HeapHandle.Offset(1, RTVDescriptorSize);											  // 偏移到描述符堆中的下一个缓冲区
	}

	/*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/

	// 创建深度/模板缓冲区及其视图
	// 创建深度/模板缓冲区
	D3D12_RESOURCE_DESC ResourceDesc;   											// 资源描述
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;					// 资源的维度（可选：缓冲区、1D纹理、2D纹理、3D纹理）
	ResourceDesc.Alignment = 0;														// 对齐方式
	ResourceDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;		// 资源的宽度（对纹理来说是以纹素为单位的宽度，对缓冲区来说是占用的字节数）
	ResourceDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;		// 资源的高度
	ResourceDesc.DepthOrArraySize = 1;												// 资源的深度（如果为 3D）或数组大小（如果是 1D 或 2D 资源的数组）
	ResourceDesc.MipLevels = 1;														// mipmap 级别
	ResourceDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;								// 资源数据格式
	ResourceDesc.SampleDesc.Count = GetMSAASampleCount();							// MSAA采样数
	ResourceDesc.SampleDesc.Quality = GetMSAASampleQuality();						// MSAA质量级别
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;								// 纹理布局
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;					// 允许为资源创建深度/模板视图


	D3D12_CLEAR_VALUE ClearValue;			// 用于清除资源的优化值。
	ClearValue.DepthStencil.Depth = 1.f;	// 用于清除深度缓冲区
	ClearValue.DepthStencil.Stencil = 0;	// 用于清除模板缓冲区
	ClearValue.Format = DepthStencilFormat;	// 清除颜色的数据格式必须与清除操作期间使用的视图的格式匹配

	CD3DX12_HEAP_PROPERTIES Properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3dDevice->CreateCommittedResource(						// 创建一个资源和一个堆，并把该资源提交到堆中				
		&Properties,										// 默认堆（性能最佳），只允许GPU访问，性能最佳
		D3D12_HEAP_FLAG_NONE,								// 指定堆选项，例如堆是否可以包含纹理，以及资源是否在适配器之间共享。
		&ResourceDesc,										// 资源描述
		D3D12_RESOURCE_STATE_COMMON,						// 资源的初始状态
		&ClearValue,										// 用于清除资源的优化值
		IID_PPV_ARGS(DepthStencilBuffer.GetAddressOf())		// 资源接口的COM ID
	);

	D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc;					// 描述可从深度模具视图访问的纹理的子资源。
	DSVDesc.Format = DepthStencilFormat;					// 资源数据格式
	DSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;	// 访问深度模板视图中的资源的方式：资源将作为 2D 纹理进行访问
	DSVDesc.Flags = D3D12_DSV_FLAG_NONE;					// 指定纹理是否为只读：None
	DSVDesc.Texture2D.MipSlice = 0;							// Texture2D要使用的第一个 mipmap 级别

	// 创建深度/模板视图
	D3dDevice->CreateDepthStencilView(DepthStencilBuffer.Get(), &DSVDesc, DSVHeap->GetCPUDescriptorHandleForHeapStart());

	/*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/

	// 资源状态转换
	CD3DX12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(	// 描述不同用法之间子资源转换的结构
		DepthStencilBuffer.Get(),			// 转换中使用的资源
		D3D12_RESOURCE_STATE_COMMON,		// 资源的“之前”用法
		D3D12_RESOURCE_STATE_DEPTH_WRITE);	// 资源的“后”用法
	GraphicsCommandList->ResourceBarrier(1, &Barrier); // 设置转换资源屏障数组，只是加入命令列表，真正执行需要传入命令队列
	GraphicsCommandList->Close();					   // 命令加入命令列表之后，提交命令列表之前必须结束命令的记录

	/*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/

	// 命令提交
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);  //将命令列表里的命令添加到命令队列中  //_countof()返回数组中的元素数

	/*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/

	// 设置视口
	// 描述视口尺寸
	ViewportInfo.TopLeftX = 0;		// 左上角的 x 坐标
	ViewportInfo.TopLeftY = 0;		// 左上角的 y 坐标
	ViewportInfo.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	ViewportInfo.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	ViewportInfo.MinDepth = 0.0f;	//将深度值从区间[0,1]转换到区间[MinDepth,MaxDepth], 实现某些特殊效果
	ViewportInfo.MaxDepth = 1.0f;

	// 设置裁剪矩形
	ViewportRect.left = 0;														// 左上角的 x 坐标
	ViewportRect.top = 0;														// 左上角的 y 坐标
	ViewportRect.right = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;	// 右下角的 x 坐标
	ViewportRect.bottom = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;	// 右下角的 y 坐标


	/*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/
	WaitGPUCommandQueueComplete();

}


#endif

