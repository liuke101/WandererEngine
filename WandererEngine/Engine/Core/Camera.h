#pragma once
/* ÉãÏñ»ú */

#include "Viewport.h"
#include "CoreObject/CoreMinimalObject.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"

struct FInputKey;
class CInputComponent;
class CTransformationComponent;

class CCamera : public CCoreMinimalObject, public FViewport
{
public:
    CVARIABLE()
    CTransformationComponent* TransformationComponent;

    CVARIABLE()
    CInputComponent* InputComponent;

    POINT LastMousePosition;
public:
    CCamera();

    virtual void BeginInit();   
    virtual void Tick(float DeltaTime);

    virtual void ExecuteKeyboard(const FInputKey& InputKey);

    virtual void OnMouseButtonDown(int X, int Y);
    virtual void OnMouseButtonUp(int X, int Y);
    virtual void OnMouseMove(int X, int Y);
    virtual void MoveForward(float InValue);
    virtual void MoveRight(float InValue);
public:
    FORCEINLINE CInputComponent* GetInputComponent() { return InputComponent; }
    FORCEINLINE CTransformationComponent* GetTransformationComponent() { return TransformationComponent; }
};
