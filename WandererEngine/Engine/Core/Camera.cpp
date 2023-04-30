#include "Camera.h"

#include "ECameraType.h"
#include "../Component/InputComponent.h"
#include "../Component/TransformationComponent.h"

CCamera::CCamera()
{
    InputComponent = CreateObject<CInputComponent>(new CInputComponent());
    TransformationComponent = CreateObject<CTransformationComponent>(new CTransformationComponent());

    MouseSensitivity = 0.001f;
    CameraType = ECameraType::CameraRoaming;
    SphericalRadius = 10.0f;
    theta = XM_PI;
    beta = XM_PI;
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
    // 按Q切换观察对象模式
    else if (InputKey.KeyName == "Q")
    {
        CameraType == ECameraType::ObservationObject;
        Engine_Log("Q切换观察对象模式");
    }
    // 按W切换相机漫游模式
    else if (InputKey.KeyName == "E")
    {
        CameraType == ECameraType::CameraRoaming;
        Engine_Log("W切换相机漫游模式");
    }
}

void CCamera::BuildViewMatrix(float DeltaTime)
{
    switch (CameraType)
    {
        case ECameraType::CameraRoaming:
        {
            // 构建观察变换矩阵
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
            // 球面坐标转换笛卡尔坐标
            XMFLOAT3 &CameraPos = TransformationComponent->GetPosition();
            CameraPos.x = SphericalRadius * sinf(theta) * cos(beta);
            CameraPos.y = SphericalRadius * sinf(theta) * sin(beta);
            CameraPos.z = SphericalRadius * cos(theta);

            XMVECTOR Pos = XMVectorSet(CameraPos.x, CameraPos.y, CameraPos.z, 1.0f);
            XMVECTOR ViewTarget = XMVectorZero();
            XMVECTOR ViewUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

            XMMATRIX ViewLookat = XMMatrixLookAtLH(Pos, ViewTarget, ViewUp);
            XMStoreFloat4x4(&ViewMatrix, ViewLookat);

            break;
        }

        default:
            break;
    }

}

void CCamera::OnMouseButtonDown(int X, int Y)
{
    bLeftMouseDown = true;
    LastMousePosition.x = X;
    LastMousePosition.y = Y;

    SetCapture(GetMainWindowsHandle()); // 鼠标捕获

    Engine_Log("右键按下");
    
}

void CCamera::OnMouseButtonUp(int X, int Y)
{
    bLeftMouseDown = false;

    ReleaseCapture();           // 释放鼠标捕获
    LastMousePosition.x = X;
    LastMousePosition.y = Y;

    Engine_Log("右键抬起");
}

void CCamera::OnMouseMove(int X, int Y)
{
    if (bLeftMouseDown)
    {
        // 根据鼠标的移动距离计算旋转角度
        float XRadians = XMConvertToRadians(static_cast<float>(X - LastMousePosition.x) * MouseSensitivity);
        float YRadians = XMConvertToRadians(static_cast<float>(Y - LastMousePosition.x) * MouseSensitivity);

        switch (CameraType)
        {
            case CameraRoaming:
            {
                RotateAroundRAxis(XRadians);
                RotateAroundYAxis(YRadians);
                break;
            }
            case ObservationObject:
            {
                theta += ( - YRadians);
                beta += XRadians;
                theta = math_libray::Clamp(theta, 0.0f, XM_2PI);    // 限制上下视角偏移量
                break;
            }
        }
        LastMousePosition.x = X;
        LastMousePosition.y = Y;

        Engine_Log("右键按住移动");
    }
}

void CCamera::OnMouseWheel(int X, int Y, float InDelta)
{
    if(CameraType==ECameraType::ObservationObject)
    {
        SphericalRadius += (InDelta / 100.f);

        // 限制滚轮缩放距离
        SphericalRadius = math_libray::Clamp(SphericalRadius, 7.0f, 40.0f);
        Engine_Log("滚轮缩放");
    }
}

// +1向前，-1向后
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
        Engine_Log("前进后退");
    }
}
// +1向右，-1向左
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
        Engine_Log("左右");
    }
}

void CCamera::RotateAroundRAxis(float InRotateDegrees)
{
    // 获取RUL(相机的xyz轴)
    XMFLOAT3 UpVector = TransformationComponent->GetUpVector();
    XMFLOAT3 LookatVector = TransformationComponent->GetLookatVector();

    // 以相机的x轴为轴旋转
    XMMATRIX RotationR = XMMatrixRotationAxis(XMLoadFloat3(&TransformationComponent->GetRightVector()), InRotateDegrees);

    // 相机的yz轴绕x轴旋转
    XMStoreFloat3(&TransformationComponent->GetUpVector(), XMVector3TransformNormal(XMLoadFloat3(&UpVector), RotationR));
    XMStoreFloat3(&TransformationComponent->GetLookatVector(), XMVector3TransformNormal(XMLoadFloat3(&LookatVector), RotationR));
}

void CCamera::RotateAroundYAxis(float InRotateDegrees)
{
    // 获取RUL(相机的xyz轴)
    XMFLOAT3 RightVector = TransformationComponent->GetRightVector();
    XMFLOAT3 UpVector = TransformationComponent->GetUpVector();
    XMFLOAT3 LookatVector = TransformationComponent->GetLookatVector();

    // 获取绕世界空间Y轴旋转的旋转矩阵
    XMMATRIX RotationY = XMMatrixRotationY(InRotateDegrees);

    // 计算相机各个轴绕世界空间Y轴旋转
    XMStoreFloat3(&TransformationComponent->GetRightVector(),XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationY));
    XMStoreFloat3(&TransformationComponent->GetUpVector(), XMVector3TransformNormal(XMLoadFloat3(&UpVector), RotationY));
    XMStoreFloat3(&TransformationComponent->GetLookatVector(), XMVector3TransformNormal(XMLoadFloat3(&LookatVector), RotationY));
}


