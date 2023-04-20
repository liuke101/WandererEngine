#include "InputComponent.h"

void CInputComponent::BeginInit()
{
}

void CInputComponent::Tick(float DeltaTime)
{
    if(CaptureKeyboardInfoDelegate.IsBound())
    {
        FInputKey InputKey;
        CaptureKeyboardInfoDelegate.Execute(InputKey);
    }
}
