#pragma once

/* 传递Windows平台指令参数 */
#if defined(_WIN32)

#include "../EngineMinimal.h"

class FWinMainCommandParameters
{
public:
	FWinMainCommandParameters(HINSTANCE InhInstance, HINSTANCE InpreInstance, PSTR IncmdLine, int InshowCmd)
		:HInstance(InhInstance)
		, PreInstance(InpreInstance)
		, CmdLine(IncmdLine)
		, ShowCmd(InshowCmd)
	{

	}

	HINSTANCE HInstance;
	HINSTANCE PreInstance;
	PSTR CmdLine;
	int ShowCmd;
};

#elif 1

#endif 
//其他平台
