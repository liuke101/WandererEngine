#include "WindowsMessageProcessing.h"
#include "../../Component/Input/Input.h"
#include <windowsx.h>
LRESULT CALLBACK EngineWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// ����һЩ�ض�����Ϣ��ע�⣬�ڴ�����һ����Ϣ֮������Ӧ������0
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_RBUTTONDOWN:
		MouseDownDelegates.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

	case WM_RBUTTONUP:
		MouseUpDelegates.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

	case WM_MOUSEMOVE:
		MouseMoveDelegates.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		return 0;

	case WM_MOUSEWHEEL:
		MouseWheelDelegates.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), static_cast<short>HIWORD(wParam));
		return 0;
	}

	// ������û�д������Ϣת����Ĭ�ϵĴ��ڹ���
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
