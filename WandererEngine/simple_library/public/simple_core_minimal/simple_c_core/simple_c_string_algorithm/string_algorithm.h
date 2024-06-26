#pragma once
// �ַ����㷨
#include "../simple_core_minimal.h"
#include "../../../simple_library_macro.h"

_CRT_BEGIN_C_HEADER

// �Ƴ����ַ���""
void SIMPLE_LIBRARY_API remove_string_start(char *str, char const* sub_str);
// ��ǰ�����Ƴ��ַ�''
void SIMPLE_LIBRARY_API remove_char_start(char* str, char sub_str);
// �Ӻ���ǰ�Ƴ��ַ�''
void SIMPLE_LIBRARY_API remove_char_end(char* str, char sub_str);
// �Ӻ���ǰ�Ƴ������ַ�''
void SIMPLE_LIBRARY_API remove_all_char_end(char* str, char sub_str);

//�Ӻ���ǰ�����ַ�������������
int SIMPLE_LIBRARY_API find_string_from_end(const char* str, char const* sub_str, int start_pos);

//��ǰ���������ַ�������������
int SIMPLE_LIBRARY_API find_string(const char *str, char const* sub_str,int start_pos);

//�ж��ַ����Ƿ����
bool SIMPLE_LIBRARY_API c_str_contain(const char* buff_str,const char *sub_str);

//ǰ��ո��޵�
void SIMPLE_LIBRARY_API trim_start_and_end_inline(char* buff);

//ȥ��ǰ��Ŀո�
void SIMPLE_LIBRARY_API trim_start_inline(char *buff);
//ȥ������Ŀո�
void SIMPLE_LIBRARY_API trim_end_inline(char *buff);

//���
bool SIMPLE_LIBRARY_API split(const char *buf,const char* str_split,char *l,char *r, bool bcontain_str_split);


// ���ڲ��滻�ַ����������ַ���a�滻Ϊ���ַ���b
void SIMPLE_LIBRARY_API replace_string_inline(char* str,const char* sub_char_a,const char* sub_char_b);
// ���ڲ��滻�ַ��������ַ�a�滻Ϊ���ַ�b
void SIMPLE_LIBRARY_API replace_char_inline(char *str, const char sub_char_a, const char sub_char_b);

int SIMPLE_LIBRARY_API get_printf(char *buf, const char *format, ...);

int SIMPLE_LIBRARY_API get_printf_s(char *out_buf, const char *format, ...);

int SIMPLE_LIBRARY_API get_printf_s_s(int buffer_size,char *out_buf, const char *format, ...);
// �и�ָ�����ַ���
char SIMPLE_LIBRARY_API*string_mid(const char *int_buf ,char *out_buf,int start,int count);

// խ�ַ���ת���ַ���
int SIMPLE_LIBRARY_API char_to_tchar(const char *str, wchar_t *tc);
// ���ַ���תխ�ַ���
int SIMPLE_LIBRARY_API tchar_to_char(const wchar_t *tc, char *str);

//ע�� ��str �������㹻��Ŀռ� ��Ҫ��һ���Զ�ƥ���ڴ��ָ��
void SIMPLE_LIBRARY_API wremove_string_start(wchar_t *str, wchar_t const* sub_str);

int SIMPLE_LIBRARY_API wfind_string(wchar_t *str, wchar_t const* sub_str);

void SIMPLE_LIBRARY_API wremove_wchar_start(wchar_t *str, wchar_t sub_str);

void SIMPLE_LIBRARY_API wremove_wchar_end(wchar_t *str, wchar_t sub_str);

void SIMPLE_LIBRARY_API wremove_all_wchar_end(wchar_t *str, wchar_t sub_str);

void SIMPLE_LIBRARY_API wreplace_wchar_inline(wchar_t *str, const wchar_t sub_char_a, const wchar_t sub_char_b);
void SIMPLE_LIBRARY_API wreplace_string_inline(wchar_t* str, const wchar_t* sub_char_a, const wchar_t* sub_char_b);

// ��ȡ�����ַ���������ַ�������buf��
int SIMPLE_LIBRARY_API wget_printf(wchar_t *buf, const wchar_t *format, ...);

// ��ȡ�����ַ��������ַ���
int SIMPLE_LIBRARY_API wget_printf_s(wchar_t *out_buf, const wchar_t *format, ...);

int SIMPLE_LIBRARY_API wget_printf_s_s(int buffer_size, wchar_t *out_buf,const wchar_t *format, ...);

wchar_t SIMPLE_LIBRARY_API*wstring_mid(const wchar_t *int_buf, wchar_t *out_buf, int start, int count);

_CRT_END_C_HEADER