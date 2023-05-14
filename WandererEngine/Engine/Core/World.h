#pragma once
#include "CoreObject/CoreMinimalObject.h"
#include "../CodeReflection/CodeReflectionMacroTag.h"

struct FInputKey;
class CTransformationComponent;
class CInputComponent;
class GCamera;
class GActorObject;

class CWorld : public CCoreMinimalObject
{
public:
    CWorld();

    GCamera* GetCamra() { return Camera; }

	template<class T>
	T* CreateActorObject()
	{
		T* InArray = new T();
		ActorObjects.push_back(InArray);

		return InArray;
	}

protected:
    CVARIABLE()
    GCamera* Camera;

	//存储我们的Actors
	CVARIABLE()
    vector<GActorObject*> ActorObjects;
};


