#include "WindowsEngine.h"
#include "../../Debug/EngineDebug.h"
#include "../../Config/EngineRenderConfig.h"

#if defined(_WIN32)
#include "WindowsMessageProcessing.h"

int FWindowsEngine::PreInit(FWinMainCommandParameters InParameters)
{
	// ��־ϵͳ��ʼ��
	const char LogPath[] = "../log";  
	init_log_system(LogPath);
	Engine_Log("Log Init.");

	// ��������

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
	// ������������
	WNDCLASSEX WindowsClass;							// ������
	WindowsClass.cbSize = sizeof(WNDCLASSEX);			// �������С�����ֽ�Ϊ��λ��
	WindowsClass.cbClsExtra = 0;						// ���մ�����ṹ����Ķ����ֽ���
	WindowsClass.cbWndExtra = 0;						// �ڴ���ʵ��֮�����Ķ����ֽ���
	WindowsClass.hbrBackground = NULL;					// ���Ʊ���
	WindowsClass.hCursor = LoadCursor(NULL, IDC_ARROW); // ����һ����ͷ���
	WindowsClass.hIcon = NULL;							// Ӧ�ó�����ڴ�������ʾ��ͼ��
	WindowsClass.hIconSm = NULL;						// Ӧ�ó�����ʾ�����Ͻǵ�ͼ��
	WindowsClass.hInstance = InParameters.HInstance;	// ����ʵ��
	WindowsClass.lpszClassName = L"Wanderer Engine";	// ��������
	WindowsClass.lpszMenuName = NULL;					// �˵�����Ĭ�ϲ˵�
	WindowsClass.style = CS_VREDRAW | CS_HREDRAW;		// ������ʽ����Ȼ�߶ȷ����ı�ʱ�ػ洰��
	WindowsClass.lpfnWndProc = EngineWindowProc;		// ��Ϣ������

	// ע�ᴰ��ʵ��
	ATOM RegisterAtom = RegisterClassEx(&WindowsClass); //ע�ᴰ����
	if (!RegisterAtom)
	{
		Engine_Log_Error("Register WindowsClass failed.");
		MessageBox(0, L"Register WindowsClass failed", L"Error", MB_OK); // ��Ϣ��
		return false;
	}

	// ����������ϽǺ����½ǵ�����
	// {���Ͻ� x ����, ���Ͻ� y ����, ���½� x ����, ���½� y ����}
	RECT Rect = { 0,0,FEngineRenderConfig::GetRenderConfig()->ScreenWidth,FEngineRenderConfig::GetRenderConfig()->ScreenHeight };
	// �������ھ��Σ��������꣬������ʽ���Ƿ��в˵���
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, NULL);

	// ���㴰�ڿ��
	int WindowWidth = Rect.right - Rect.left;
	int WindowHight = Rect.bottom - Rect.top;
	
	// �������ھ��
	MainWindowsHandle = CreateWindowEx(
		NULL,					// ���ڵ���չ������ʽ
		L"Wanderer Engine",		// ��������
		L"Wanderer Engine",		// ����ʾ�ڴ��ڵı�������ȥ
		WS_OVERLAPPEDWINDOW,	// ���ڵ���ʽ��
		100,					// ���ڵĳ�ʼˮƽλ�á�
		100,					// ���ڵĳ�ʼ��ֱλ�á�
		WindowWidth,			// ���ڵĿ��
		WindowHight,			// ���ڵĸ߶�
		NULL,					// ���ڵĸ����ڻ������ߴ��ڵľ��
		NULL,					// �˵��ľ������ָ���Ӵ��ڱ�ʶ��
		InParameters.HInstance, // ����ʵ���ľ��
		NULL					// һ��ָ���û��������ݵ�ָ��
	);
	
	if (!MainWindowsHandle)
	{
		Engine_Log_Error("CreateWindow failed.");
		MessageBox(0, L"CreateWindow failed.", 0, 0);
		return false;
	}

	// ��ʾ����
	ShowWindow(MainWindowsHandle, SW_SHOW);  // ���ʣ�����ڶ���Ӧ��ʹ��WinMain������InshowCmd����

	// ˢ�´���
	UpdateWindow(MainWindowsHandle);

	Engine_Log("InitWindows complete.");
}

bool FWindowsEngine::InitDirect3D()
{
	// 1. ��ⴴ��DXGI�����Ƿ�ɹ�
	ANALYSIS_HRESULT(CreateDXGIFactory1(IID_PPV_ARGS(&DXGIFactory))); //IID_PPV_ARGS�����ڻ�ȡCOM ID(GUID)
	/*��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/
	
	// 2. ����DX12�豸����ʾ����������ͨ��ʹ�õ��豸ΪӲ���������������Կ���
	HRESULT D3dDeviceResult = D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice));
	if (FAILED(D3dDeviceResult))
	{
		// ������ʧ�ܣ�ʹ�����������WARP��Windows�߼���դ��ƽ̨)
		ComPtr<IDXGIAdapter> WARPAdapter;
		ANALYSIS_HRESULT(DXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&WARPAdapter)));
		ANALYSIS_HRESULT(D3D12CreateDevice(WARPAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice)));
	}
	/*��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/
	
	// 3. ����Χ��Fence
	/*
	��Ⱦ���̣�
		Fence->SetEventOnCompletion
		ִ������
		�ύ����
		Queue->Signal
		wait
	*/
	ANALYSIS_HRESULT(D3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));
	/*��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/
	
	// 4. ����������С�����������������б�
	D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
	QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;	// �洢����һϵ�пɹ� GPU ֱ��ִ�е�����
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;	// ָʾĬ���������
	ANALYSIS_HRESULT(D3dDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue)));

	ANALYSIS_HRESULT(D3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(CommandAllocator.GetAddressOf()))); // GetAddressof(): �˷��������ú����������� COM�ӿڵ�ָ�롣

	ANALYSIS_HRESULT(D3dDevice->CreateCommandList(
		0,	//�� GPU ����
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		CommandAllocator.Get(),	// �������б��������������� //Get():�˷��������ڰ�ԭʼ�� COM �ӿ�ָ����Ϊ�������ݸ�������
		nullptr, //nullptr����ǣ�������ʱ���������ʼ�ܵ�״̬�� �����ϵ�
		IID_PPV_ARGS(GraphicsCommandList.GetAddressOf())));
	/*��������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/
	
	// 5. ����������������
	SwapChain.Reset(); // �ͷ�֮ǰ�����Ľ��������������ؽ�
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	SwapChainDesc.BufferDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;					// �������ֱ��ʿ��
	SwapChainDesc.BufferDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;					// �������ֱ��ʸ߶�
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = FEngineRenderConfig::GetRenderConfig()->RefreshRate;	// ˢ���ʷ���
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;													// ˢ���ʷ�ĸ
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;						// ɨ���������ʶ����ڱ����ϻ���ɨ���ߵ�ֵ��
	SwapChainDesc.BufferCount = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;					    // ������������Ĭ��Ϊ2������˫����
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;											// ��������Ⱦ����ȾĿ�꣨Render Target��
	SwapChainDesc.OutputWindow = MainWindowsHandle;															// ��Ⱦ���ڵľ��
	SwapChainDesc.Windowed = true;																			// ture������ģʽ false��ȫ��ģʽ
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;												// ����������ʱ��������̨������������
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;											// ʹ�ܹ�ͨ������ ResizeTarget ���л�����/ȫ��ģʽ

	ANALYSIS_HRESULT(DXGIFactory->CreateSwapChain(CommandQueue.Get(), &SwapChainDesc, SwapChain.GetAddressOf()));




	return false;
}


#endif

