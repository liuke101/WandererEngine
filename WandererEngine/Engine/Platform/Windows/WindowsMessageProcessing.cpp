#include "WindowsMessageProcessing.h"

LRESULT CALLBACK 
EngineWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// 处理一些特定的消息。注意，在处理完一个消息之后，我们应当返回0
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	// 将上面没有处理的消息转发给默认的窗口过程
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
