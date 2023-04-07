#include "WindowsEngine.h"
#include "../../Debug/EngineDebug.h"
#include "../../Config/EngineRenderConfig.h"

#if defined(_WIN32)
#include "WindowsMessageProcessing.h"

int FWindowsEngine::PreInit(FWinMainCommandParameters InParameters)
{
	// 日志系统初始化
	const char LogPath[] = "../log";  
	init_log_system(LogPath);
	Engine_Log("Log Init.");

	// 处理命令

	//

	if (InitWindows(InParameters))
	{
		
	}

	Engine_Log("Engine pre-initialization complete.");
	return 0;
}

int FWindowsEngine::Init()
{
	Engine_Log("Engine initialization complete.");
	return 0;
}

int FWindowsEngine::PostInit()
{
	Engine_Log("Engine post-initialization complete.");
	return 0;
}

void FWindowsEngine::Tick()
{

}

int FWindowsEngine::PreExit()
{
	FEngineRenderConfig::Destory();
	Engine_Log("Engine pre-exit complete.");
	return 0;
}

int FWindowsEngine::Exit()
{
	Engine_Log("Engine exit complete.");
	return 0;
}

int FWindowsEngine::PostExit()
{
	
	Engine_Log("Engine post-exit complete.");
	return 0;
}

bool FWindowsEngine::InitWindows(FWinMainCommandParameters InParameters)
{
	// 描述窗口特征
	WNDCLASSEX WindowsClass;							// 窗口类
	WindowsClass.cbSize = sizeof(WNDCLASSEX);			// 窗口类大小（以字节为单位）
	WindowsClass.cbClsExtra = 0;						// 按照窗口类结构分配的额外字节数
	WindowsClass.cbWndExtra = 0;						// 在窗口实例之后分配的额外字节数
	WindowsClass.hbrBackground = NULL;					// 绘制背景
	WindowsClass.hCursor = LoadCursor(NULL, IDC_ARROW); // 设置一个箭头光标
	WindowsClass.hIcon = NULL;							// 应用程序放在磁盘上显示的图标
	WindowsClass.hIconSm = NULL;						// 应用程序显示在左上角的图标
	WindowsClass.hInstance = InParameters.HInstance;	// 窗口实例
	WindowsClass.lpszClassName = L"Wanderer Engine";	// 窗口名称
	WindowsClass.lpszMenuName = NULL;					// 菜单：无默认菜单
	WindowsClass.style = CS_VREDRAW | CS_HREDRAW;		// 窗口样式：宽度或高度发生改变时重绘窗口
	WindowsClass.lpfnWndProc = EngineWindowProc;		// 消息处理函数

	// 注册窗口实例
	ATOM RegisterAtom = RegisterClassEx(&WindowsClass); //注册窗口类
	if (!RegisterAtom)
	{
		Engine_Log_Error("Register WindowsClass failed.");
		MessageBox(0, L"Register WindowsClass failed", L"Error", MB_OK); // 消息框
		return false;
	}

	// 定义矩形左上角和右下角的坐标
	// {左上角 x 坐标, 左上角 y 坐标, 右下角 x 坐标, 右下角 y 坐标}
	RECT Rect = { 0,0,FEngineRenderConfig::GetRenderConfig()->ScreenWidth,FEngineRenderConfig::GetRenderConfig()->ScreenHeight };
	// 调整窗口矩形（矩形坐标，窗口样式，是否有菜单）
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, NULL);

	// 计算窗口宽高
	int WindowWidth = Rect.right - Rect.left;
	int WindowHight = Rect.bottom - Rect.top;
	
	// 创建窗口句柄
	MainWindowsHandle = CreateWindowEx(
		NULL,					// 窗口的扩展窗口样式
		L"Wanderer Engine",		// 窗口名称
		L"Wanderer Engine",		// 会显示在窗口的标题栏上去
		WS_OVERLAPPEDWINDOW,	// 窗口的样式。
		100,					// 窗口的初始水平位置。
		100,					// 窗口的初始垂直位置。
		WindowWidth,			// 窗口的宽度
		WindowHight,			// 窗口的高度
		NULL,					// 窗口的父窗口或所有者窗口的句柄
		NULL,					// 菜单的句柄，或指定子窗口标识符
		InParameters.HInstance, // 窗口实例的句柄
		NULL					// 一个指向用户定义数据的指针
	);
	
	if (!MainWindowsHandle)
	{
		Engine_Log_Error("CreateWindow failed.");
		MessageBox(0, L"CreateWindow failed.", 0, 0);
		return false;
	}

	// 显示窗口
	ShowWindow(MainWindowsHandle, SW_SHOW);  // 疑问：这里第二项应该使用WinMain函数的InshowCmd参数

	// 刷新窗口
	UpdateWindow(MainWindowsHandle);

	Engine_Log("InitWindows complete.");
}

