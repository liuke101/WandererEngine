#include "TransformationComponent.h"

CTransformationComponent::CTransformationComponent()
    : Position(0.0f, 0.0f, 0.0f)
    , Rotation(0.0f, 0.0f, 0.0f)
    , Scale(1.0f, 1.0f, 1.0f)
    , RightVector(1.0f, 0.0f, 0.0f)
    , UpVector(0.0f, 1.0f, 0.0f)
    , LookatVector(0.0f, 0.0f, 1.0f)
{
}

void CTransformationComponent::SetPosition(const XMFLOAT3& InNewPosition)
{
    Position = InNewPosition;
}

void CTransformationComponent::SetRotation(const fvector_3d& InNewRotation)
{
    // 欧拉角
    // DirectXMath 库采用的是 ZXY 顺规(roll,pitch,yaw)
    float RollRadians = XMConvertToRadians(InNewRotation.z);
    float PitchRadians = XMConvertToRadians(InNewRotation.x);
    float YawRadians = XMConvertToRadians(InNewRotation.y);

    // 欧拉角转换成旋转矩阵
    XMMATRIX M_RollPitchYaw = XMMatrixRotationRollPitchYaw(PitchRadians,YawRadians,RollRadians);

    // 对RUL向量进行旋转
    XMStoreFloat3(&RightVector, XMVector3TransformNormal(XMLoadFloat3(&RightVector), M_RollPitchYaw));
    XMStoreFloat3(&UpVector, XMVector3TransformNormal(XMLoadFloat3(&UpVector), M_RollPitchYaw));
    XMStoreFloat3(&LookatVector, XMVector3TransformNormal(XMLoadFloat3(&LookatVector), M_RollPitchYaw));
}

void CTransformationComponent::SetScale(const fvector_3d& InNewScale)
{
    Scale.x = InNewScale.x;
    Scale.y = InNewScale.y;
    Scale.z = InNewScale.z;
}

void CTransformationComponent::SetRightVector(const XMFLOAT3& InRightVector)
{
    RightVector = InRightVector;
}

void CTransformationComponent::SetUpVector(const XMFLOAT3& InUpVector)
{
    UpVector = InUpVector;
}

void CTransformationComponent::SetLookatVector(const XMFLOAT3& InLookatVector)
{
    LookatVector = InLookatVector;
}

void CTransformationComponent::CalcRULVector()
{
    XMVECTOR Lookat = XMLoadFloat3(&LookatVector);
    XMVECTOR Right = XMLoadFloat3(&RightVector);
    XMVECTOR Up = XMLoadFloat3(&UpVector);

    Lookat = XMVector3Normalize(Lookat);
    Up = XMVector3Normalize(XMVector3Cross(Lookat, Right));
    Right = XMVector3Cross(Up, Lookat);

    XMStoreFloat3(&RightVector, Right);
    XMStoreFloat3(&UpVector, Up);
    XMStoreFloat3(&LookatVector, Lookat);
}

fvector_3d CTransformationComponent::CalcNegativePosVector()
{
    fvector_3d FVector3;
    CalcNegativePosVector(FVector3);
    return FVector3;
}

void CTransformationComponent::CalcNegativePosVector(fvector_3d& InPos3)
{
    XMVECTOR Right = XMLoadFloat3(&RightVector);
    XMVECTOR Up = XMLoadFloat3(&UpVector);
    XMVECTOR Lookat = XMLoadFloat3(&LookatVector);

    XMVECTOR NewPosition = XMLoadFloat3(&Position);

    // 观察变换矩阵第四行的分量就是向量点积的结果，平移矩阵第四行为负值
    InPos3.x = XMVectorGetX(XMVector3Dot(-NewPosition, Right));
    InPos3.y = XMVectorGetX(XMVector3Dot(-NewPosition, Up));
    InPos3.z = XMVectorGetX(XMVector3Dot(-NewPosition, Lookat));
}
