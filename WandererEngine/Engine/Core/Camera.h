#pragma once
#include "Viewport/Viewport.h"
#include "../Actor/Core/ActorObject.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"
#include "../Interface/DirectXDeviceInterface.h"

enum ECameraType;
struct FInputKey;
class CTransformationComponent;
class CInputComponent;


// 相机
class GCamera
    : public GActorObject
    , public FViewport
    , public IDirectXDeviceInterface
{
    CVARIABLE()
    CInputComponent* InputComponent;

public:
    GCamera();

    virtual void BeginInit();   
    virtual void Tick(float DeltaTime);

    virtual void ExecuteKeyboard(const FInputKey& InputKey);

    // 构建观察变换矩阵
    virtual void BuildViewMatrix(float DeltaTime);

    virtual void OnMouseButtonDown(int X, int Y);
    virtual void OnMouseButtonUp(int X, int Y);
    virtual void OnMouseMove(int X, int Y);
    virtual void OnMouseWheel(int X, int Y, float InDelta);

    virtual void MoveForward(float InValue);
    virtual void MoveRight(float InValue);

protected:
    // 上下旋转：绕RightVector旋转
    virtual void RotateAroundRAxis(float InRotateDegrees);
    // 左右旋转：绕世界空间Y轴旋转
    virtual void RotateAroundYAxis(float InRotateDegrees);
public:
    FORCEINLINE CInputComponent* GetInputComponent() { return InputComponent; }

protected:
    // 鼠标配置
    POINT LastMousePosition;  // 上一帧鼠标位置
    bool bLeftMouseDown;      // 检测左键是否按下
    float MouseSensitivity;  // 灵敏度
    ECameraType CameraType;  // 摄像机状态

    // 球面坐标转换笛卡尔坐标
    float SphericalRadius;      // 球面半径
    float alpha;                // 上角
    float beta;                 // 下角
};
