#include "EngineFactory.h"

#if defined(_WIN32) 
#include "Platform/Windows/WindowsEngine.h"
#endif 

FEngineFactory::FEngineFactory()
{

}

FEngine* FEngineFactory::CreatEngine()
{
#if defined(_WIN32) 
	return new FWindowsEngine;
#elif defined(_LINUX)

#endif //DEBUG
}
