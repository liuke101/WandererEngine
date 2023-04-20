#pragma once
/* Windowsƽ̨���� */
#if defined(_WIN32)
#include "../../Core/Engine.h"
// Windowsƽ̨������
class CWindowsEngine :public CEngine
{
	friend class IRenderingInterface;

public:
	CWindowsEngine();
	~CWindowsEngine();
	virtual int PreInit(FWinMainCommandParameters InParameters);
	virtual int Init(FWinMainCommandParameters InParameters);
	virtual int PostInit();

	
	virtual void Tick(float DeltaTime);

	virtual int PreExit();
	virtual int Exit();
	virtual int PostExit();


public:
	// ��ȡ��ǰ��Buffer
	ID3D12Resource* GetCurrentSwapBuffer() const;	

	// ��ȡ��ǰBuffer�����������
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentSwapBufferView() const;

	// ��ȡ��ǰ���ģ�建���������������
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentDepthStencilView() const;

public:
	// ��ȡ��̨�������������ݸ�ʽ
	DXGI_FORMAT GetBackBufferFormat() const { return BackBufferFormat; }
	// ��ȡ���ģ�建�����������ݸ�ʽ
	DXGI_FORMAT GetDepthStencilFormat() const { return DepthStencilFormat; }
	// ��ȡMSAA������
	UINT GetMSAASampleCount() const;
	// ��ȡMSAA��������
	UINT GetMSAASampleQuality() const;

protected:
	// ʹ��Χ��Fence��ˢ���������
	void WaitGPUCommandQueueComplete();

private:
	// ��ʼ��Windows���� 
	bool InitWindows(FWinMainCommandParameters InParameters);
	// ��ʼ��Direct3D 
	bool InitDirect3D();
    void PostInitDirect3D();

protected:
	UINT64 CurrentFenceIndex = 0;       // Χ��ֵ
	int CurrentSwapBufferIndex = 0;		// ��ǰBuffer������
protected:
	ComPtr<IDXGIFactory4> DXGIFactory;	// ����DirectX ͼ�λ����ṹ(DXGI)����
	ComPtr<ID3D12Device> D3dDevice;     // ��ʾ������; �����ڴ�������������������б�������С�Χ��Fence����Դ���ܵ�״̬���󡢶ѡ���ǩ�����������������Դ��ͼ��
	ComPtr<ID3D12Fence> Fence;		    // Χ��������ʵ�� GPU �� CPU ���ͬ��

	// �������
	ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList;	// �����б���CPU���ƽ������б��е������ύ���������
	ComPtr<ID3D12CommandAllocator> CommandAllocator;		// ����������������б��ڵ�����洢������������� 
	ComPtr<ID3D12CommandQueue> CommandQueue;				// ������У��洢��GPU�ϣ�ͨ��`ExecuteCommandLists`���÷������������
	
	// ������
	ComPtr<IDXGISwapChain> SwapChain;	// ������

	// ��������
	ComPtr<ID3D12DescriptorHeap> RTVHeap;					// RTV�������ѣ���ȾĿ����ͼ(render target view)
	ComPtr<ID3D12DescriptorHeap> DSVHeap;					// DSV�������ѣ����ģ����ͼ(depth/stencil view)

	// ������
	std::vector<ComPtr<ID3D12Resource>> SwapChainBuffer;	// ������������
	ComPtr<ID3D12Resource> DepthStencilBuffer;				// ���/ģ�建����

	// �ӿ�
	// ��̨��������СΪ�������ڣ���ʱֻ��ϣ���ѳ������Ƶ���̨��������ĳ�������������У��������������ӿڡ�����ϵ�Ի��������Ͻ�Ϊԭ��
	D3D12_VIEWPORT ViewportInfo;

	// �ü�����
	D3D12_RECT ViewportRect;

protected:
	HWND MainWindowsHandle;			// �����ھ��
	UINT MSAA4XQualityLevels;		// 4X MSAA��������
	bool bMSAA4XEnable;				// 4X MSAA�Ƿ���
	DXGI_FORMAT BackBufferFormat;   // ��̨�������������ݸ�ʽ
	DXGI_FORMAT DepthStencilFormat; // ���ģ�建�����������ݸ�ʽ
	UINT RTVDescriptorSize;			// RTV��������ռ�ֽڵĴ�С
};

#endif