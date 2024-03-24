#pragma once
// 路径操作
#include "../simple_c_core/simple_core_minimal.h"

_CRT_BEGIN_C_HEADER
// 获取path_buf路径的身体
void get_path_directory_inline(char *path_buf);

// 获取path_buf路径的身体并拷贝到buf
void get_path_directory(char *buf, const char *path_buf);

// 获取path_buf路径的头部并拷贝到buf
void get_path_clean_filename(char *buf, const char *path_buf);
void get_path_clean_filename_w(wchar_t* buf, const wchar_t* path_buf);

// 归一化路径：文件路径中的"\\"转换成"/"
void normalization_path(char* path_buf);

// 归一化路径：path_buf路径中的"/"转换成"\\",并拷贝到buf
void normalization_directory(char *buf, const char *path_buf);

char* get_full_path(char* in_path_buf, int in_buff_len,const char *in_path);


_CRT_END_C_HEADER
