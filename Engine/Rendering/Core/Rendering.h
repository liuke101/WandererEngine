﻿#pragma once
/* 渲染接口 */
// 需要渲染的地方继承该接口即可
#include "../../Core/CoreObject/GuidInterface.h"
#include "../../Core/Engine.h"

#if defined(_WIN32)
#include "../../Platform/Windows/WindowsEngine.h"
#else
#endif

// 渲染接口 
// 需要渲染的地方继承该接口即可
class IRenderingInterface //: public IGuidInterface
{
    friend class CWindowsEngine;
public:
    IRenderingInterface();
    virtual ~IRenderingInterface();

    virtual void Init();

    virtual void PreDraw(float DeltaTime);  
    virtual void Draw(float DeltaTime);
    virtual void PostDraw(float DeltaTime);
};
