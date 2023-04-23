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
    BuildViewMatrix(DeltaTime);
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

void CCamera::BuildViewMatrix(float DeltaTime)
{

    TransformationComponent->CalcLRUVector();

    fvector_3d V3;
    TransformationComponent->CalcNegativePosVecotr(V3);

    XMFLOAT3 LookatVector = TransformationComponent->GetLookatVector();
    XMFLOAT3 RightVector = TransformationComponent->GetRightVector();
    XMFLOAT3 UpVector = TransformationComponent->GetUpVector();

    ViewMatrix = {
        RightVector.x, UpVector.x, LookatVector.x, 0.0f,
        RightVector.y, UpVector.y, LookatVector.y, 0.0f,
        RightVector.z, UpVector.z, LookatVector.z, 0.0f,
        V3.x,          V3.y,       V3.z,            1.f };
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
    XMFLOAT3 fPosition = TransformationComponent->GetPosition();
    XMFLOAT3 fLookat = TransformationComponent->GetLookatVector();

    XMVECTOR AmountMovement = XMVectorReplicate(InValue * 1.0f);
    XMVECTOR Lookat = XMLoadFloat3(&fLookat);
    XMVECTOR Position = XMLoadFloat3(&fPosition);

    XMStoreFloat3(&fPosition,XMVectorMultiplyAdd(AmountMovement, Lookat, Position));

    TransformationComponent->SetPosition(fPosition);
}
// +1向左，-1向右
void CCamera::MoveRight(float InValue)
{

}


