#pragma once
// ������Ⱦ����
class FEngineRenderConfig
{
public:
	FEngineRenderConfig();

	int ScreenWidth;	// ��Ļ���
	int ScreenHeight;	// ��Ļ�߶�
	int RefreshRate;	// ˢ����
	int SwapChainCount;	// ����������

	//��ȡ��Ⱦ����
	static FEngineRenderConfig* GetRenderConfig(); 
	//������Ⱦ����
	static void Destory(); 

private:
	static FEngineRenderConfig* RenderConfig;
};