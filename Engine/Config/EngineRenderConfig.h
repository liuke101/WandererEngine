#pragma once
/* 引擎渲染配置 */
class FEngineRenderConfig
{
public:
	FEngineRenderConfig();

	//屏幕配置
	int ScreenWidth;	// 屏幕宽度
	int ScreenHeight;	// 屏幕高度
	int RefreshRate;	// 刷新率
	int SwapChainCount;	// 交换链数量

	
	//获取渲染配置
	static FEngineRenderConfig* GetRenderConfig(); 
	//销毁渲染配置
	static void Destroy(); 

private:
	static FEngineRenderConfig* RenderConfig;
};