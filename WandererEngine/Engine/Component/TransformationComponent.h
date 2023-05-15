#pragma once
#include "Core/Component.h"
// �ƶ���� 
class CTransformationComponent : public CComponent
{
public:
    CTransformationComponent();

    virtual void SetPosition(const XMFLOAT3& InNewPosition);
    virtual void SetRotation(const fvector_3d& InNewRotation);
    virtual void SetScale(const fvector_3d& InNewScale);

    virtual void SetRightVector(const XMFLOAT3& InRightVector);
    virtual void SetUpVector(const XMFLOAT3& InUpVector);
    virtual void SetLookatVector(const XMFLOAT3& InLookatVector);

    XMFLOAT3& GetPosition()  { return Position; }
    fvector_3d GetRotation() const { return fvector_3d(Rotation.x, Rotation.y, Rotation.z); }
    fvector_3d GetScale() const { return fvector_3d(Scale.x, Scale.y, Scale.z); }

    XMFLOAT3& GetRightVector()  { return RightVector; }
    XMFLOAT3& GetUpVector()  { return UpVector; }
    XMFLOAT3& GetLookatVector()  { return LookatVector; }

    // ����Right Up Lookat ��������Ϊ��ת����ǰ����
    void CalcRULVector();

    // ����Posλ���������Ӹ�����Ϊƽ�ƾ��������
    fvector_3d CalcNegativePosVector();
    void CalcNegativePosVector(fvector_3d& InPos3);
private:
    XMFLOAT3 Position;      // λ��
    XMFLOAT3 Rotation;      // ��ת
    XMFLOAT3 Scale;         // ����

    XMFLOAT3 RightVector;   // ���������������x��
    XMFLOAT3 UpVector;      // ���������������y��
    XMFLOAT3 LookatVector;  // �۲췽��,�����ָ��object�ķ��򣬼������z��
};