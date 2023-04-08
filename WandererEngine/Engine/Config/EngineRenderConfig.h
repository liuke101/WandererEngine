#pragma once
/* 引擎渲染配置 */
class FEngineRenderConfig
{
public:
	FEngineRenderConfig();

	int ScreenWidth;	// 屏幕宽度
	int ScreenHeight;	// 屏幕高度
	int RefreshRate;	// 刷新率
	int SwapChainCount;	// 交换链数量

	//获取渲染配置
	static FEngineRenderConfig* GetRenderConfig(); 
	//销毁渲染配置
	static void Destory(); 

private:
	static FEngineRenderConfig* RenderConfig;
};