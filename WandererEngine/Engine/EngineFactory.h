#pragma once
/* 工厂模式——引擎工厂：生成引擎 */
#include "Core/Engine.h"

class FEngineFactory
{
public:
	FEngineFactory(); // 构造函数
	static FEngine *CreatEngine();  // 创建引擎
};