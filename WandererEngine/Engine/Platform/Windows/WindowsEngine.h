#pragma once
/* Windows平台引擎 */
#if defined(_WIN32)
#include "../../Core/Engine.h"

// Windows平台引擎类
class FWindowsEngine :public FEngine
{
public:
	virtual int PreInit(FWinMainCommandParameters InParameters);
	virtual int Init();
	virtual int PostInit();

	virtual void Tick();

	virtual int PreExit();
	virtual int Exit();
	virtual int PostExit();

private:
	// 初始化Windows窗口 
	bool InitWindows(FWinMainCommandParameters InParameters);
};

#endif