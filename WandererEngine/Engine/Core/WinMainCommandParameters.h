#pragma once
//Windows平台
#if defined(_WIN32)

#include "../EngineMinimal.h"

class FWinMainCommandParameters
{
public:
	FWinMainCommandParameters(HINSTANCE InhInstance, HINSTANCE InpreInstance, PSTR IncmdLine, int InshowCmd);

	HINSTANCE HInstance;
	HINSTANCE PreInstance;
	PSTR CmdLine;
	int ShowCmd;
};

#elif 1

#endif 
//其他平台
