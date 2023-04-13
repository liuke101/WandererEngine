#pragma once
/* 引擎抽象类 */

// Windows平台
#if defined(_WIN32)

#include "WinMainCommandParameters.h"

#endif //1

// 引擎抽象类
class FEngine
{
public:
	// 初始化 
	virtual int PreInit(	
#if defined(_WIN32) 
		FWinMainCommandParameters InParameters
#endif //1
	) = 0;

	virtual int Init(	
#if defined(_WIN32) 
		FWinMainCommandParameters InParameters
#endif //1
	) = 0;	

	virtual int PostInit() = 0;		

	// Tick 
	virtual void Tick(float DeltaTime) = 0;

	// 退出
	virtual int PreExit() = 0;
	virtual int Exit() = 0;
	virtual int PostExit() = 0;
};

