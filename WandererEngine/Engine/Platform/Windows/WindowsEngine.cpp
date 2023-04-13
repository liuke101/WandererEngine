#include "WindowsEngine.h"
#include "../../Debug/EngineDebug.h"
#include "../../Config/EngineRenderConfig.h"

#if defined(_WIN32)
#include "WindowsMessageProcessing.h"

FWindowsEngine::FWindowsEngine()
	: MSAA4XQualityLevels(0)
	, bMSAA4XEnable(false)
	, BackBufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM) //UNORM�� ��ʾ��һ��������޷�����������Χ[0,1]
	, DepthStencilFormat(DXGI_FORMAT_D24_UNORM_S8_UINT)
	, CurrentSwapBufferIndex(0)
{
	// ����������������
	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChainBuffer.push_back(ComPtr<ID3D12Resource>());
	}
}

int FWindowsEngine::PreInit(FWinMainCommandParameters InParameters)
{
	// ��־ϵͳ��ʼ��
	const char LogPath[] = "../log";  
	init_log_system(LogPath);
	Engine_Log("Log Init.");

	// ��������

	//

	Engine_Log("Engine pre-initialization complete.");
	return 0;
}

int FWindowsEngine::Init(FWinMainCommandParameters InParameters)
{

	if (InitWindows(InParameters))
	{

	}

	if (InitDirect3D())
	{

	}

	Engine_Log("Engine initialization complete.");
	return 0;
}