bool FWindowsEngine::InitDirect3D()
{
	// 1. 检测创建DXGI对象是否成功
	ANALYSIS_HRESULT(CreateDXGIFactory1(IID_PPV_ARGS(&DXGIFactory))); //IID_PPV_ARGS宏用于获取COM ID(GUID)
	/*———————————————————————————————————————————————————————————————————————————————————————————————————————*/
	
	// 2. 创建DX12设备（显示适配器），通常使用的设备为硬件适配器（独立显卡）
	HRESULT D3dDeviceResult = D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice));
	if (FAILED(D3dDeviceResult))
	{
		// 若创建失败，使用软件适配器WARP（Windows高级光栅化平台)
		ComPtr<IDXGIAdapter> WARPAdapter;
		ANALYSIS_HRESULT(DXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&WARPAdapter)));
		ANALYSIS_HRESULT(D3D12CreateDevice(WARPAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice)));
	}
	/*———————————————————————————————————————————————————————————————————————————————————————————————————————*/
	
	// 3. 创建围栏Fence
	/*
	渲染流程：
		Fence->SetEventOnCompletion
		执行命令
		提交呈现
		Queue->Signal
		wait
	*/
	ANALYSIS_HRESULT(D3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));
	/*———————————————————————————————————————————————————————————————————————————————————————————————————————*/
	
	// 4. 创建命令队列、命令分配器、命令列表
	D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
	QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;	// 存储的是一系列可供 GPU 直接执行的命令
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;	// 指示默认命令队列
	ANALYSIS_HRESULT(D3dDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue)));

	ANALYSIS_HRESULT(D3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(CommandAllocator.GetAddressOf()))); // GetAddressof(): 此方法可利用函数参数返回 COM接口的指针。

	ANALYSIS_HRESULT(D3dDevice->CreateCommandList(
		0,	//单 GPU 操作
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		CommandAllocator.Get(),	// 将命令列表关联到命令分配器 //Get():此方法常用于把原始的 COM 接口指针作为参数传递给函数。
		nullptr, //nullptr如果是，则运行时设置虚拟初始管道状态， 开销较低
		IID_PPV_ARGS(GraphicsCommandList.GetAddressOf())));
	/*———————————————————————————————————————————————————————————————————————————————————————————————————————*/
	
	// 5. 描述并创建交换链
	SwapChain.Reset(); // 释放之前创建的交换链，随后进行重建
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	SwapChainDesc.BufferDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;					// 缓冲区分辨率宽度
	SwapChainDesc.BufferDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;					// 缓冲区分辨率高度
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = FEngineRenderConfig::GetRenderConfig()->RefreshRate;	// 刷新率分子
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;													// 刷新率分母
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;						// 扫描线命令，标识如何在表面上绘制扫描线的值。
	SwapChainDesc.BufferCount = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;					    // 缓冲区数量，默认为2即采用双缓冲
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;											// 将数据渲染到渲染目标（Render Target）
	SwapChainDesc.OutputWindow = MainWindowsHandle;															// 渲染窗口的句柄
	SwapChainDesc.Windowed = true;																			// ture：窗口模式 false：全屏模式
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;												// 缓冲区交换时，丢弃后台缓冲区的内容
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;											// 使能够通过调用 ResizeTarget 来切换窗口/全屏模式

	ANALYSIS_HRESULT(DXGIFactory->CreateSwapChain(CommandQueue.Get(), &SwapChainDesc, SwapChain.GetAddressOf()));




	return false;
}


#endif

