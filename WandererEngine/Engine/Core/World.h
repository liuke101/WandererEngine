#pragma once
#include "CoreObject/CoreMinimalObject.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"

class GCamera;
struct FInputKey;
class CInputComponent;
class CTransformationComponent;

class CWorld : public CCoreMinimalObject
{
public:
    CWorld();

    GCamera* GetCamra() { return Camera; }

protected:
    CVARIABLE()
    GCamera* Camera;
};


