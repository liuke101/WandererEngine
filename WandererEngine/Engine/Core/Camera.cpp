#include "Camera.h"

#include "ECameraType.h"
#include "../Component/InputComponent.h"
#include "../Component/TransformationComponent.h"

CCamera::CCamera()
{
    InputComponent = CreateObject<CInputComponent>(new CInputComponent());
    TransformationComponent = CreateObject<CTransformationComponent>(new CTransformationComponent());

    MouseSensitivity = 0.5f;
    CameraType = ECameraType::CameraRoaming;
    SphericalRadius = 10.0f;
    alpha = XM_PI;
    beta = XM_PI;
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
    InputComponent->OnMouseWheelDelegate.Bind(this, &CCamera::OnMouseWheel);
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
    // ��Q�л��۲����ģʽ
    else if (InputKey.KeyName == "Q")
    {
        CameraType = ECameraType::ObservationObject;
    }
    // ��W�л��������ģʽ
    else if (InputKey.KeyName == "E")
    {
        CameraType = ECameraType::CameraRoaming;
    }
}

void CCamera::BuildViewMatrix(float DeltaTime)
{
    switch (CameraType)
    {
        case ECameraType::CameraRoaming:
        {
            // �����۲�任����
            // ���ǿ�����������ģ�lookatֻ�ᳯ���������ǰ��
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

            break;
        }

        case ECameraType::ObservationObject:
        {
            // ��������ת���ѿ�������
            // ���ǿ�������ģ�lookatֻ�ῴ�����壬����ʹ���������꣬����Χ������۲졣
            XMFLOAT3 &CameraPos = TransformationComponent->GetPosition();

            CameraPos.x = SphericalRadius * cosf(alpha) * cosf(beta);
            CameraPos.y = SphericalRadius * sinf(alpha);
            CameraPos.z = SphericalRadius * cosf(alpha) * sinf(beta);

            XMVECTOR Pos = XMVectorSet(CameraPos.x, CameraPos.y, CameraPos.z, 1.0f);
            XMVECTOR ViewTarget = XMVectorZero();
            XMVECTOR ViewUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

            XMMATRIX ViewLookat = XMMatrixLookAtLH(Pos, ViewTarget, ViewUp);
            XMStoreFloat4x4(&ViewMatrix, ViewLookat);

            break;
        }
    }
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
    if (bLeftMouseDown)
    {
        // ���������ƶ����������ת�Ƕ�
        float XRadians = XMConvertToRadians(static_cast<float>(X - LastMousePosition.x) * MouseSensitivity);
        float YRadians = XMConvertToRadians(static_cast<float>(Y - LastMousePosition.y) * MouseSensitivity);

        switch (CameraType)
        {
            case CameraRoaming:
            {
                RotateAroundYAxis(XRadians);
                RotateAroundRAxis(YRadians);

                break;
            }
            case ObservationObject:
            {
                beta += XRadians;
                alpha += YRadians;
                //alpha = math_libray::Clamp(alpha, 0.0f, XM_2PI*2);    // �����ӽ�ƫ����
                break;
            }
        }
    }
    LastMousePosition.x = X;
    LastMousePosition.y = Y;
}

void CCamera::OnMouseWheel(int X, int Y, float InDelta)
{
    if(CameraType==ECameraType::ObservationObject)
    {
        SphericalRadius += (InDelta / 100.f);

        // ���ƹ������ž���
        SphericalRadius = math_libray::Clamp(SphericalRadius, 1.0f, 300.0f);
    }
}

// +1��ǰ��-1���
void CCamera::MoveForward(float InValue)
{
    if (CameraType == ECameraType::CameraRoaming)
    {
        XMFLOAT3 fPosition = TransformationComponent->GetPosition();
        XMFLOAT3 fLookat = TransformationComponent->GetLookatVector();

        XMVECTOR AmountMovement = XMVectorReplicate(InValue * 1.0f);
        XMVECTOR Lookat = XMLoadFloat3(&fLookat);
        XMVECTOR Position = XMLoadFloat3(&fPosition);

        XMStoreFloat3(&fPosition, XMVectorMultiplyAdd(AmountMovement, Lookat, Position));

        TransformationComponent->SetPosition(fPosition);
    }
}
// +1���ң�-1����
void CCamera::MoveRight(float InValue)
{
    if (CameraType == ECameraType::CameraRoaming)
    {
        XMFLOAT3 fPosition = TransformationComponent->GetPosition();
        XMFLOAT3 fRight = TransformationComponent->GetRightVector();

        XMVECTOR AmountMovement = XMVectorReplicate(InValue * 1.0f);
        XMVECTOR Right = XMLoadFloat3(&fRight);
        XMVECTOR Position = XMLoadFloat3(&fPosition);

        XMStoreFloat3(&fPosition, XMVectorMultiplyAdd(AmountMovement, Right, Position));

        TransformationComponent->SetPosition(fPosition);
    }
}

void CCamera::RotateAroundRAxis(float InRotateDegrees)
{
    // ��ȡRUL(�����xyz��)
    XMFLOAT3 UpVector = TransformationComponent->GetUpVector();
    XMFLOAT3 LookatVector = TransformationComponent->GetLookatVector();

    // �������RightVectorΪ����תasd
    XMMATRIX RotationR = XMMatrixRotationAxis(XMLoadFloat3(&TransformationComponent->GetRightVector()), InRotateDegrees);


    // �����yz����x����ת
    XMStoreFloat3(&TransformationComponent->GetUpVector(), XMVector3TransformNormal(XMLoadFloat3(&UpVector), RotationR));
    XMStoreFloat3(&TransformationComponent->GetLookatVector(), XMVector3TransformNormal(XMLoadFloat3(&LookatVector), RotationR));
}

void CCamera::RotateAroundYAxis(float InRotateDegrees)
{
    // ��ȡRUL(�����xyz��)
    XMFLOAT3 RightVector = TransformationComponent->GetRightVector();
    XMFLOAT3 UpVector = TransformationComponent->GetUpVector();
    XMFLOAT3 LookatVector = TransformationComponent->GetLookatVector();

    // ��ȡ������ռ�Y����ת����ת����
    XMMATRIX RotationY = XMMatrixRotationY(InRotateDegrees);

    // �������������������ռ�Y����ת
    XMStoreFloat3(&TransformationComponent->GetRightVector(),XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationY));
    XMStoreFloat3(&TransformationComponent->GetUpVector(), XMVector3TransformNormal(XMLoadFloat3(&UpVector), RotationY));
    XMStoreFloat3(&TransformationComponent->GetLookatVector(), XMVector3TransformNormal(XMLoadFloat3(&LookatVector), RotationY));
}
