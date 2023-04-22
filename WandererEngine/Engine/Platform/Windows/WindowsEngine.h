#pragma once
/* Windowsƽ̨���� */
#if defined(_WIN32)
#include "../../Core/Engine.h"
#include "../../Rendering/Engine/Core/RenderingEngine.h"

class CDirectXRenderingEngine;

// Windowsƽ̨������
class CWindowsEngine :public CEngine
{
	friend class IRenderingInterface;

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
	// ��ʼ��Windows���� 
	bool InitWindows(FWinMainCommandParameters InParameters);
	
protected:
	HWND MainWindowsHandle;	 // �����ھ��

protected:
	CDirectXRenderingEngine* RenderingEngine;
};

#endif