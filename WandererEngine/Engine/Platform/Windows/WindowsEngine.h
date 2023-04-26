#pragma once
/* Windows平台引擎 */
#if defined(_WIN32)
#include "../../Core/Engine.h"
#include "../../Rendering/Engine/Core/RenderingEngine.h"

class CWorld;
class CDirectXRenderingEngine;

// Windows平台引擎类
class CWindowsEngine :public CEngine
{
	friend class IDirectXDeviceInterface;
public:
	CWindowsEngine();
	~CWindowsEngine();
	virtual int PreInit(FWinMainCommandParameters InParameters);
	virtual int Init(FWinMainCommandParameters InParameters);
	virtual int PostInit();

	virtual void Tick(float DeltaTime);

	virtual int PreExit();
	virtual int Exit();
	virtual int PostExit();


	CDirectXRenderingEngine* GetRenderingEngine() { return RenderingEngine; }

private:
	// 初始化Windows窗口 
	bool InitWindows(FWinMainCommandParameters InParameters);
	
protected:
	HWND MainWindowsHandle;	 // 主窗口句柄

protected:
	CDirectXRenderingEngine* RenderingEngine;
	CWorld* World;
};

#endif