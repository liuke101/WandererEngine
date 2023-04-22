#pragma once
#include "../../../Core/Engine.h"

class CRenderingEngine : public CEngine
{
public:
	void SetMianWindowsHandle(HWND InNewMainWindowsHandle);

protected:
	HWND MainWindowsHandle;
};