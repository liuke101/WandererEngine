#include "EngineFactory.h"

#if defined(_WIN32) 
#include "Platform/Windows/WindowsEngine.h"
#endif 

FEngineFactory::FEngineFactory()
{

}

FEngine* FEngineFactory::CreatEngine()
{
	// ����Windowsƽ̨����
#if defined(_WIN32) 
	return new FWindowsEngine;

	// ����ƽ̨
#elif defined(_LINUX)

#endif 
}
