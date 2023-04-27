#pragma once
/* ����� */

#include "Viewport/Viewport.h"
#include "CoreObject/CoreMinimalObject.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"
#include "../Interface/DirectXDeviceInterface.h"

enum ECameraType;
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
    virtual void OnMouseWheel(int X, int Y,float InDelta);

    virtual void MoveForward(float InValue);
    virtual void MoveRight(float InValue);

protected:
    // ������ת����RightVector��ת
    virtual void RotateAroundRAxis(float InRotateDegrees);
    // ������ת��������ռ�Y����ת
    virtual void RotateAroundYAxis(float InRotateDegrees);
public:
    FORCEINLINE CInputComponent* GetInputComponent() { return InputComponent; }
    FORCEINLINE CTransformationComponent* GetTransformationComponent() { return TransformationComponent; }

protected:
    // �������
    POINT LastMousePosition;  // ��һ֡���λ��
    bool bLeftMouseDown;      // �������Ƿ���
    float MouseSensitivity;  // ������
    ECameraType CameraType;  // �����״̬

    // ��������ת���ѿ�������
    float SphericalRadius;      // ����뾶
    float theta;                // �Ͻ�
    float beta;                 // �½�
};
