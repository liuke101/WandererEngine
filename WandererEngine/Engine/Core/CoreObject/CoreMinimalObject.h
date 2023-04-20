#pragma once
/* 核心Object，相当于UE的UObject，统一管理素有Object */
#include "../../Platform/Windows/WindowsPlatform.h"
#include "../../CodeReflection/CodeReflectionMacroTag.h"
#include "../../EngineMinimal.h"
#include "GuidInterface.h"

class CCoreMinimalObject : public IGuidInterface
{
public:
    CCoreMinimalObject();
    virtual ~CCoreMinimalObject();

    virtual void BeginInit() {}   //相当于UE的BeginPlay()
    virtual void Tick(float DeltaTime) {}

    // 创建指定对象
    template<class T>
    T* CreateObject(CCoreMinimalObject *NewObject)
    {
        return dynamic_cast<T*>(NewObject); //核心Object转换成子类Object
    }

    // 是否Tick，令对象可以Tick，引擎关闭Tick（否则无限循环，耗尽内存）
    bool IsTick() const { return bTick; }
  
protected:
    bool bTick;

};

extern vector<CCoreMinimalObject*> GObjects;