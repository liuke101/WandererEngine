#pragma once
#include "../../Core/Engine.h"

// 鼠标操作多播委托
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaputureOnMouseDelegates, void, int, int);
DEFINITION_MULTICAST_SINGLE_DELEGATE(FCaputureOnMouseWheelDelegates, void, int, int,float);

// 鼠标按下
extern FCaputureOnMouseDelegates MouseDownDelegates;
// 鼠标松开
extern FCaputureOnMouseDelegates MouseUpDelegates;
// 鼠标移动
extern FCaputureOnMouseDelegates MouseMoveDelegates;
// 鼠标滚轮
extern FCaputureOnMouseWheelDelegates MouseWheelDelegates;