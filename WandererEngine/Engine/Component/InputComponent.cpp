#include "InputComponent.h"
#include "Input/Input.h"
void CInputComponent::BeginInit()
{
    // °ó¶¨Î¯ÍÐ
    MouseDownDelegates.AddFunction(this, &CInputComponent::OnMouseButtonDown);
    MouseUpDelegates.AddFunction(this, &CInputComponent::OnMouseButtonUp);
    MouseMoveDelegates.AddFunction(this, &CInputComponent::OnMouseMove);
    MouseWheelDelegates.AddFunction(this, &CInputComponent::OnMouseWheel);
}

void CInputComponent::Tick(float DeltaTime)
{
    if(KeyboardDelegate.IsBound())
    {
        FInputKey InputKey;
        if(GetAsyncKeyState('W') & 0x8000)
        {
            InputKey.KeyName = "W";
        }
        else if (GetAsyncKeyState('S') & 0x8000)
        {
            InputKey.KeyName = "S";
        }
        else if (GetAsyncKeyState('A') & 0x8000)
        {
            InputKey.KeyName = "A";
        }
        else if(GetAsyncKeyState('D') & 0x8000)
        {
            InputKey.KeyName = "D";
        }
        else if (GetAsyncKeyState('Q') & 0x8000)
        {
            InputKey.KeyName = "Q";
        }
        else if (GetAsyncKeyState('E') & 0x8000)
        {
            InputKey.KeyName = "E";
        }
        else
        {
            return;
        }
        KeyboardDelegate.Execute(InputKey);
    }
}

void CInputComponent::OnMouseButtonDown(int X, int Y)
{
    if(OnMouseButtonDownDelegate.IsBound())
    {
        OnMouseButtonDownDelegate.Execute(X, Y);
    }

}

void CInputComponent::OnMouseButtonUp(int X, int Y)
{
    if (OnMouseButtonUpDelegate.IsBound())
    {
        OnMouseButtonUpDelegate.Execute(X, Y);
    }
}

void CInputComponent::OnMouseMove(int X, int Y)
{
    if (OnMouseMoveDelegate.IsBound())
    {
        OnMouseMoveDelegate.Execute(X, Y);
    }
}

void CInputComponent::OnMouseWheel(int X, int Y, float InDelta)
{
    if (OnMouseWheelDelegate.IsBound())
    {
        OnMouseWheelDelegate.Execute(X, Y, InDelta);
    }
}
