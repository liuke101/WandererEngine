#pragma once
// 传入一个值，就检测值的状态
// 
// 以下为枚举值：
// S_OK				0x00000000
// E_UNEXPECTED		0x8000FFFF 意外的失败
// E_NOTIMPL		0x80004001 未实现
// E_OUTOFMEMORY	0x8007000E 未能分配所需的内存
// E_INVALIDARG		0x80070057 一个或多个参数无效 
// E_NOINTERFACE	0x80004002 不支持此接口
// E_POINTER		0x80004003 无效指针
// E_HANDLE			0x80070006 无效句柄
// E_ABORT			0x80004004 操作终止
// E_FAIL			0x80004005 错误
// E_ACCESSDENIED	0x80070005 一般的访问被拒绝错误、

#define ANALYSIS_HRESULT(InValue) \
{\
	if (FAILED(InValue))\
	{\
		Engine_Log_Error("Error = %i",(int)InValue);\
		assert(0);\
	}\
	else if (SUCCEEDED(InValue))\
	{\
		Engine_Log_Success("Success !");\
	}\
}