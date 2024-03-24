#pragma once
// GUID��ȫ��Ψһ��ʶ����Globally Unique Identifier��
// ���ã���������Ͷ����Ψһ�ԣ���ֹ�ظ� 
#include "../simple_c_core/simple_core_minimal.h"

_CRT_BEGIN_C_HEADER
// guid���ݽṹ����������Ϊsimple_c_guid
typedef struct
{
    unsigned int a;
    unsigned int b;
    unsigned int c;
    unsigned int d;
}simple_c_guid;

// ����guid
void create_guid(simple_c_guid* c_guid);

// �����ַ�����ʽ��guid
void create_guid_str(char* c_guid);

// �ж�guid�Ƿ������壺guidȫ��Ϊ0ʱ������
bool is_guid_valid(simple_c_guid* c_guid);

// �ж��ַ���guid�Ƿ�������
bool is_guid_valid_str(const char* c_guid);

// ��һ��guid����guid����
void normalization_guid(simple_c_guid* c_guid);

// guidת�ַ���
void guid_to_string(char *buf, const simple_c_guid * c_guid);

// �ַ���תguid
void string_to_guid(const char* buf, simple_c_guid* c_guid);

// guid����ж�
bool guid_equal(const simple_c_guid * c_guid_a, const simple_c_guid * c_guid_b);

// �ַ�����ʽ��guid����ж� 
bool guid_equal_str(const char *guid_string, const simple_c_guid * c_guid);
 _CRT_END_C_HEADER