#pragma once
#include "CoreObject/CoreMinimalObject.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"

class CCamera;
struct FInputKey;
class CInputComponent;
class CTransformationComponent;

class CWorld : public CCoreMinimalObject
{
public:
    CWorld();

    CCamera* GetCamra() { return Camera; }

protected:
    CVARIABLE()
    CCamera* Camera;
};


