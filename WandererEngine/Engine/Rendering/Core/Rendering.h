#pragma once
/* 渲染接口 */
// 需要渲染的地方继承该接口即可
#include "../../Core/Engine.h"

class IRenderingInterface
{
    friend class FWindowsEngine;
public:
    IRenderingInterface();
    virtual ~IRenderingInterface();

    virtual void Init();

    virtual void Draw(float DeltaTime);

    // 重载运算符，判断guid是否相等
    bool operator==(const IRenderingInterface &InOther)  
    {
        return guid_equal(&Guid, &InOther.Guid);
    }

private:
    static vector<IRenderingInterface*> RenderingInterface;
    simple_c_guid Guid;
};
