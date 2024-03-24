#pragma once
// GUID：全局唯一标识符（Globally Unique Identifier）
// 作用：描述程序和对象的唯一性，防止重复 
#include "../simple_c_core/simple_core_minimal.h"

_CRT_BEGIN_C_HEADER
// guid数据结构，变量类型为simple_c_guid
typedef struct
{
    unsigned int a;
    unsigned int b;
    unsigned int c;
    unsigned int d;
}simple_c_guid;

// 创建guid
void create_guid(simple_c_guid* c_guid);

// 创建字符串格式的guid
void create_guid_str(char* c_guid);

// 判断guid是否有意义：guid全部为0时无意义
bool is_guid_valid(simple_c_guid* c_guid);

// 判断字符串guid是否有意义
bool is_guid_valid_str(const char* c_guid);

// 归一化guid：将guid清零
void normalization_guid(simple_c_guid* c_guid);

// guid转字符串
void guid_to_string(char *buf, const simple_c_guid * c_guid);

// 字符串转guid
void string_to_guid(const char* buf, simple_c_guid* c_guid);

// guid相等判定
bool guid_equal(const simple_c_guid * c_guid_a, const simple_c_guid * c_guid_b);

// 字符串格式的guid相等判定 
bool guid_equal_str(const char *guid_string, const simple_c_guid * c_guid);
 _CRT_END_C_HEADER