int FWindowsEngine::PostInit()
{
	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/
	
	// ��ʼ��������������
	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChainBuffer[i].Reset();
	}
	DepthStencilBuffer.Reset();

	// ���Ľ������ĺ�̨��������С����ʽ�ͻ����������� Ӧ�ڵ���Ӧ�ó��򴰿ڴ�Сʱ���ô˺�����
	SwapChain->ResizeBuffers(
		FEngineRenderConfig::GetRenderConfig()->SwapChainCount,
		FEngineRenderConfig::GetRenderConfig()->ScreenWidth,
		FEngineRenderConfig::GetRenderConfig()->ScreenHeight,
		BackBufferFormat,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// ����RTV
	RTVDescriptorSize = D3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV); // ��ȡRTV��������С
	CD3DX12_CPU_DESCRIPTOR_HANDLE HeapHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());		 // ��ȡ���������е�һ���������ľ��
	// ��������С��Ϊƫ������ʹ��ƫ�����ҵ���ǰ��̨��������RTV
	for (UINT i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChain->GetBuffer(i, IID_PPV_ARGS(&SwapChainBuffer[i]));						  // ��ý������еĻ�������Դ
		D3dDevice->CreateRenderTargetView(SwapChainBuffer[i].Get(), nullptr, HeapHandle); // Ϊ��ȡ���Ļ���������RTV
		HeapHandle.Offset(1,RTVDescriptorSize);											  // ƫ�Ƶ����������е���һ��������
	}

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/
	
	// �������/ģ�建����������ͼ
	// �������/ģ�建����
	D3D12_RESOURCE_DESC ResourceDesc;   											// ��Դ����
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;					// ��Դ��ά�ȣ���ѡ����������1D����2D����3D����
	ResourceDesc.Alignment = 0;														// ���뷽ʽ
	ResourceDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;		// ��Դ�Ŀ�ȣ���������˵��������Ϊ��λ�Ŀ�ȣ��Ի�������˵��ռ�õ��ֽ�����
	ResourceDesc.Height= FEngineRenderConfig::GetRenderConfig()->ScreenHeight;		// ��Դ�ĸ߶�
	ResourceDesc.DepthOrArraySize = 1;												// ��Դ����ȣ����Ϊ 3D���������С������� 1D �� 2D ��Դ�����飩
	ResourceDesc.MipLevels = 1;														// mipmap ����
	ResourceDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;								// ��Դ���ݸ�ʽ
	ResourceDesc.SampleDesc.Count = bMSAA4XEnable ? 4 : 1;							// MSAA������
	ResourceDesc.SampleDesc.Quality = bMSAA4XEnable ? (MSAA4XQualityLevels - 1) : 0;// MSAA��������
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;								// ������
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;					// ����Ϊ��Դ�������/ģ����ͼ
	

	D3D12_CLEAR_VALUE ClearValue;			// ���������Դ���Ż�ֵ��
	ClearValue.DepthStencil.Depth = 1.f;	// ���������Ȼ�����
	ClearValue.DepthStencil.Stencil = 0;	// �������ģ�建����
	ClearValue.Format = DepthStencilFormat;	// �����ɫ�����ݸ�ʽ��������������ڼ�ʹ�õ���ͼ�ĸ�ʽƥ��

	CD3DX12_HEAP_PROPERTIES Properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3dDevice->CreateCommittedResource(						// ����һ����Դ��һ���ѣ����Ѹ���Դ�ύ������				
		&Properties,	// Ĭ�϶ѣ�������ѣ���ֻ����GPU���ʣ��������
		D3D12_HEAP_FLAG_NONE,								// ָ����ѡ�������Ƿ���԰��������Լ���Դ�Ƿ���������֮�乲��
		&ResourceDesc,										// ��Դ����
		D3D12_RESOURCE_STATE_COMMON,						// ��Դ�ĳ�ʼ״̬
		&ClearValue,										// ���������Դ���Ż�ֵ
		IID_PPV_ARGS(DepthStencilBuffer.GetAddressOf())		// ��Դ�ӿڵ�COM ID
	); 

	D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc;					// �����ɴ����ģ����ͼ���ʵ����������Դ��
	DSVDesc.Format = DepthStencilFormat;					// ��Դ���ݸ�ʽ
	DSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;	// �������ģ����ͼ�е���Դ�ķ�ʽ����Դ����Ϊ 2D ������з���
	DSVDesc.Flags = D3D12_DSV_FLAG_NONE;					// ָ�������Ƿ�Ϊֻ����None
	DSVDesc.Texture2D.MipSlice = 0;							// Texture2DҪʹ�õĵ�һ�� mipmap ����
	
	// �������/ģ����ͼ
	D3dDevice->CreateDepthStencilView(DepthStencilBuffer.Get(), &DSVDesc, DSVHeap->GetCPUDescriptorHandleForHeapStart());

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// ��Դ״̬ת��
	CD3DX12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(	// ������ͬ�÷�֮������Դת���Ľṹ
		DepthStencilBuffer.Get(),			// ת����ʹ�õ���Դ
		D3D12_RESOURCE_STATE_COMMON,		// ��Դ�ġ�֮ǰ���÷�
		D3D12_RESOURCE_STATE_DEPTH_WRITE);	// ��Դ�ġ����÷�
	GraphicsCommandList->ResourceBarrier(1,	&Barrier); // ����ת����Դ�������飬ֻ�Ǽ��������б�����ִ����Ҫ�����������
	GraphicsCommandList->Close();					   // ������������б�֮���ύ�����б�֮ǰ�����������ļ�¼

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// �����ύ
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);  //�������б����������ӵ����������  //_countof()���������е�Ԫ����
	
	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// �����ӿ�
	// �����ӿڳߴ�
	ViewportInfo.TopLeftX = 0;		// ���Ͻǵ� x ����
	ViewportInfo.TopLeftY = 0;		// ���Ͻǵ� y ����
	ViewportInfo.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	ViewportInfo.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	ViewportInfo.MinDepth = 0.0f;	//�����ֵ������[0,1]ת��������[MinDepth,MaxDepth], ʵ��ĳЩ����Ч��
	ViewportInfo.MaxDepth = 1.0f;

	// ���òü�����
	ViewportRect.left = 0;														// ���Ͻǵ� x ����
	ViewportRect.top = 0;														// ���Ͻǵ� y ����
	ViewportRect.right = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;	// ���½ǵ� x ����
	ViewportRect.bottom = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;	// ���½ǵ� y ����


	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/
	Engine_Log("Engine post-initialization complete.");
	return 0;
}

