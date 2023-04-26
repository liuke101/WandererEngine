#pragma once
/* ������Ⱦ���� */
class FEngineRenderConfig
{
public:
	FEngineRenderConfig();

	//��Ļ����
	int ScreenWidth;	// ��Ļ���
	int ScreenHeight;	// ��Ļ�߶�
	int RefreshRate;	// ˢ����
	int SwapChainCount;	// ����������

	
	//��ȡ��Ⱦ����
	static FEngineRenderConfig* GetRenderConfig(); 
	//������Ⱦ����
	static void Destroy(); 

private:
	static FEngineRenderConfig* RenderConfig;
};