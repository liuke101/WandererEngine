#pragma once
/* ����ģʽ�������湤������������ */
#include "Core/Engine.h"

class FEngineFactory
{
public:
	FEngineFactory(); // ���캯��
	static FEngine *CreatEngine();  // ��������
};