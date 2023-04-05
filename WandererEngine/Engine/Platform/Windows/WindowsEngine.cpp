#include "WindowsEngine.h"
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
	RECT Rect = { 0,0,1280,720 };
	// 调整窗口矩形（矩形坐标，窗口样式，是否有菜单）
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, NULL);

	// 计算窗口宽高
	int WindowWidth = Rect.right - Rect.left;
	int WindowHight = Rect.bottom - Rect.top;
	
	// 创建窗口句柄
	HWND Hwnd = CreateWindowEx(
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
	
	if (!Hwnd)
	{
		Engine_Log_Error("CreateWindow failed.");
		MessageBox(0, L"CreateWindow failed.", 0, 0);
		return false;
	}

	// 显示窗口
	ShowWindow(Hwnd, SW_SHOW);  // 疑问：这里第二项应该使用WinMain函数的InshowCmd参数
	UpdateWindow(Hwnd);

	Engine_Log("InitWindows complete.");
}


#endif

