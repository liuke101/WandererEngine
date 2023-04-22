#pragma once
/* 输入输出组件 */
#include "Core/Component.h"
#include "Input/InputType.h"

// 单播
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureKeyboardDelegate, void, const FInputKey &);
DEFINITION_SIMPLE_SINGLE_DELEGATE(FCaptureMouseDelegate, void, int, int);

class CInputComponent : public CComponent
{
public:
    CVARIABLE()
    FCaptureKeyboardDelegate KeyboardDelegate;

    CVARIABLE()
    FCaptureMouseDelegate OnMouseButtonDownDelegate;

    CVARIABLE()
    FCaptureMouseDelegate OnMouseButtonUpDelegate;

    CVARIABLE()
    FCaptureMouseDelegate OnMouseMoveDelegate;
public:

    virtual void BeginInit();  
    virtual void Tick(float DeltaTime);

    virtual void OnMouseButtonDown(int X, int Y);
    virtual void OnMouseButtonUp(int X, int Y);
    virtual void OnMouseMove(int X, int Y);
};