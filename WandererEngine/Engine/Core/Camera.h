#pragma once
/* 摄像机 */

#include "Viewport/Viewport.h"
#include "CoreObject/CoreMinimalObject.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"
#include "../Interface/DirectXDeviceInterface.h"

struct FInputKey;
class CInputComponent;
class CTransformationComponent;

class CCamera : public CCoreMinimalObject, public FViewport,public IDirectXDeviceInterface
{

    CVARIABLE()
    CTransformationComponent* TransformationComponent;

    CVARIABLE()
    CInputComponent* InputComponent;

public:
    CCamera();

    virtual void BeginInit();   
    virtual void Tick(float DeltaTime);

    virtual void ExecuteKeyboard(const FInputKey& InputKey);

    // 构建观察变换矩阵
    virtual void BuildViewMatrix(float DeltaTime);

    virtual void OnMouseButtonDown(int X, int Y);
    virtual void OnMouseButtonUp(int X, int Y);
    virtual void OnMouseMove(int X, int Y);
    virtual void MoveForward(float InValue);
    virtual void MoveRight(float InValue);

protected:
    // 绕Y轴旋转
    virtual void RotateAroundYAxis(float InRotateDegrees);
    // 绕Z轴旋转
    virtual void RotateAroundZAxis(float InRotateDegrees);
public:
    FORCEINLINE CInputComponent* GetInputComponent() { return InputComponent; }
    FORCEINLINE CTransformationComponent* GetTransformationComponent() { return TransformationComponent; }

protected:
    POINT LastMousePosition;  
    bool bLeftMouseDown;      // 左键是否按下

    // 鼠标配置
    float MouseSensitivity;  // 灵敏度
};
