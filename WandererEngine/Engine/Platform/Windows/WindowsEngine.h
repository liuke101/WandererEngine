#pragma once
/* Windowsƽ̨���� */
#if defined(_WIN32)
#include "../../Core/Engine.h"
#include "../../Rendering/Engine/Core/RenderingEngine.h"

class CDirectXRenderingEngine;
class CWorld;

// Windowsƽ̨������
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

public:
	// ��ʼ��Windows���� 
	bool InitWindows(FWinMainCommandParameters InParameters);
	
protected:
	HWND MainWindowsHandle;	 // �����ھ��

protected:
	CDirectXRenderingEngine* RenderingEngine;
	CWorld* World;
};

#endif