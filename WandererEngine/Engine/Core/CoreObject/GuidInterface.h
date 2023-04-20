#pragma once
#include "../../EngineMinimal.h"
/* 提供一个对象的Guid */

class IGuidInterface
{
public:
    IGuidInterface();

    // 获取Guid
    simple_c_guid GetGuid() { return Guid; }

    // 重载运算符，判断guid是否相等
    bool operator==(const IGuidInterface& InOther)
    {
        return guid_equal(&Guid, &InOther.Guid);
    }
private:
    simple_c_guid Guid;
};