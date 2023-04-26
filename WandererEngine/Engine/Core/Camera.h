#pragma once
/* ����� */

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

    // �����۲�任����
    virtual void BuildViewMatrix(float DeltaTime);

    virtual void OnMouseButtonDown(int X, int Y);
    virtual void OnMouseButtonUp(int X, int Y);
    virtual void OnMouseMove(int X, int Y);
    virtual void MoveForward(float InValue);
    virtual void MoveRight(float InValue);

protected:
    // ��Y����ת
    virtual void RotateAroundYAxis(float InRotateDegrees);
    // ��Z����ת
    virtual void RotateAroundZAxis(float InRotateDegrees);
public:
    FORCEINLINE CInputComponent* GetInputComponent() { return InputComponent; }
    FORCEINLINE CTransformationComponent* GetTransformationComponent() { return TransformationComponent; }

protected:
    POINT LastMousePosition;  
    bool bLeftMouseDown;      // ����Ƿ���

    // �������
    float MouseSensitivity;  // ������
};
