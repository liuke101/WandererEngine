#pragma once
#include "Core/Component.h"
// 移动组件 
class CTransformationComponent : public CComponent
{
public:
    CTransformationComponent();

    void SetPosition(const XMFLOAT3& InNewPosition);
    void SetRotation(const fvector_3d& InNewRotation);
    void SetScale(const fvector_3d& InNewScale);

    void SetRightVector(const XMFLOAT3& InRightVector);
    void SetUpVector(const XMFLOAT3& InUpVector);
    void SetLookatVector(const XMFLOAT3& InLookatVector);

    XMFLOAT3& GetPosition()  { return Position; }
    fvector_3d GetRotation() const { return fvector_3d(Rotation.x, Rotation.y, Rotation.z); }
    fvector_3d GetScale() const { return fvector_3d(Scale.x, Scale.y, Scale.z); }

    XMFLOAT3& GetRightVector()  { return RightVector; }
    XMFLOAT3& GetUpVector()  { return UpVector; }
    XMFLOAT3& GetLookatVector()  { return LookatVector; }

    // 计算Right Up Lookat 向量，作为旋转矩阵前三列
    void CalcRULVector();

    // 计算Pos位移向量，加负号作为平移矩阵第四行
    fvector_3d CalcNegativePosVector();
    void CalcNegativePosVector(fvector_3d& InPos3);
private:
    XMFLOAT3 Position;      // 位置
    XMFLOAT3 Rotation;      // 旋转
    XMFLOAT3 Scale;         // 缩放

    XMFLOAT3 RightVector;   // 左向量，即相机的x轴
    XMFLOAT3 UpVector;      // 上向量，即相机的y轴
    XMFLOAT3 LookatVector;  // 观察方向,摄像机指向object的方向，即相机的z轴
};