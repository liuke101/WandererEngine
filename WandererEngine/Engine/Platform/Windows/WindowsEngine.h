#pragma once
/* Windowsƽ̨���� */
#if defined(_WIN32)
#include "../../Core/Engine.h"

// Windowsƽ̨������
class FWindowsEngine :public FEngine
{
public:
	virtual int PreInit(FWinMainCommandParameters InParameters);
	virtual int Init();
	virtual int PostInit();

	virtual void Tick();

	virtual int PreExit();
	virtual int Exit();
	virtual int PostExit();

private:
	// ��ʼ��Windows���� 
	bool InitWindows(FWinMainCommandParameters InParameters);
	// ��ʼ��Direct3D 
	bool InitDirect3D();

protected:
	ComPtr<IDXGIFactory4> DXGIFactory; // ����DirectX ͼ�λ����ṹ(DXGI)����
	ComPtr<ID3D12Device> D3dDevice;    // ��ʾ������; �����ڴ�������������������б�������С�Χ��Fence����Դ���ܵ�״̬���󡢶ѡ���ǩ�����������������Դ��ͼ��
	ComPtr<ID3D12Fence> Fence;		   // Χ��������ʵ�� GPU �� CPU ���ͬ��

	ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList;	// �����б���CPU���ƽ������б��е������ύ���������
	ComPtr<ID3D12CommandAllocator> CommandAllocator;		// ����������������б��ڵ�����洢������������� ���������ͨ��`ExecuteCommandLists`���÷������������
	ComPtr<ID3D12CommandQueue> CommandQueue;				// ������У��洢��GPU��
	
	ComPtr<IDXGISwapChain> SwapChain;

protected:
	HWND MainWindowsHandle;	// �����ھ��
};

#endif