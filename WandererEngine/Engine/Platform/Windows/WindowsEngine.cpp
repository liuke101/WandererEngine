#include "WindowsEngine.h"
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
	RECT Rect = { 0,0,1280,720 };
	// �������ھ��Σ��������꣬������ʽ���Ƿ��в˵���
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, NULL);

	// ���㴰�ڿ��
	int WindowWidth = Rect.right - Rect.left;
	int WindowHight = Rect.bottom - Rect.top;
	
	// �������ھ��
	HWND Hwnd = CreateWindowEx(
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
	
	if (!Hwnd)
	{
		Engine_Log_Error("CreateWindow failed.");
		MessageBox(0, L"CreateWindow failed.", 0, 0);
		return false;
	}

	// ��ʾ����
	ShowWindow(Hwnd, SW_SHOW);  // ���ʣ�����ڶ���Ӧ��ʹ��WinMain������InshowCmd����

	// ˢ�´���
	UpdateWindow(Hwnd);

	Engine_Log("InitWindows complete.");
}


#endif

