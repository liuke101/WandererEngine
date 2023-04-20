#include "EngineFactory.h"

#if defined(_WIN32) 
#include "Platform/Windows/WindowsEngine.h"
#endif 

FEngineFactory::FEngineFactory()
{

}

CEngine* FEngineFactory::CreateEngine()
{
	// 创建Windows平台引擎
#if defined(_WIN32) 
	return new CWindowsEngine();

	// 其他平台
#elif defined(_LINUX_)

#endif 
}
