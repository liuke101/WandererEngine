#include "Camera.h"
#include "../Component/InputComponent.h"
#include "../Component/TransformationComponent.h"

CCamera::CCamera()
{
    InputComponent = CreateObject<CInputComponent>(new CInputComponent());
    TransformationComponent = CreateObject<CTransformationComponent>(new CTransformationComponent());
}

void CCamera::BeginInit()
{
    // 初始化透视投影矩阵
    ViewportInit();

    // 绑定按键
    InputComponent->KeyboardDelegate.Bind(this, &CCamera::ExecuteKeyboard);
    InputComponent->OnMouseButtonDownDelegate.Bind(this, &CCamera::OnMouseButtonDown);
    InputComponent->OnMouseButtonUpDelegate.Bind(this, &CCamera::OnMouseButtonUp);
    InputComponent->OnMouseMoveDelegate.Bind(this, &CCamera::OnMouseMove);
}

void CCamera::Tick(float DeltaTime)
{

}

void CCamera::ExecuteKeyboard(const FInputKey& InputKey)
{
    if(InputKey.KeyName == "W")
    {
        MoveForward(1.0f);
    }
    else if(InputKey.KeyName == "S")
    {
        MoveForward(-1.0f);
    }
    else if (InputKey.KeyName == "A")
    {
        MoveRight(1.0f);
    }
    else if (InputKey.KeyName == "D")
    {
        MoveRight(-1.0f);
    }
    
}

void CCamera::OnMouseButtonDown(int X, int Y)
{
    LastMousePosition.x = X;
    LastMousePosition.y = Y;
}

void CCamera::OnMouseButtonUp(int X, int Y)
{

}

void CCamera::OnMouseMove(int X, int Y)
{

}

// +1向前，-1向后
void CCamera::MoveForward(float InValue)
{

}

// +1向左，-1向右
void CCamera::MoveRight(float InValue)
{

}

