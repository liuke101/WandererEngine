#include "WindowsEngine.h"
#include "../../Debug/EngineDebug.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Rendering/Core/Rendering.h"
#include "../../Mesh/BoxMesh.h"
#include "../../Mesh/SphereMesh.h"
#include "../../Mesh/CylinderMesh.h"
#include "../../Mesh/ConeMesh.h"
#include "../../Mesh/PlaneMesh.h"
#include "../../Mesh/CustomMesh.h"
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../../Core/World.h"
#include "../../Rendering/Engine/DirectX/DirectX12RenderingEngine.h"

#if defined(_WIN32)
#include "WindowsMessageProcessing.h"


CWindowsEngine::CWindowsEngine()
    : RenderingEngine(new CDirectX12RenderingEngine())
{
	
}

CWindowsEngine::~CWindowsEngine()
{
	delete RenderingEngine;
}

int CWindowsEngine::PreInit(FWinMainCommandParameters InParameters)
{
	// ��־ϵͳ��ʼ��
	const char LogPath[] = "../log";  
	init_log_system(LogPath);
	Engine_Log("Log Init.");

	// ��������

	Engine_Log("Engine pre-initialization complete.");

	//��Ⱦ�����ʼ��
	RenderingEngine->PreInit(InParameters);
	return 0;
}

int CWindowsEngine::Init(FWinMainCommandParameters InParameters)
{

	InitWindows(InParameters);

	RenderingEngine->SetMianWindowsHandle(MainWindowsHandle);

	RenderingEngine->Init(InParameters);

	CWorld *World= CreateObject<CWorld>(new CWorld());

	Engine_Log("Engine initialization complete.");
	return 0;
}

int CWindowsEngine::PostInit()
{
	RenderingEngine->PostInit();
	
	// �����ʼ��
	for (auto& Temp : GObjects)
	{
		Temp->BeginInit();
	}

	Engine_Log("Engine post-initialization complete.");
	return 0;
}

void CWindowsEngine::Tick(float DeltaTime)
{
	// ����Tick
	for (auto& Temp : GObjects)
	{
		if (Temp->IsTick())
		{
			Temp->Tick(DeltaTime);
		}
	}
	
	RenderingEngine->Tick(DeltaTime);
}

int CWindowsEngine::PreExit()
{
	RenderingEngine->PreExit();

	Engine_Log("Engine pre-exit complete.");
	return 0;
}

int CWindowsEngine::Exit()
{
	RenderingEngine->Exit();

	Engine_Log("Engine exit complete.");
	return 0;
}

int CWindowsEngine::PostExit()
{
	FEngineRenderConfig::Destroy();
	RenderingEngine->PostExit();

	Engine_Log("Engine post-exit complete.");
	return 0;
}


bool CWindowsEngine::InitWindows(FWinMainCommandParameters InParameters)
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

	return true;
}

#endif