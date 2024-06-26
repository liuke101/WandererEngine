#pragma once
// 文件操作
#include "../simple_c_core/simple_core_minimal.h"

_CRT_BEGIN_C_HEADER
#define _number_of_successful_conversions(a) a
#define _out_pram(a) a
#define _in_pram(a) a

typedef struct
{
	int index;
	char paths[2048][512];//MAX_PATH
}def_c_paths;

typedef struct
{
	int index;
	wchar_t paths[2048][512];//MAX_PATH
}def_c_paths_w;

void init_def_c_paths(def_c_paths *c_paths);
void init_def_c_paths_w(def_c_paths_w* c_paths);

// 拷贝文件（源路径，目标路径）
int copy_file(char *Src, char *Dest);

// 查找文件（第3个参数确定是否递归查找）
void find_files(char const *in_path, def_c_paths *str,bool b_recursion);

// 文件是否存在
bool is_file_exists(char const* filename);

// 创建文件夹
bool create_file(char const *filename);
 
// 创建路径
bool create_file_directory(char const *in_path);

// 打开地址（可以是网站也可以是本地文件）
bool open_url(const char* url);

// 通过参数打开url
bool open_url_by_param(const char* url, const char* param);

// 通过操作打开某个东西
bool open_by_operation(const char* in_operation, const char* url, const char* param);

// 打开一个文件夹
bool open_explore(const char* url);


// 从指定的文件路径读取内容，并将内容存储到传入的字符缓冲区中。如果读取文件成功，则返回 true，否则返回 false。
// 使用该接口 一定要初始化buf,比如：char buf[1024] = { 0 };
bool get_file_buf(const char *path,char *buf);

bool save_file_buff(const char* path, char* buf);

bool add_file_buf(const char *path, char *buf);

// 以字符串的方式存储，如果数据中有0 自动截断，建议用二进制存储
bool add_new_file_buf(const char *path, char *buf);

// 以二进制方式读取
bool load_data_from_disk(const char* path, char* buf);

// 计算指定文件名的文件大小
unsigned int get_file_size_by_filename(const char *filename);
// 计算指定文件的文件大小并返回结果。它接受一个已经打开的文件指针作为参数，而不是文件名。
unsigned int get_file_size(FILE *file_handle);

//这个是以二进制方式存储，不会遇到像0自动截断的情况
bool save_data_to_disk(const char* path, char* buf, int buf_size);

//宽字符和窄字符
// 
//宽字符转窄字符
_number_of_successful_conversions(size_t) wchar_t_to_char(
	_out_pram(char*) dst_char,
	size_t char_size,
	_in_pram(wchar_t const*) _Src);

//窄字符转宽字符
_number_of_successful_conversions(size_t) char_to_wchar_t(
	_out_pram(wchar_t*) dst_wchar_t,
	size_t wchar_t_size,
	_in_pram(char const*) _Src);

//宽字符
//////////////////////////////////////////////
// 以字符串的方式存储，如果数据中有0 自动截断，建议用二进制存储
bool add_new_file_buf_w(const wchar_t* path, char* buf);

bool get_file_buf_w(const wchar_t* path, char* buf);

// 以二进制方式存储，不会遇到像0自动截断的情况
bool save_data_to_disk_w(const wchar_t* path, char* buf,int buf_size);

// 以二进制方式读取 buf的大小要比实际大小+1 因为最后一位留给/0
bool load_data_from_disk_w(const wchar_t* path, char* buf);

bool is_file_exists_w(const wchar_t *filename);

// 打开地址
bool open_url_w(const wchar_t* url);

bool open_url_by_param_w(const wchar_t* url,const wchar_t *param);

bool open_by_operation_w(const wchar_t *in_operation, const wchar_t* url, const wchar_t* param);

// 打开一个文件夹
bool open_explore_w(const wchar_t* url);

unsigned int get_file_size_by_filename_w(const wchar_t* filename);
_CRT_END_C_HEADER