#pragma once
/* ����Object���൱��UE��UObject��ͳһ��������Object */
#include "../../Platform/Windows/WindowsPlatform.h"
#include "../../CodeReflection/CodeReflectionMacroTag.h"
#include "../../EngineMinimal.h"
#include "GuidInterface.h"

class CCoreMinimalObject : public IGuidInterface
{
public:
    CCoreMinimalObject();
    virtual ~CCoreMinimalObject();

    virtual void BeginInit() {}   //�൱��UE��BeginPlay()
    virtual void Tick(float DeltaTime) {}

    // ����ָ������
    template<class T>
    T* CreateObject(CCoreMinimalObject *NewObject)
    {
        return dynamic_cast<T*>(NewObject); //����Objectת��������Object
    }

    // �Ƿ�Tick����������Tick������ر�Tick����������ѭ�����ľ��ڴ棩
    bool IsTick() const { return bTick; }
  
protected:
    bool bTick;

};

extern vector<CCoreMinimalObject*> GObjects;