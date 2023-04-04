#include "WindowsEngine.h"
#if defined(_WIN32)
#include "WindowsMessageProcessing.h"

int FWindowsEngine::PreInit(FWinMainCommandParameters InParameters)
{
	// ��������

	if (InitWindows(InParameters))
	{

	}
	return 0;
}

int FWindowsEngine::Init()
{
	return 0;
}

int FWindowsEngine::PostInit()
{
	return 0;
}

void FWindowsEngine::Tick()
{

}

int FWindowsEngine::PreExit()
{
	return 0;
}

int FWindowsEngine::Exit()
{
	return 0;
}

int FWindowsEngine::PostExit()
{
	return 0;
}

bool FWindowsEngine::InitWindows(FWinMainCommandParameters InParameters)
{
	// ������������
	WNDCLASSEX WindowsClass;
	WindowsClass.cbSize = sizeof(WNDCLASSEX); // �ö���ʵ��ռ����ڴ�
	WindowsClass.cbClsExtra = 0;	 // �Ƿ���Ҫ����ռ�
	WindowsClass.cbWndExtra = 0;
	WindowsClass.hbrBackground = NULL; //����������Ǿ���GDI����
	WindowsClass.hCursor = LoadCursor(NULL, IDC_ARROW); // ����һ����ͷ���
	WindowsClass.hIcon = NULL; // Ӧ�ó�����ڴ�������ʾ��ͼ��
	WindowsClass.hIconSm = NULL; // Ӧ�ó�����ʾ�����Ͻǵ�ͼ��
	WindowsClass.hInstance = InParameters.HInstance; // ����ʵ��
	WindowsClass.lpszClassName = L"Wanderer Engine"; // ��������
	WindowsClass.lpszMenuName = NULL; // �˵�
	WindowsClass.style = CS_VREDRAW | CS_HREDRAW; // ��ô���ƴ��� ��ֱ��ˮƽ�����ı�ʱ�ػ�
	WindowsClass.lpfnWndProc = EngineWindowProc; // ��Ϣ������

	// ע�ᴰ��ʵ��
	ATOM RegisterAtom = RegisterClassEx(&WindowsClass); //ע�ᴰ��
	if (!RegisterAtom)
	{
		MessageBox(0, L"RegisterClass FAILED", L"Error", MB_OK); // ��Ϣ��
		return false;
	}

	// ���ڳߴ磺(left,top,right,bottom);
	RECT Rect = { 0,0,1280,720 };
	// �������ھ��Σ����ڣ�������ʽ���Ƿ��в˵���
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, NULL);

	// ���㴰�ڿ��
	int WindowWidth = Rect.right - Rect.left;
	int WindowHight = Rect.bottom - Rect.top;
	
	// �������ھ��
	HWND Hwnd = CreateWindowEx(
		NULL, // ���ڵ���չ������ʽ
		L"Wanderer Engine", // ��������
		L"Wanderer Engine", // ����ʾ�ڴ��ڵı�������ȥ
		WS_OVERLAPPEDWINDOW, // ���ڵ���ʽ��
		100, // ���ڵĳ�ʼˮƽλ�á�
		100, // ���ڵĳ�ʼ��ֱλ�á�
		WindowWidth, // ���ڵĿ��
		WindowHight, // ���ڵĸ߶�
		NULL, // ���ڵĸ����ڻ������ߴ��ڵľ��
		NULL, // �˵��ľ������ָ���Ӵ��ڱ�ʶ��
		InParameters.HInstance, // ����ʵ��
		NULL
	);
	
	if (!Hwnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	// ��ʾ����
	ShowWindow(Hwnd, SW_SHOW);
	UpdateWindow(Hwnd);
}


#endif

