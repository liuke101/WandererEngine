#pragma once
/* 工厂模式——引擎工厂：创建各平台引擎 */
#include "Core/Engine.h"

// 引擎工厂类
class FEngineFactory
{
public:
	// 构造函数
	FEngineFactory();
	// 创建引擎
	static FEngine *CreatEngine();  
};