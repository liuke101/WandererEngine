#include "WindowsMessageProcessing.h"

LRESULT CALLBACK 
EngineWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	// ����һЩ�ض�����Ϣ��ע�⣬�ڴ�����һ����Ϣ֮������Ӧ������0
	switch (msg)
	{
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	// ������û�д������Ϣת����Ĭ�ϵĴ��ڹ���
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
