#include "EngineMinimal.h"
#include "EngineFactory.h"

int  WINAPI WinMain(HINSTANCE hInstance, HINSTANCE preInstance, PSTR cmdLine, int showCmd)
{
	int ReturnValue = 0;

	if (FEngine* Engine = FEngineFactory::CreatEngine())
	{
		ReturnValue = Engine->PreInit(
#if defined(_WIN32)
			FWinMainCommandParameters(hInstance, hInstance, cmdLine, showCmd)
#endif
		);

		if (ReturnValue != 0)
		{
			return ReturnValue;
		}

		
		ReturnValue = Engine->Init();
		if (ReturnValue != 0)
		{
			return ReturnValue;
		}

		ReturnValue = Engine->PostInit();
		if (ReturnValue != 0)
		{
			return ReturnValue;
		}

		while (true)
		{
			Engine->Tick();
		}

		ReturnValue = Engine->PreExit();
		if (ReturnValue != 0)
		{
			return ReturnValue;
		}

		ReturnValue = Engine->Exit();
		if (ReturnValue != 0)
		{
			return ReturnValue;
		}

		ReturnValue = Engine->PostExit();
		if (ReturnValue != 0)
		{
			return ReturnValue;
		}

		ReturnValue = 0;
	}
	else
	{
		ReturnValue = 1;
	}

	return ReturnValue;
}