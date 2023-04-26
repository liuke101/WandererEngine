#include "Camera.h"
#include "../Component/InputComponent.h"
#include "../Component/TransformationComponent.h"

CCamera::CCamera()
{
    InputComponent = CreateObject<CInputComponent>(new CInputComponent());
    TransformationComponent = CreateObject<CTransformationComponent>(new CTransformationComponent());

    MouseSensitivity = 1.0f;
}

void CCamera::BeginInit()
{
    // ��ʼ��͸��ͶӰ����
    ViewportInit();

    // �󶨰���
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
        MoveRight(-1.0f);
    }
    else if (InputKey.KeyName == "D")
    {
        MoveRight(1.0f);
    }
    
}

void CCamera::BuildViewMatrix(float DeltaTime)
{

    TransformationComponent->CalcLRUVector();

    fvector_3d V3;
    TransformationComponent->CalcNegativePosVector(V3);

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
    bLeftMouseDown = true;
    LastMousePosition.x = X;
    LastMousePosition.y = Y;

    SetCapture(GetMainWindowsHandle()); // ��겶��
}

void CCamera::OnMouseButtonUp(int X, int Y)
{
    bLeftMouseDown = false;

    ReleaseCapture();           // �ͷ���겶��
    LastMousePosition.x = X;
    LastMousePosition.y = Y;
    
}

void CCamera::OnMouseMove(int X, int Y)
{
    if(bLeftMouseDown)
    {
        // ���������ƶ����������ת�Ƕ�
        float XRadians = XMConvertToRadians(static_cast<float>(X - LastMousePosition.x) * MouseSensitivity);
        float YRadians = XMConvertToRadians(static_cast<float>(Y - LastMousePosition.x) * MouseSensitivity);

        RotateAroundYAxis(XRadians);
        RotateAroundZAxis(YRadians);
    }
    LastMousePosition.x = X;
    LastMousePosition.y = Y;
}

// +1��ǰ��-1���
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
// +1���ң�-1����
void CCamera::MoveRight(float InValue)
{
    XMFLOAT3 fPosition = TransformationComponent->GetPosition();
    XMFLOAT3 fRight = TransformationComponent->GetRightVector();

    XMVECTOR AmountMovement = XMVectorReplicate(InValue * 1.0f);
    XMVECTOR Right = XMLoadFloat3(&fRight);
    XMVECTOR Position = XMLoadFloat3(&fPosition);

    XMStoreFloat3(&fPosition, XMVectorMultiplyAdd(AmountMovement, Right, Position));

    TransformationComponent->SetPosition(fPosition);
}

void CCamera::RotateAroundYAxis(float InRotateDegrees)
{
    // ��ȡ�����xyz��
    XMFLOAT3 RightVector = TransformationComponent->GetRightVector();
    XMFLOAT3 UpVector = TransformationComponent->GetUpVector();
    XMFLOAT3 LookatVector = TransformationComponent->GetLookatVector();

    // ��ȡ��Y����ת����ת����
    XMMATRIX RotationY = XMMatrixRotationY(InRotateDegrees);

    // ���������������Y����ת
    XMStoreFloat3(&TransformationComponent->GetRightVector(), XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationY));
    XMStoreFloat3(&TransformationComponent->GetUpVector(), XMVector3TransformNormal(XMLoadFloat3(&UpVector), RotationY));
    XMStoreFloat3(&TransformationComponent->GetLookatVector(), XMVector3TransformNormal(XMLoadFloat3(&LookatVector), RotationY));
}

void CCamera::RotateAroundZAxis(float InRotateDegrees)
{
    // ��ȡ�����xyz��
    XMFLOAT3 RightVector = TransformationComponent->GetRightVector();
    XMFLOAT3 UpVector = TransformationComponent->GetUpVector();
    XMFLOAT3 LookatVector = TransformationComponent->GetLookatVector();

    // ��ȡ��Z����ת����ת����
    XMMATRIX RotationZ = XMMatrixRotationZ(InRotateDegrees);

    // ���������������z����ת
    XMStoreFloat3(&TransformationComponent->GetRightVector(),XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationZ));
    XMStoreFloat3(&TransformationComponent->GetUpVector(), XMVector3TransformNormal(XMLoadFloat3(&UpVector), RotationZ));
    XMStoreFloat3(&TransformationComponent->GetLookatVector(), XMVector3TransformNormal(XMLoadFloat3(&LookatVector), RotationZ));
}


