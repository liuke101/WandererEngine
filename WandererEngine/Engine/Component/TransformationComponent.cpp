#include "TransformationComponent.h"

CTransformationComponent::CTransformationComponent()
    : Position(0.0f, 0.0f, 0.0f)
    , RightVector(1.0f,0.0f,0.0f)
    , UpVector(0.0f,1.0f,0.0f)
    , LookatVector(0.0f, 0.0f, 1.0f)
{
}

void CTransformationComponent::SetPosition(const XMFLOAT3 InNewPosition)
{
    Position = InNewPosition;
}

void CTransformationComponent::SetRightVector(const XMFLOAT3 InRightVector)
{
    RightVector = InRightVector;
}

void CTransformationComponent::SetUpVector(const XMFLOAT3 InUpVector)
{
    UpVector = InUpVector;
}

void CTransformationComponent::SetLookatVector(const XMFLOAT3 InNewLookatVector)
{
    LookatVector = InNewLookatVector;
}

void CTransformationComponent::CalcLRUVector()
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

fvector_3d CTransformationComponent::CalcNegativePosVecotr()
{
    fvector_3d FVector3;
    CalcNegativePosVecotr();
    return FVector3;
}

void CTransformationComponent::CalcNegativePosVecotr(fvector_3d& InPos3)
{
    XMVECTOR Right = XMLoadFloat3(&RightVector);
    XMVECTOR Up = XMLoadFloat3(&UpVector);
    XMVECTOR Lookat = XMLoadFloat3(&LookatVector);

    XMVECTOR NewPosition = XMLoadFloat3(&Position);

    // �۲�任��������еķ���������������Ľ����ƽ�ƾ��������Ϊ��ֵ
    InPos3.x = XMVectorGetX(XMVector3Dot(-NewPosition, Right));
    InPos3.y = -XMVectorGetX(XMVector3Dot(-NewPosition, Up));
    InPos3.z = -XMVectorGetX(XMVector3Dot(-NewPosition, Lookat));
}
