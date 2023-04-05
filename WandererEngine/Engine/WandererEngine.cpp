/* Wanderer Engine */
#include "EngineMinimal.h"
#include "EngineFactory.h"
#include "Debug/Log/SimpleLog.h"

/* ��ʼ�� */
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

/* �˳� */
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

//hInstance ��ǰӦ�ó����ʵ�����
//preInstance �ϸ�Ӧ�ó����ʵ�����
//cmdLine Ӧ�ó����������
//int showCmd ���ƴ�����ʾ��ʽ
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE preInstance, _In_ PSTR cmdLine, _In_ int showCmd)
{
	int ReturnValue = 0;

	if (FEngine* Engine = FEngineFactory::CreatEngine())
	{
		// ��ʼ��
		ReturnValue = Init(Engine, hInstance, preInstance, cmdLine, showCmd); 

		// Tick
		while (true)
		{
			Tick(Engine);
		}

		// �˳�
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