void FWindowsEngine::Tick(float DeltaTime)
{
	// ���������������Ϊ��һ֡��׼��
	ANALYSIS_HRESULT(CommandAllocator->Reset());

	// ���������б�
	GraphicsCommandList->Reset(CommandAllocator.Get(), NULL);

	// ת��״̬
	CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(	
		GetCurrentSwapBuffer(),
		D3D12_RESOURCE_STATE_PRESENT,			//��D3D12_RESOURCE_STATE_COMMONͬ��
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	GraphicsCommandList->ResourceBarrier(1, &ResourceBarrierPresent);
		
	// ��Ҫÿִ֡��
	// ���ӿ�/�ü�����
	GraphicsCommandList->RSSetViewports(1, &ViewportInfo);
	GraphicsCommandList->RSSetScissorRects(1, &ViewportRect);

	// ���RenderTarget
	GraphicsCommandList->ClearRenderTargetView(
		GetCurrentSwapBufferView(),		// CPU ���������,��ʾҪ�����RT�ĶѵĿ�ʼ
		DirectX::Colors::Red,			// ���RT����ɫ
		0,								// ָ���Ľṹ�����еľ�����
		nullptr);						// Ҫ�������Դ��ͼ�о��� ��D3D12_RECT �ṹ���顣���ΪNULL�������������Դ��ͼ

	// ������/ģ�建����
	GraphicsCommandList->ClearDepthStencilView(
		GetCurrentDepthStencilView(),						// CPU ���������,��ʾҪ��������ģ��ĶѵĿ�ʼ
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,	// Ҫ�������������
		1.0f,												// ���������Ȼ�������ֵ����ֵ�������� 0 �� 1 ֮�䡣
		0,													// ���ģ�建������ֵ��
		0,													// ָ���Ľṹ�����еľ�����
		NULL);												// Ҫ�������Դ��ͼ�о��� ��D3D12_RECT �ṹ���顣���ΪNULL�������������Դ��ͼ

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// ����ĺϲ��׶�
	// ����RT�����ģ��� CPU ���������
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentSwapBufferView = GetCurrentSwapBufferView();
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentDepthStencilView = GetCurrentDepthStencilView();
	GraphicsCommandList->OMSetRenderTargets(1, &CurrentSwapBufferView,true,&CurrentDepthStencilView);	
	
	// ��Դת��
	CD3DX12_RESOURCE_BARRIER ResourceBarrierPresentRenderTarget= CD3DX12_RESOURCE_BARRIER::Transition(
		GetCurrentSwapBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);

	GraphicsCommandList->ResourceBarrier(1, &ResourceBarrierPresentRenderTarget);

	ANALYSIS_HRESULT(GraphicsCommandList->Close());							// �ر����������
			
	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// �����ύ
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };		
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	// ��������Buffer������
	ANALYSIS_HRESULT(SwapChain->Present(0, 0));								// ��������Buffer������
	CurrentSwapBufferIndex = !(bool)CurrentSwapBufferIndex;

	//CPU��GPU
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

ID3D12Resource* FWindowsEngine::GetCurrentSwapBuffer() const
{
	return SwapChainBuffer[CurrentSwapBufferIndex].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE FWindowsEngine::GetCurrentSwapBufferView() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		RTVHeap->GetCPUDescriptorHandleForHeapStart(),	//��ȡ��ʾ�ѿ�ʼ�� CPU ���������
		CurrentSwapBufferIndex,
		RTVDescriptorSize);
}

