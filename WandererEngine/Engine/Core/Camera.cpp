#include "Camera.h"
#include "CameraType.h"
#include "../Component/InputComponent.h"
#include "../Component/TransformationComponent.h"

GCamera::GCamera()
    :GActorObject()
{
    InputComponent = CreateObject<CInputComponent>(new CInputComponent());
    MouseSensitivity = 0.5f;
    CameraType = ECameraType::CameraRoaming;
    SphericalRadius = 10.0f;
    alpha = XM_PI;
    beta = XM_PI;
}

void GCamera::BeginInit()
{
    // ��ʼ��͸��ͶӰ����
    ViewportInit();

    // �󶨰���
    InputComponent->KeyboardDelegate.Bind(this, &GCamera::ExecuteKeyboard);
    InputComponent->OnMouseButtonDownDelegate.Bind(this, &GCamera::OnMouseButtonDown);
    InputComponent->OnMouseButtonUpDelegate.Bind(this, &GCamera::OnMouseButtonUp);
    InputComponent->OnMouseMoveDelegate.Bind(this, &GCamera::OnMouseMove);
    InputComponent->OnMouseWheelDelegate.Bind(this, &GCamera::OnMouseWheel);
}

void GCamera::Tick(float DeltaTime)
{
    BuildViewMatrix(DeltaTime);
}

void GCamera::ExecuteKeyboard(const FInputKey& InputKey)
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
        CameraType == ObservationObject;
        Engine_Log("Q�л��۲����ģʽ")
    }
    // ��W�л��������ģʽ
    else if (InputKey.KeyName == "E")
    {
        CameraType == CameraRoaming;
        Engine_Log("W�л��������ģʽ")
    }
}

void GCamera::BuildViewMatrix(float DeltaTime)
{
    switch (CameraType)
    {
        case CameraRoaming:
        {
            // �����۲�任����
            // ���ǿ�����������ģ�lookatֻ�ᳯ���������ǰ��
            GetTransformationComponent()->CalcRULVector();

            fvector_3d V3;
            GetTransformationComponent()->CalcNegativePosVector(V3);

            XMFLOAT3 LookatVector = GetTransformationComponent()->GetLookatVector();
            XMFLOAT3 RightVector = GetTransformationComponent()->GetRightVector();
            XMFLOAT3 UpVector = GetTransformationComponent()->GetUpVector();

            ViewMatrix = {
                RightVector.x, UpVector.x, LookatVector.x, 0.0f,
                RightVector.y, UpVector.y, LookatVector.y, 0.0f,
                RightVector.z, UpVector.z, LookatVector.z, 0.0f,
                V3.x,          V3.y,       V3.z,           1.f };

            break;
        }

        case ObservationObject:
        {
            // ��������ת���ѿ�������
            // ���ǿ�������ģ�lookatֻ�ῴ�����壬����ʹ���������꣬����Χ������۲졣
            XMFLOAT3 &CameraPos = GetTransformationComponent()->GetPosition();

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

void GCamera::OnMouseButtonDown(int X, int Y)
{
    bLeftMouseDown = true;
    LastMousePosition.x = X;
    LastMousePosition.y = Y;
    SetCapture(GetMainWindowsHandle()); // ��겶��
    Engine_Log("�Ҽ�����")
}

void GCamera::OnMouseButtonUp(int X, int Y)
{
    bLeftMouseDown = false;
    ReleaseCapture();           // �ͷ���겶��
    LastMousePosition.x = X;
    LastMousePosition.y = Y;
    Engine_Log("�Ҽ�̧��")
}

void GCamera::OnMouseMove(int X, int Y)
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
                alpha += YRadians;
                beta += XRadians;
                //alpha = math_libray::Clamp(alpha, 0.0f, XM_2PI);    // ���������ӽ�ƫ����
                break;
            }
        }
        Engine_Log("��ס��ת")
    }
    LastMousePosition.x = X;
    LastMousePosition.y = Y;
}

void GCamera::OnMouseWheel(int X, int Y, float InDelta)
{
    if(CameraType==ObservationObject)
    {
        SphericalRadius += (InDelta / 100.f);

        // ���ƹ������ž���
        SphericalRadius = math_libray::Clamp(SphericalRadius, 1.0f, 300.0f);

        Engine_Log("��������")
    }
}

// +1��ǰ��-1���
void GCamera::MoveForward(float InValue)
{
    if (CameraType == CameraRoaming)
    {
        XMFLOAT3 fPosition = GetTransformationComponent()->GetPosition();
        XMFLOAT3 fLookat = GetTransformationComponent()->GetLookatVector();

        XMVECTOR AmountMovement = XMVectorReplicate(InValue * 1.0f);
        XMVECTOR Lookat = XMLoadFloat3(&fLookat);
        XMVECTOR Position = XMLoadFloat3(&fPosition);

        XMStoreFloat3(&fPosition, XMVectorMultiplyAdd(AmountMovement, Lookat, Position));

        GetTransformationComponent()->SetPosition(fPosition);
        Engine_Log("ǰ��")
    }
}
// +1���ң�-1����
void GCamera::MoveRight(float InValue)
{
    if (CameraType == CameraRoaming)
    {
        XMFLOAT3 fPosition = GetTransformationComponent()->GetPosition();
        XMFLOAT3 fRight = GetTransformationComponent()->GetRightVector();

        XMVECTOR AmountMovement = XMVectorReplicate(InValue * 1.0f);
        XMVECTOR Right = XMLoadFloat3(&fRight);
        XMVECTOR Position = XMLoadFloat3(&fPosition);

        XMStoreFloat3(&fPosition, XMVectorMultiplyAdd(AmountMovement, Right, Position));

        GetTransformationComponent()->SetPosition(fPosition);
        Engine_Log("����")
    }

}

void GCamera::RotateAroundRAxis(float InRotateDegrees)
{
    // ��ȡRUL(�����xyz��)
    XMFLOAT3 UpVector = GetTransformationComponent()->GetUpVector();
    XMFLOAT3 LookatVector = GetTransformationComponent()->GetLookatVector();

    // �������RightVectorΪ����תasd
    XMMATRIX RotationR = XMMatrixRotationAxis(XMLoadFloat3(&GetTransformationComponent()->GetRightVector()), InRotateDegrees);


    // �����yz����x����ת
    XMStoreFloat3(&GetTransformationComponent()->GetUpVector(), XMVector3TransformNormal(XMLoadFloat3(&UpVector), RotationR));
    XMStoreFloat3(&GetTransformationComponent()->GetLookatVector(), XMVector3TransformNormal(XMLoadFloat3(&LookatVector), RotationR));
}

void GCamera::RotateAroundYAxis(float InRotateDegrees)
{
    // ��ȡRUL(�����xyz��)
    XMFLOAT3 RightVector = GetTransformationComponent()->GetRightVector();
    XMFLOAT3 UpVector = GetTransformationComponent()->GetUpVector();
    XMFLOAT3 LookatVector = GetTransformationComponent()->GetLookatVector();

    // ��ȡ������ռ�Y����ת����ת����
    XMMATRIX RotationY = XMMatrixRotationY(InRotateDegrees);

    // �������������������ռ�Y����ת
    XMStoreFloat3(&GetTransformationComponent()->GetRightVector(),XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationY));
    XMStoreFloat3(&GetTransformationComponent()->GetUpVector(), XMVector3TransformNormal(XMLoadFloat3(&UpVector), RotationY));
    XMStoreFloat3(&GetTransformationComponent()->GetLookatVector(), XMVector3TransformNormal(XMLoadFloat3(&LookatVector), RotationY));
}


