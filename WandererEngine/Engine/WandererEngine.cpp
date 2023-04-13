/* Wanderer Engine */
#include "EngineMinimal.h"
#include "EngineFactory.h"
#include "Debug/Log/SimpleLog.h"

/* 初始化 */
int Init(FEngine* InEngine, HINSTANCE hInstance, HINSTANCE preInstance, PSTR cmdLine, int showCmd)
{
#if defined(_WIN32)
	FWinMainCommandParameters WinMainParameters(hInstance, hInstance, cmdLine, showCmd);
#endif

	int ReturnValue = InEngine->PreInit(
#if defined(_WIN32)
		WinMainParameters
#endif
	);

	if (ReturnValue != 0)
	{
		Engine_Log_Error("[%i]Engine pre-initialization error, check and initialization problem.", ReturnValue);
		return ReturnValue;
	}


	ReturnValue = InEngine->Init(
#if defined(_WIN32)
		WinMainParameters
#endif
	);

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
	float DeltaTime = 0.03f;	//每帧之间的时间差
	InEngine->Tick(DeltaTime);
	Sleep(30);					// 单位ms
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

	return ReturnValue;
}

//hInstance 当前应用程序的实例句柄
//preInstance 上个应用程序的实例句柄
//cmdLine 应用程序的命令行
//int showCmd 控制窗口显示方式
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE preInstance, _In_ PSTR cmdLine, _In_ int showCmd)
{
	int ReturnValue = 0;

	if (FEngine* Engine = FEngineFactory::CreatEngine())
	{
		// 初始化
		ReturnValue = Init(Engine, hInstance, preInstance, cmdLine, showCmd); 

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