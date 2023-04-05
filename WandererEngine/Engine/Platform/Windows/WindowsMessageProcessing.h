#pragma once
/* 消息处理 */

#include "../../Core/Engine.h"

// 窗口过程函数
// hWnd	接收消息的窗口过程的句柄。
// msg	消息
// wParam	其他的消息特定信息
// lParam	其他的消息特定信息
LRESULT CALLBACK
EngineWindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
