#pragma once
/* 按键输入类型 */
#include "../../EngineMinimal.h"

enum EPressState
{
    Press,      // 按下
    Release,    // 松开
};

struct FInputKey
{
    FInputKey();

    string KeyName;
    EPressState PressState;
};