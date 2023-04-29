#pragma once
#include "Core/Component.h"
// �ƶ���� 
class CTransformationComponent : public CComponent
{
public:
    CTransformationComponent();

    void SetPosition(const XMFLOAT3& InNewPosition);
    void SetRotation(const fvector_3d& InNewRotation);
    void SetScale(const fvector_3d& InNewScale);

    void SetRightVector(const XMFLOAT3& InRightVector);
    void SetUpVector(const XMFLOAT3& InUpVector);
    void SetLookatVector(const XMFLOAT3& InNewLookatVector);

    XMFLOAT3& GetPosition()  { return Position; }
    fvector_3d GetRotation() const { return fvector_3d(Rotation.x, Rotation.y, Rotation.z); }
    fvector_3d GetScale() const { return fvector_3d(Scale.x, Scale.y, Scale.z); }

    XMFLOAT3& GetRightVector()  { return RightVector; }
    XMFLOAT3& GetUpVector()  { return UpVector; }
    XMFLOAT3& GetLookatVector()  { return LookatVector; }

    // ����Right Up Lookat ��������Ϊ��ת����ǰ����
    void CalcLRUVector();

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