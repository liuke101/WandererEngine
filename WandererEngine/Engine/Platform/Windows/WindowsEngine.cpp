#include "WindowsEngine.h"
#if defined(_WIN32)
#include "WindowsMessageProcessing.h"

int FWindowsEngine::PreInit(FWinMainCommandParameters InParameters)
{
	// 处理命令

	/*if (InitWindows(InParameters))
	{

	}*/
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
	// 描述窗口特征
	WNDCLASSEX WindowsClass;
	WindowsClass.cbSize = sizeof(WNDCLASSEX); // 该对象实际占多大内存
	WindowsClass.cbClsExtra = 0;	 // 是否需要额外空间
	WindowsClass.hbrBackground = NULL; //如果有设置那就是GDI擦除
	WindowsClass.hCursor = LoadCursor(NULL, IDC_ARROW); // 设置一个箭头光标
	WindowsClass.hIcon = NULL; // 应用程序放在磁盘上显示的图标
	WindowsClass.hIconSm = NULL; // 应用程序显示在左上角的图标
	WindowsClass.hInstance = InParameters.HInstance; // 窗口实例
	WindowsClass.lpszClassName = L"Wanderer Engine"; // 窗口名称
	WindowsClass.lpszMenuName = NULL; // 菜单
	WindowsClass.style = CS_VREDRAW | CS_HREDRAW; // 怎么绘制窗口 垂直和水平发生改变时重绘
	WindowsClass.lpfnWndProc = EngineWindowProc; // 消息处理函数

	// 注册窗口实例
	ATOM RegisterAtom = RegisterClassEx(&WindowsClass); //注册窗口
	if (!RegisterAtom)
	{
		MessageBox(0, L"RegisterClass FAILED", L"Error", MB_OK); // 消息框
		return false;
	}

	// 窗口尺寸：(left,top,right,bottom);
	RECT Rect = { 0,0,1280,720 };
	// 调整窗口矩形（窗口，窗口样式，是否有菜单）
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, NULL);

	// 计算窗口宽高
	int WindowWidth = Rect.right - Rect.left;
	int WindowHight = Rect.bottom - Rect.top;
	
	// 创建窗口句柄
	HWND Hwnd = CreateWindowEx(
		NULL, // 窗口的扩展窗口样式
		L"Wanderer Engine", // 窗口名称
		L"RENZHAI Engine", // 会显示在窗口的标题栏上去
		WS_OVERLAPPEDWINDOW, // 窗口的样式。
		100, // 窗口的初始水平位置。
		100, // 窗口的初始垂直位置。
		WindowWidth, // 窗口的宽度
		WindowHight, // 窗口的高度
		NULL, // 窗口的父窗口或所有者窗口的句柄
		NULL, // 菜单的句柄，或指定子窗口标识符
		InParameters.HInstance, // 窗口实例
		NULL
	);
	
	if (!Hwnd)
	{
		MessageBox(0, L"CreateWindow Failed.", 0, 0);
		return false;
	}

	// 显示窗口
	ShowWindow(Hwnd, SW_SHOW);
	UpdateWindow(Hwnd);
}


#endif

