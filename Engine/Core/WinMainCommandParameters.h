#pragma once
/* 传递WinMain参数 */

/* Windows平台 */
#if defined(_WIN32)

#include "../EngineMinimal.h"

// WinMain参数
class FWinMainCommandParameters
{
public:
	FWinMainCommandParameters(HINSTANCE InhInstance, HINSTANCE InpreInstance, PSTR IncmdLine, int InshowCmd);

	HINSTANCE HInstance;	// 当前应用程序的实例句柄
	HINSTANCE PreInstance;  // 上个应用程序的实例句柄
	PSTR CmdLine;			// 应用程序的命令行
	int ShowCmd;			// 控制窗口显示方式			
};

#elif 0

#endif 
//其他平台
