#pragma once
/* Windowsƽ̨���� */
#if defined(_WIN32)
#include "../../Core/Engine.h"

// Windowsƽ̨������
class FWindowsEngine :public FEngine
{
public:
	FWindowsEngine();
	virtual int PreInit(FWinMainCommandParameters InParameters);
	virtual int Init(FWinMainCommandParameters InParameters);
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
	ComPtr<IDXGIFactory4> DXGIFactory;	// ����DirectX ͼ�λ����ṹ(DXGI)����
	ComPtr<ID3D12Device> D3dDevice;     // ��ʾ������; �����ڴ�������������������б�������С�Χ��Fence����Դ���ܵ�״̬���󡢶ѡ���ǩ�����������������Դ��ͼ��
	ComPtr<ID3D12Fence> Fence;		    // Χ��������ʵ�� GPU �� CPU ���ͬ��

	// �������
	ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList;	// �����б���CPU���ƽ������б��е������ύ���������
	ComPtr<ID3D12CommandAllocator> CommandAllocator;		// ����������������б��ڵ�����洢������������� ���������ͨ��`ExecuteCommandLists`���÷������������
	ComPtr<ID3D12CommandQueue> CommandQueue;				// ������У��洢��GPU��
	
	// ������
	ComPtr<IDXGISwapChain> SwapChain;	// ������

	// ��������
	ComPtr<ID3D12DescriptorHeap> RTVHeap;					// RTV�������ѣ���ȾĿ����ͼ(render target view)
	ComPtr<ID3D12DescriptorHeap> DSVHeap;					// DSV�������ѣ����ģ����ͼ(depth/stencil view)

	// ������
	std::vector<ComPtr<ID3D12Resource>> SwapChainBuffer;	// ������������
	ComPtr<ID3D12Resource> DepthStencilBuffer;				// ���/ģ�建����

protected:
	HWND MainWindowsHandle;			// �����ھ��
	UINT MSAA4XQualityLevels;		// 4X MSAA��������
	bool bMSAA4XEnable;				// 4X MSAA�Ƿ���
	DXGI_FORMAT BackBufferFormat;   // ��̨�������������ݸ�ʽ
	DXGI_FORMAT DepthStencilFormat; // ���ģ�建�����������ݸ�ʽ
	UINT RTVDescriptorSize;			// RTV��������ռ�ֽڵĴ�С
};

#endif