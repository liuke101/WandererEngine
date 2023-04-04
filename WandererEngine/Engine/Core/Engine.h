#pragma once

//Windowsƽ̨
#if defined(_WIN32)

#include "WinMainCommandParameters.h"

#endif //1

class FEngine
{
public:
	//��ʼ��
	virtual int PreInit(
#if defined(_WIN32) 
		FWinMainCommandParameters Inparameters
#endif //1
	) = 0;
	virtual int Init() = 0;
	virtual int PostInit() = 0;

	//Tick
	virtual void Tick() = 0;

	//�˳�
	virtual int PreExit() = 0;
	virtual int Exit() = 0;
	virtual int PostExit() = 0;
};

