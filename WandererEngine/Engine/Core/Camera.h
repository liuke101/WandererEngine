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

public:
    CCamera();

    virtual void BeginInit();   
    virtual void Tick(float DeltaTime);

    virtual void ExecuteKeyboard(const FInputKey&);
public:
    FORCEINLINE CInputComponent* GetInputComponent() { return InputComponent; }
    FORCEINLINE CTransformationComponent* GetTransformationComponent() { return TransformationComponent; }
};
