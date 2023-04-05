#pragma once
/* ����WinMain���� */

/* Windowsƽ̨ */
#if defined(_WIN32)

#include "../EngineMinimal.h"

// WinMain����
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

	HINSTANCE HInstance;	// ��ǰӦ�ó����ʵ�����
	HINSTANCE PreInstance;  // �ϸ�Ӧ�ó����ʵ�����
	PSTR CmdLine;			// Ӧ�ó����������
	int ShowCmd;			// ���ƴ�����ʾ��ʽ			
};

#elif 1

#endif 
//����ƽ̨
