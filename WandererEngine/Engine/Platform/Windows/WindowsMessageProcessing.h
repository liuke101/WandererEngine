#pragma once
/* ��Ϣ���� */

#include "../../Core/Engine.h"

// ���ڹ��̺���
// hWnd	������Ϣ�Ĵ��ڹ��̵ľ����
// msg	��Ϣ
// wParam	��������Ϣ�ض���Ϣ
// lParam	��������Ϣ�ض���Ϣ
LRESULT CALLBACK
EngineWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
