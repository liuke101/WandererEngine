#pragma once
/* 输入输出组件 */
#include "Core/Component.h"
#include "Input/InputType.h"

DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureKeyboardInfoDelegate, void, const FInputKey &);

class CInputComponent : public CComponent
{
public:
    //CVARIABLE()
    FCaptureKeyboardInfoDelegate CaptureKeyboardInfoDelegate;

public:
    virtual void BeginInit();  
    virtual void Tick(float DeltaTime);
};