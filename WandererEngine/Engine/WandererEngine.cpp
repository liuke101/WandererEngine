/* Wanderer Engine */
#include "EngineMinimal.h"
#include "EngineFactory.h"
#include "Debug/Log/SimpleLog.h"

/* ��ʼ�� */
int Init(CEngine* InEngine, HINSTANCE hInstance, HINSTANCE preInstance, PSTR cmdLine, int showCmd)
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

	return ReturnValue;
}

/* Tick */
void Tick(CEngine* InEngine)
{
	float DeltaTime = 0.03f;	//ÿ֮֡���ʱ���
	InEngine->Tick(DeltaTime);
	//Sleep(30);					// ��λms
}

/* �˳� */
int Exit(CEngine* InEngine)
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

CEngine* Engine = nullptr;
//hInstance ��ǰӦ�ó����ʵ�����
//preInstance �ϸ�Ӧ�ó����ʵ�����
//cmdLine Ӧ�ó����������
//int showCmd ���ƴ�����ʾ��ʽ
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE preInstance, _In_ PSTR cmdLine, _In_ int showCmd)
{
	int ReturnValue = 0;

	Engine = FEngineFactory::CreateEngine();
    if (Engine)
	{
		// ��ʼ��
		Init(Engine, hInstance, preInstance, cmdLine, showCmd); 

		MSG EngineMsg = { 0 };
		// Tick
		while (EngineMsg.message != WM_QUIT)
		{
			// PM_NOREMOVE			PeekMessage ����󲻻�Ӷ�����ɾ����Ϣ��
			// PM_REMOVE			PeekMessage ����󣬽��Ӷ�����ɾ����Ϣ��
		    // PM_NOYIELD			��ֹϵͳ�ͷŵȴ����÷�����(���� WaitForInputIdle) ���κ��̡߳�����ֵ�� PM_NOREMOVE �� PM_REMOVE�����һ��
			// 
			// PM_QS_INPUT			�������ͼ�����Ϣ��
			// PM_QS_PAINT			���������Ϣ��
			// PM_QS_POSTMESSAGE	���������ѷ�������Ϣ��������ʱ�����ȼ���
			// PM_QS_SENDMESSAGE	���������ѷ��͵���Ϣ��
			if (PeekMessage(&EngineMsg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&EngineMsg);
				DispatchMessage(&EngineMsg);
			}
			else
			{
				Tick(Engine);
			}
		}

		// �˳�
		ReturnValue = Exit(Engine);
	}
	else
	{
		ReturnValue = 1;
	}

	Engine_Log("[%i]The engine has exited.", ReturnValue);
	return ReturnValue;
}