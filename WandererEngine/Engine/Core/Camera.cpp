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
    // 初始化透视投影矩阵
    ViewportInit();

    // 绑定按键
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
    // 按Q切换观察对象模式
    else if (InputKey.KeyName == "Q")
    {
        CameraType = ObservationObject;  
        Engine_Log("Q切换观察对象模式")
    }
    // 按W切换相机漫游模式
    else if (InputKey.KeyName == "E")
    {
        CameraType = CameraRoaming;
        Engine_Log("W切换相机漫游模式")
    }
}

void GCamera::BuildViewMatrix(float DeltaTime)
{
    switch (CameraType)
    {
        case CameraRoaming:
        {
            // 构建观察变换矩阵
            // 这是看向整个世界的，lookat只会朝向摄像机的前方
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
            // 球面坐标转换笛卡尔坐标
            // 这是看向物体的，lookat只会看向物体，所以使用球面坐标，可以围绕物体观察。
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
    SetCapture(GetMainWindowsHandle()); // 鼠标捕获
    Engine_Log("右键按下")
}

void GCamera::OnMouseButtonUp(int X, int Y)
{
    bLeftMouseDown = false;
    ReleaseCapture();           // 释放鼠标捕获
    LastMousePosition.x = X;
    LastMousePosition.y = Y;
    Engine_Log("右键抬起")
}

void GCamera::OnMouseMove(int X, int Y)
{
    if (bLeftMouseDown)
    {
        // 根据鼠标的移动距离计算旋转角度
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
                beta += (-XRadians);
                //alpha = math_libray::Clamp(alpha, 0.0f, XM_2PI);    // 限制上下视角偏移量
                break;
            }
        }
        Engine_Log("按住旋转")
    }
    LastMousePosition.x = X;
    LastMousePosition.y = Y;
}

void GCamera::OnMouseWheel(int X, int Y, float InDelta)
{
    if(CameraType==ObservationObject)
    {
        SphericalRadius += (InDelta / 100.f);

        // 限制滚轮缩放距离
        SphericalRadius = math_libray::Clamp(SphericalRadius, 1.0f, 300.0f);

        Engine_Log("滚轮缩放")
    }
}

// +1向前，-1向后
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
        Engine_Log("前后")
    }
}
// +1向右，-1向左
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
        Engine_Log("左右")
    }

}

void GCamera::RotateAroundRAxis(float InRotateDegrees)
{
    // 获取RUL(相机的xyz轴)
    XMFLOAT3 UpVector = GetTransformationComponent()->GetUpVector();
    XMFLOAT3 LookatVector = GetTransformationComponent()->GetLookatVector();

    // 以相机的RightVector为轴旋转asd
    XMMATRIX RotationR = XMMatrixRotationAxis(XMLoadFloat3(&GetTransformationComponent()->GetRightVector()), InRotateDegrees);


    // 相机的yz轴绕x轴旋转
    XMStoreFloat3(&GetTransformationComponent()->GetUpVector(), XMVector3TransformNormal(XMLoadFloat3(&UpVector), RotationR));
    XMStoreFloat3(&GetTransformationComponent()->GetLookatVector(), XMVector3TransformNormal(XMLoadFloat3(&LookatVector), RotationR));
}

void GCamera::RotateAroundYAxis(float InRotateDegrees)
{
    // 获取RUL(相机的xyz轴)
    XMFLOAT3 RightVector = GetTransformationComponent()->GetRightVector();
    XMFLOAT3 UpVector = GetTransformationComponent()->GetUpVector();
    XMFLOAT3 LookatVector = GetTransformationComponent()->GetLookatVector();

    // 获取绕世界空间Y轴旋转的旋转矩阵
    XMMATRIX RotationY = XMMatrixRotationY(InRotateDegrees);

    // 计算相机各个轴绕世界空间Y轴旋转
    XMStoreFloat3(&GetTransformationComponent()->GetRightVector(),XMVector3TransformNormal(XMLoadFloat3(&RightVector), RotationY));
    XMStoreFloat3(&GetTransformationComponent()->GetUpVector(), XMVector3TransformNormal(XMLoadFloat3(&UpVector), RotationY));
    XMStoreFloat3(&GetTransformationComponent()->GetLookatVector(), XMVector3TransformNormal(XMLoadFloat3(&LookatVector), RotationY));
}


