#include "EngineMinimal.h"
#include "EngineFactory.h"
#include "Debug/Log/SimpleLog.h"

/* 初始化 */
int Init(FEngine* InEngine, HINSTANCE hInstance, HINSTANCE preInstance, PSTR cmdLine, int showCmd)
{
	int ReturnValue = InEngine->PreInit(
#if defined(_WIN32)
		FWinMainCommandParameters(hInstance, hInstance, cmdLine, showCmd)
#endif
	);

	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine pre-initialization error, check and initialization problem.", ReturnValue);
		return ReturnValue;
	}


	ReturnValue = InEngine->Init();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine initialization error, check and initialization problem.", ReturnValue);
		return ReturnValue;
	}

	ReturnValue = InEngine->PostInit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine post-initialization error, check and initialization problem.", ReturnValue);
		return ReturnValue;
	}
}

/* Tick */
void Tick(FEngine* InEngine)
{
	InEngine->Tick();
}

/* 退出 */
int Exit(FEngine* InEngine)
{
	
	int ReturnValue = InEngine->PreExit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine pre-exit failed.", ReturnValue);
		return ReturnValue;
	}

	ReturnValue = InEngine->Exit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine exit failed.", ReturnValue);
		return ReturnValue;
	}

	ReturnValue = InEngine->PostExit();
	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine post-exit failed.", ReturnValue);
		return ReturnValue;
	}
}

//hInstance 自己的实例
//preInstance 上次的实例
//cmdLine 传递命令
//int showCmd 多少条cmd
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE preInstance, PSTR cmdLine, int showCmd)
{
	int ReturnValue = 0;

	if (FEngine* Engine = FEngineFactory::CreatEngine())
	{
		// 初始化
		ReturnValue = Init(Engine, hInstance, preInstance, cmdLine, showCmd); // 人宅这里把直接写Init，为什么？

		// Tick
		while (true)
		{
			Tick(Engine);
		}

		// 退出
		ReturnValue = Exit(Engine);

		ReturnValue = 0;
	}
	else
	{
		ReturnValue = 1;
	}

	Engine_Log("[%i]The engine has exited.", ReturnValue);
	return ReturnValue;
}