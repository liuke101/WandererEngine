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
			break;
	
	    case WM_RBUTTONDOWN:
	        MouseDownDelegates.Broadcast(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam));
			break;
	
	    case WM_RBUTTONUP:
		    MouseDownDelegates.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
	
	    case WM_MOUSEMOVE:
		    MouseDownDelegates.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
			break;
	
	    case WM_MOUSEWHEEL:
		    MouseWheelDelegates.Broadcast(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), static_cast<short>HIWORD(wParam));
			break;
		default:
			break;
	}
	
	// ������û�д������Ϣת����Ĭ�ϵĴ��ڹ���
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

