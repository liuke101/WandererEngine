#pragma once
#include "Core/Component.h"
// 移动组件 
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

    // 计算Right Up Lookat 向量，作为旋转矩阵前三列
    void CalcLRUVector();

    // 计算Pos位移向量，加负号作为平移矩阵第四行
    fvector_3d CalcNegativePosVecotr();
    void CalcNegativePosVecotr(fvector_3d& InPos3);
private:
    XMFLOAT3 Position;      // 摄像机的位置
    XMFLOAT3 RightVector;   // 左向量
    XMFLOAT3 UpVector;      // 上向量
    XMFLOAT3 LookatVector;  // 观察方向,摄像机指向object的方向
};