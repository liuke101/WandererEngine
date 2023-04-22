#pragma once
#include "../../Core/Engine.h"

// 鼠标操作多播委托
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaputureOnMouseDelegates, void, int, int);


extern FCaputureOnMouseDelegates MouseDownDelegates;
extern FCaputureOnMouseDelegates MouseUpDelegates;
extern FCaputureOnMouseDelegates MouseMoveDelegates;