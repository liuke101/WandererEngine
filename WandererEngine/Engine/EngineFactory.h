#pragma once
/* ����ģʽ�������湤����������ƽ̨���� */
#include "Core/Engine.h"

// ���湤����
class FEngineFactory
{
public:
	// ���캯��
	FEngineFactory();
	// ��������
	static CEngine *CreateEngine();  
};