D3D12_CPU_DESCRIPTOR_HANDLE FWindowsEngine::GetCurrentDepthStencilView() const
{
	return DSVHeap->GetCPUDescriptorHandleForHeapStart();
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
	// ����DX12�ĵ��Բ�
	ComPtr<ID3D12Debug> D3D12Debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&D3D12Debug))))
	{
		D3D12Debug->EnableDebugLayer();
	}

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// 1. ����DXGI����
	ANALYSIS_HRESULT(CreateDXGIFactory1(IID_PPV_ARGS(&DXGIFactory))); //IID_PPV_ARGS�����ڻ�ȡCOM ID(GUID)

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/
	
	// 2. ����DX12�豸����ʾ����������ͨ��ʹ�õ��豸ΪӲ���������������Կ���
	HRESULT D3dDeviceResult = D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice));
	if (FAILED(D3dDeviceResult))
	{
		// ������ʧ�ܣ�ʹ�����������WARP��Windows�߼���դ��ƽ̨)
		ComPtr<IDXGIAdapter> WARPAdapter;
		ANALYSIS_HRESULT(DXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&WARPAdapter)));
		ANALYSIS_HRESULT(D3D12CreateDevice(WARPAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice)));
	}

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/
	
	// 3. ����Χ��Fence
	// ʵ��ˢ��������й��ܣ�ǿ��CPU�ȴ���ֱ��GPU�����������Ĵ����ﵽĳ��ָ����Χ����Ϊֹ��Ч�ʲ��ߵ�ͬ��������
	/*
	��Ⱦ���̣�
		Fence->SetEventOnCompletion
		ִ������
		�ύ����
		Queue->Signal
		wait
	*/
	ANALYSIS_HRESULT(D3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// 4. ����������С�����������������б�
	D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
	QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;	// �洢����һϵ�пɹ� GPU ֱ��ִ�е�����
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;	// ָʾĬ���������
	ANALYSIS_HRESULT(D3dDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue)));

	ANALYSIS_HRESULT(D3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(CommandAllocator.GetAddressOf()))); // GetAddressof(): �˷��������ú����������� COM�ӿڵ�ָ��

	HRESULT CMLResult = D3dDevice->CreateCommandList(
		0,									// �� GPU ����
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		CommandAllocator.Get(),				// �������б���������������	//Get():�˷��������ڰ�ԭʼ�� COM �ӿ�ָ����Ϊ�������ݸ�����
		nullptr,							// �����nullptr��������ʱ���������ʼ�ܵ�״̬�� �����ϵ�
		IID_PPV_ARGS(GraphicsCommandList.GetAddressOf()));

	if (FAILED(CMLResult))
	{
		Engine_Log_Error("Error = %i", (int)CMLResult); 
	}
	// ����Ҫ�������б����ڹر�״̬����Ϊ��һ�����������б�ʱ������Ҫ�����������ã��ڵ������÷���֮ǰ�����Ƚ����ر�
	GraphicsCommandList->Close();

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// 5. ����4X MSAA���������֧��
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS QualityLevels;
	QualityLevels.Format = BackBufferFormat;							// �������ݸ�ʽ
	QualityLevels.SampleCount = 4;										// ��������
	QualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;	// ���ڿ�����������ı�־
	QualityLevels.NumQualityLevels = 0;									// ����������

	ANALYSIS_HRESULT(D3dDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &QualityLevels, sizeof(QualityLevels)));
	MSAA4XQualityLevels = QualityLevels.NumQualityLevels;				// 4X MSAA��������

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// 6. ����������������
	SwapChain.Reset(); // �ͷ�֮ǰ�����Ľ��������������ؽ�  
	//Reset():���� ComPtr ʵ������Ϊ nullptr �ͷ���֮��ص��������ã�ͬʱ������ײ� COM �ӿڵ����ü���)���˷����Ĺ����뽫 ComPtr Ŀ��ʵ����ֵΪ nullptr ��Ч����ͬ��

	DXGI_SWAP_CHAIN_DESC SwapChainDesc;																	 // ����������
	SwapChainDesc.BufferDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;				 // �������ֱ��ʿ��
	SwapChainDesc.BufferDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;				 // �������ֱ��ʸ߶�
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = FEngineRenderConfig::GetRenderConfig()->RefreshRate;// ˢ���ʷ���
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;												 // ˢ���ʷ�ĸ
	SwapChainDesc.BufferDesc.Format = BackBufferFormat;													 // �������ݸ�ʽ
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;					 // ɨ���������ʶ����ڱ����ϻ���ɨ���ߵ�ֵ��

	SwapChainDesc.SampleDesc.Count = bMSAA4XEnable ? 4 : 1;												 // MSAA��������
	SwapChainDesc.SampleDesc.Quality = bMSAA4XEnable ? (MSAA4XQualityLevels - 1) : 0;					 // MSAA������������

	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;										 // ��������Ⱦ����ȾĿ�꣨Render Target��
	SwapChainDesc.BufferCount = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;					 // ������������Ĭ��Ϊ2������˫����
	SwapChainDesc.OutputWindow = MainWindowsHandle;														 // ��Ⱦ���ڵľ��
	SwapChainDesc.Windowed = true;																		 // ture������ģʽ false��ȫ��ģʽ
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;											 // ����������ʱ��������̨������������
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;										 // ʹ�ܹ�ͨ������ ResizeTarget ���л�����/ȫ��ģʽ
	
	
	HRESULT CSCResult = DXGIFactory->CreateSwapChain(CommandQueue.Get(), &SwapChainDesc, SwapChain.GetAddressOf());
	if (FAILED(CSCResult))
	{
		Engine_Log_Error("Error = %i", (int)CSCResult); 
	}

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// 7. ������������
	// RTV��
	D3D12_DESCRIPTOR_HEAP_DESC RTVDescriptorHeapDesc;												// ������������
	RTVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;									// ����������������
	RTVDescriptorHeapDesc.NumDescriptors = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;	// ���е�������������RTV������Ӧ�õ��ڽ���������������
	RTVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;									// ָʾ�ѵ�Ĭ���÷�
	RTVDescriptorHeapDesc.NodeMask = 0;																// ���ڵ�����������������Ϊ��
	ANALYSIS_HRESULT(D3dDevice->CreateDescriptorHeap(&RTVDescriptorHeapDesc, IID_PPV_ARGS(RTVHeap.GetAddressOf())));
	// DSV��
	D3D12_DESCRIPTOR_HEAP_DESC DSVDescriptorHeapDesc;
	DSVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;		
	DSVDescriptorHeapDesc.NumDescriptors = 1;		// ���е�������������DSA����Ϊ1
	DSVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;		
	DSVDescriptorHeapDesc.NodeMask = 0;									
	ANALYSIS_HRESULT(D3dDevice->CreateDescriptorHeap(&DSVDescriptorHeapDesc, IID_PPV_ARGS(DSVHeap.GetAddressOf())));

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/
	

	return false;
}


#endif

