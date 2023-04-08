#pragma once
// ����һ��ֵ���ͼ��ֵ��״̬
// 
// ����Ϊö��ֵ��
// S_OK				0x00000000
// E_UNEXPECTED		0x8000FFFF �����ʧ��
// E_NOTIMPL		0x80004001 δʵ��
// E_OUTOFMEMORY	0x8007000E δ�ܷ���������ڴ�
// E_INVALIDARG		0x80070057 һ������������Ч 
// E_NOINTERFACE	0x80004002 ��֧�ִ˽ӿ�
// E_POINTER		0x80004003 ��Чָ��
// E_HANDLE			0x80070006 ��Ч���
// E_ABORT			0x80004004 ������ֹ
// E_FAIL			0x80004005 ����
// E_ACCESSDENIED	0x80070005 һ��ķ��ʱ��ܾ�����

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