#pragma once
#include "Core/Component.h"
// �ƶ���� 
class CTransformationComponent : public CComponent
{
public:
    CTransformationComponent();

    void SetPosition(const XMFLOAT3 InNewPosition);
    
    void SetRightVector(const XMFLOAT3 InRightVector);
    void SetUpVector(const XMFLOAT3 InUpVector);
    void SetLookatVector(const XMFLOAT3 InNewLookatVector);

    XMFLOAT3 GetPosition() const { return Position; }
    XMFLOAT3 GetRightVector() const { return RightVector; }
    XMFLOAT3 GetUpVector() const { return UpVector; }
    XMFLOAT3 GetLookatVector() const { return LookatVector; }

    // ����Right Up Lookat ��������Ϊ��ת����ǰ����
    void CalcLRUVector();

    // ����Posλ���������Ӹ�����Ϊƽ�ƾ��������
    fvector_3d CalcNegativePosVecotr();
    void CalcNegativePosVecotr(fvector_3d& InPos3);
private:
    XMFLOAT3 Position;      // �������λ��
    XMFLOAT3 RightVector;   // ������
    XMFLOAT3 UpVector;      // ������
    XMFLOAT3 LookatVector;  // �۲췽��,�����ָ��object�ķ���
};