#include "DirectXRenderingEngine.h"

#include "../../../../Debug/EngineDebug.h"
#include "../../../../Config/EngineRenderConfig.h"
#include "../../../../Rendering/Core/Rendering.h"
#include "../../../../Mesh/BoxMesh.h"
#include "../../../../Mesh/SphereMesh.h"
#include "../../../../Mesh/CylinderMesh.h"
#include "../../../../Mesh/ConeMesh.h"
#include "../../../../Mesh/PlaneMesh.h"
#include "../../../../Mesh/CustomMesh.h"
#include "../../../../Core/CoreObject/CoreMinimalObject.h"
#include "../../../../Core/World.h"
#include "../../../../Mesh/Core/MeshManage.h"
#include "../../../../Mesh/Core/Material/Material.h"
#include "../../../../Manage/LightManage.h"
#include "../../../../Actor/Light/ParallelLight.h"
#if defined(_WIN32)
#include "../../../../Core/WinMainCommandParameters.h"


CDirectXRenderingEngine::CDirectXRenderingEngine()
	: CurrentFenceIndex(0)
	, CurrentSwapBufferIndex(0)
	, MSAA4XQualityLevels(0)
	, bMSAA4XEnable(false)
	, BackBufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM) //UNORM�� ��ʾ��һ��������޷�����������Χ[0,1]
	, DepthStencilFormat(DXGI_FORMAT_D24_UNORM_S8_UINT)
{
	// ����������������
	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChainBuffer.push_back(ComPtr<ID3D12Resource>());
	}

	//�ر�Tick
	bTick = false;

	LightManage = CreateObject<CLightManage>(new CLightManage());
	MeshManage = CreateObject<CMeshManage>(new CMeshManage());
}

CDirectXRenderingEngine::~CDirectXRenderingEngine()
{
	delete LightManage;
	delete MeshManage;
}

int CDirectXRenderingEngine::PreInit(FWinMainCommandParameters InParameters)
{
	Engine_Log("DirectXRenderingEngine pre-initialization complete.");
	return 0;
}

int CDirectXRenderingEngine::Init(FWinMainCommandParameters InParameters)
{
	InitDirect3D();

	PostInitDirect3D();

	MeshManage->Init();

	Engine_Log("DirectXRenderingEngine initialization complete.");

	return 0;
}

int CDirectXRenderingEngine::PostInit()
{
	ANALYSIS_HRESULT(GraphicsCommandList->Reset(CommandAllocator.Get(), NULL));

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/
	// �����ƹ�
	if(GParallelLight* ParallelLight = World->CreateActorObject<GParallelLight>())
	{
		ParallelLight->SetPosition(XMFLOAT3(10.0f, 2.0f, 10.0f));
		ParallelLight->SetRotation(fvector_3d(0.0f, 0.0f, 0.0f));
	}
    // ����Mesh
	if (GPlaneMesh* PlaneMesh = World->CreateActorObject<GPlaneMesh>())
	{
		PlaneMesh->CreateMesh(100.0f, 100.0f, 100, 100);
		PlaneMesh->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));		// ����λ��
		PlaneMesh->SetRotation(fvector_3d(0.0f, 0.0f, 0.0f));	// ������ת
		PlaneMesh->SetScale(fvector_3d(1.0f, 1.0f, 1.0f));		// ��������
		if (CMaterial* InMaterial = (*PlaneMesh->GetMaterials())[0])
		{
			InMaterial->SetMaterialType(HalfLambert);
			InMaterial->SetBaseColor(fvector_4d(0.5f, 0.5f, 0.5f, 1.0f));
			InMaterial->SetRoughness(0.8f);
		}
	}

	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	{
		SphereMesh->CreateMesh(2.0f, 100.0f, 100.0f);
		SphereMesh->SetPosition(XMFLOAT3(0.0f, 2.0f, 0.0f));	// ����λ��
		SphereMesh->SetRotation(fvector_3d(0.0f, 0.0f, 0.0f));	// ������ת
		SphereMesh->SetScale(fvector_3d(1.0f, 1.0f, 1.0f));		// ��������
		if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
		{
			InMaterial->SetMaterialType(LocalNormal);
			InMaterial->SetBaseColor(fvector_4d(0.9f, 0.8f, 0.9f, 1.0f));
			InMaterial->SetRoughness(0.8f);
		}
	}

	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	{
		SphereMesh->CreateMesh(2.0f, 100.0f, 100.0f);
		SphereMesh->SetPosition(XMFLOAT3(5.0f, 2.0f, 0.0f));	// ����λ��
		SphereMesh->SetRotation(fvector_3d(0.0f, 0.0f, 0.0f));	// ������ת
		SphereMesh->SetScale(fvector_3d(1.0f, 1.0f, 1.0f));		// ��������
		if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
		{
			InMaterial->SetMaterialType(HalfLambert);
			InMaterial->SetBaseColor(fvector_4d(0.9f, 0.8f, 0.9f, 1.0f));
			InMaterial->SetRoughness(0.8f);
			InMaterial->SetMaterialDisplayStatus(WireframeDisplay);
		}
	}

	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	{
		SphereMesh->CreateMesh(2.0f, 100.0f, 100.0f);
		SphereMesh->SetPosition(XMFLOAT3(10.0f, 2.0f, 0.0f));	// ����λ��
		SphereMesh->SetRotation(fvector_3d(0.0f, 0.0f, 0.0f));	// ������ת
		SphereMesh->SetScale(fvector_3d(1.0f, 1.0f, 1.0f));		// ��������
		if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
		{
			InMaterial->SetMaterialType(Phong);
			InMaterial->SetBaseColor(fvector_4d(0.1f, 0.1f, 0.1f, 1.0f));
			InMaterial->SetRoughness(0.8f);
		}
	}

	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	{
		SphereMesh->CreateMesh(2.0f, 100.0f, 100.0f);
		SphereMesh->SetPosition(XMFLOAT3(0.0f, 7.0f, 0.0f));	// ����λ��
		SphereMesh->SetRotation(fvector_3d(0.0f, 0.0f, 0.0f));	// ������ת
		SphereMesh->SetScale(fvector_3d(1.0f, 1.0f, 1.0f));		// ��������
		if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
		{
			InMaterial->SetMaterialType(BlinnPhong);
			InMaterial->SetBaseColor(fvector_4d(0.1f, 0.1f, 0.1f, 1.0f));
			InMaterial->SetRoughness(0.8f);
		}
	}

	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	{
		SphereMesh->CreateMesh(2.0f, 100.0f, 100.0f);
		SphereMesh->SetPosition(XMFLOAT3(5.0f, 7.0f, 0.0f));	// ����λ��
		SphereMesh->SetRotation(fvector_3d(0.0f, 0.0f, 0.0f));	// ������ת
		SphereMesh->SetScale(fvector_3d(1.0f, 1.0f, 1.0f));		// ��������
		if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
		{
			InMaterial->SetMaterialType(WrapLight);
			InMaterial->SetBaseColor(fvector_4d(0.8f, 0.6f, 0.7f, 1.0f));
			InMaterial->SetRoughness(0.8f);
		}
	}

	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	{
		SphereMesh->CreateMesh(2.0f, 100.0f, 100.0f);
		SphereMesh->SetPosition(XMFLOAT3(10.0f, 7.0f, 0.0f));	// ����λ��
		SphereMesh->SetRotation(fvector_3d(0.0f, 0.0f, 0.0f));	// ������ת
		SphereMesh->SetScale(fvector_3d(1.0f, 1.0f, 1.0f));		// ��������
		if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
		{
			InMaterial->SetMaterialType(Minnaert);
			InMaterial->SetBaseColor(fvector_4d(0.5f, 0.5f, 0.5f, 1.0f));
			InMaterial->SetRoughness(1.0f);
		}
	}

	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	{
		SphereMesh->CreateMesh(2.0f, 100.0f, 100.0f);
		SphereMesh->SetPosition(XMFLOAT3(15.0f, 2.0f, 0.0f));	// ����λ��
		SphereMesh->SetRotation(fvector_3d(0.0f, 0.0f, 0.0f));	// ������ת
		SphereMesh->SetScale(fvector_3d(1.0f, 1.0f, 1.0f));		// ��������
		if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
		{
			InMaterial->SetMaterialType(Banded);
			InMaterial->SetBaseColor(fvector_4d(1.0f, 1.0f, 1.0f, 1.0f));
			InMaterial->SetRoughness(0.5f);
		}
	}

	if (GSphereMesh* SphereMesh = World->CreateActorObject<GSphereMesh>())
	{
		SphereMesh->CreateMesh(2.0f, 100.0f, 100.0f);
		SphereMesh->SetPosition(XMFLOAT3(15.0f, 7.0f, 0.0f));	// ����λ��
		SphereMesh->SetRotation(fvector_3d(0.0f, 0.0f, 0.0f));	// ������ת
		SphereMesh->SetScale(fvector_3d(1.0f, 1.0f, 1.0f));		// ��������
		if (CMaterial* InMaterial = (*SphereMesh->GetMaterials())[0])
		{
			InMaterial->SetMaterialType(Fresnel);
			InMaterial->SetBaseColor(fvector_4d(1.0f, 1.0f, 1.0f, 1.0f));
			InMaterial->SetRoughness(0.8f);
		}
	}


	


	// if (GMesh* Cone = MeshManage->CreateConeMeshComponent(1.f, 5.f, 20.0f, 20.0f))
	// {
	// 	Cone->SetPosition(XMFLOAT3(1.0f, 1.0f, 1.0f));		// ����λ��
	// 	Cone->SetRotation(fvector_3d(0.0f, 0.0f, 0.0f));	// ������ת
	// 	Cone->SetScale(fvector_3d(1.0f, 1.0f, 1.0f));		// ��������
	// }

	// string MeshObj1Path = "../WandererEngine/Monkey.obj";  // ·��Ϊ��Ӧexe��������λ��
	// if (GMesh* Cone = MeshManage->CreateCustomMeshComponent(MeshObj1Path))
	// {
	// 	Cone->SetPosition(XMFLOAT3(0.0f, 1.0f, 0.0f));		// ����λ��
	// 	Cone->SetRotation(fvector_3d(0.0f, 0.0f, 0.0f));	// ������ת
	// 	Cone->SetScale(fvector_3d(1.0f, 1.0f, 1.0f));		// ��������
	// }
	//
	// string MeshObj2Path = "../WandererEngine/Teapot.obj";  // ·��Ϊ��Ӧexe��������λ��
	// if (GMesh* Cone = MeshManage->CreateCustomMeshComponent(MeshObj2Path))
	// {
	// 	Cone->SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));		// ����λ��
	// 	Cone->SetRotation(fvector_3d(0.0f, 0.0f, 0.0f));	// ������ת
	// 	Cone->SetScale(fvector_3d(1.0f, 1.0f, 1.0f));		// ��������
	// }
	// ����������������������������������������������������������������������������������������������������������������������������������

    MeshManage->BuildMesh();

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/
	ANALYSIS_HRESULT(GraphicsCommandList->Close());
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	WaitGPUCommandQueueComplete();

	Engine_Log("DirectXRenderingEngine post-initialization complete.");
	return 0;
}

void CDirectXRenderingEngine::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
	MeshManage->UpdateCalculations(DeltaTime, ViewportInfo);
}

void CDirectXRenderingEngine::Tick(float DeltaTime)
{
	// ���������������Ϊ��һ֡��׼��
	ANALYSIS_HRESULT(CommandAllocator->Reset());

	MeshManage->PreDraw(DeltaTime);

	// ת��״̬
	CD3DX12_RESOURCE_BARRIER ResourceBarrierPresent = CD3DX12_RESOURCE_BARRIER::Transition(
		GetCurrentSwapBuffer(),
		D3D12_RESOURCE_STATE_PRESENT,			//��D3D12_RESOURCE_STATE_COMMONͬ��
		D3D12_RESOURCE_STATE_RENDER_TARGET);
	GraphicsCommandList->ResourceBarrier(1, &ResourceBarrierPresent);

	// ��Ҫÿִ֡��
	// ���ӿ�/�ü�����
	GraphicsCommandList->RSSetViewports(1, &ViewportInfo);
	GraphicsCommandList->RSSetScissorRects(1, &ViewportRect);

	// ���RenderTarget
	GraphicsCommandList->ClearRenderTargetView(
		GetCurrentSwapBufferView(),		// CPU ���������,��ʾҪ�����RT�ĶѵĿ�ʼ
		DirectX::Colors::Black,			// ���RT����ɫ
		0,								// ָ���Ľṹ�����еľ�����
		nullptr);						// Ҫ�������Դ��ͼ�о��� ��D3D12_RECT �ṹ���顣���ΪNULL�������������Դ��ͼ

	// ������/ģ�建����
	GraphicsCommandList->ClearDepthStencilView(
		GetCurrentDepthStencilView(),						// CPU ���������,��ʾҪ��������ģ��ĶѵĿ�ʼ
		D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL,	// Ҫ�������������
		1.0f,												// ���������Ȼ�������ֵ����ֵ�������� 0 �� 1 ֮�䡣
		0,													// ���ģ�建������ֵ��
		0,													// ָ���Ľṹ�����еľ�����
		NULL);												// Ҫ�������Դ��ͼ�о��� ��D3D12_RECT �ṹ���顣���ΪNULL�������������Դ��ͼ

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// ����ĺϲ��׶�
	// ����RT�����ģ��� CPU ���������
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentSwapBufferView = GetCurrentSwapBufferView();
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentDepthStencilView = GetCurrentDepthStencilView();
	GraphicsCommandList->OMSetRenderTargets(1, &CurrentSwapBufferView, true, &CurrentDepthStencilView);

	MeshManage->Draw(DeltaTime);
	MeshManage->PostDraw(DeltaTime);

	// ��Դת��
	CD3DX12_RESOURCE_BARRIER ResourceBarrierPresentRenderTarget = CD3DX12_RESOURCE_BARRIER::Transition(
		GetCurrentSwapBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATE_PRESENT);

	GraphicsCommandList->ResourceBarrier(1, &ResourceBarrierPresentRenderTarget);

	// �ر������б�
	ANALYSIS_HRESULT(GraphicsCommandList->Close());

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// �����ύ
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);

	// ��������Buffer������
	ANALYSIS_HRESULT(SwapChain->Present(0, 0));								// ��������Buffer������
	CurrentSwapBufferIndex = !(bool)CurrentSwapBufferIndex;

	//CPU��GPU
	WaitGPUCommandQueueComplete();
}

int CDirectXRenderingEngine::PreExit()
{
	
	Engine_Log("DirectXRenderingEngine pre-exit complete.");
	return 0;
}

int CDirectXRenderingEngine::Exit()
{

	Engine_Log("DirectXRenderingEngine exit complete.");
	return 0;
}

int CDirectXRenderingEngine::PostExit()
{

	Engine_Log("DirectXRenderingEngine post-exit complete.");
	return 0;
}

ID3D12Resource* CDirectXRenderingEngine::GetCurrentSwapBuffer() const
{
	return SwapChainBuffer[CurrentSwapBufferIndex].Get();
}

D3D12_CPU_DESCRIPTOR_HANDLE CDirectXRenderingEngine::GetCurrentSwapBufferView() const
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		RTVHeap->GetCPUDescriptorHandleForHeapStart(),	//��ȡ��ʾ�ѿ�ʼ�� CPU ���������
		CurrentSwapBufferIndex,
		RTVDescriptorSize);
}

D3D12_CPU_DESCRIPTOR_HANDLE CDirectXRenderingEngine::GetCurrentDepthStencilView() const
{
	return DSVHeap->GetCPUDescriptorHandleForHeapStart();
}

UINT CDirectXRenderingEngine::GetMSAASampleCount() const
{
	return bMSAA4XEnable ? 4 : 1; ;
}

UINT CDirectXRenderingEngine::GetMSAASampleQuality() const
{
	return bMSAA4XEnable ? (MSAA4XQualityLevels - 1) : 0;
}

void CDirectXRenderingEngine::WaitGPUCommandQueueComplete()
{
	// ����Χ��ֵ���������������ǵ���Χ����
	CurrentFenceIndex++;

	// ��������������һ������������Χ���������
	// ������������Ҫ����GPU����(����GPU�����޸�Χ��ֵ)��������GPU��������������д�signal()����������֮ǰ,�������������µ�Χ����
	ANALYSIS_HRESULT(CommandQueue->Signal(Fence.Get(), CurrentFenceIndex));

	//��CPU�˵ȴ�GPU��ֱ������ִ�������Χ����֮ǰ����������
	if (Fence->GetCompletedValue() < CurrentFenceIndex)
	{
		// �������ʱ����󣬲����ؾ����
		HANDLE EventEX = CreateEventEx(NULL, NULL, 0, EVENT_ALL_ACCESS);

		// ��GPU���е�ǰ�ľ��Σ���ִ��Siganl()����޸���Χ��ֵ�����򼤷�Ԥ���¼�
		ANALYSIS_HRESULT(Fence->SetEventOnCompletion(CurrentFenceIndex, EventEX));

		// �ȴ�GPU����Χ���������¼�
		WaitForSingleObject(EventEX, INFINITE);
		CloseHandle(EventEX);
	}

}

bool CDirectXRenderingEngine::InitDirect3D()
{
	// ����DX12�ĵ��Բ�
	ComPtr<ID3D12Debug> D3D12Debug;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&D3D12Debug))))
	{
		D3D12Debug->EnableDebugLayer();
	}

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// 1. ����DXGI����
	ANALYSIS_HRESULT(CreateDXGIFactory1(IID_PPV_ARGS(&DXGIFactory))); //IID_PPV_ARGS�����ڻ�ȡCOM ID(GUID)

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// 2. ����DX12�豸����ʾ����������ͨ��ʹ�õ��豸ΪӲ���������������Կ���
	HRESULT D3dDeviceResult = D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice));
	if (FAILED(D3dDeviceResult))
	{
		// ������ʧ�ܣ�ʹ�����������WARP��Windows�߼���դ��ƽ̨)
		ComPtr<IDXGIAdapter> WARPAdapter;
		ANALYSIS_HRESULT(DXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&WARPAdapter)));
		ANALYSIS_HRESULT(D3D12CreateDevice(WARPAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice)));
	}

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// 3. ����Χ��Fence
	// ʵ��ˢ��������й��ܣ�ǿ��CPU�ȴ���ֱ��GPU�����������Ĵ����ﵽĳ��ָ����Χ����Ϊֹ��Ч�ʲ��ߵ�ͬ��������
	// ��������CPU��GPU��ͬ��֮�⣬���������������������֮ǰ����ˢ�����������ȷ��GPU��������ִ����ϡ�
	/*
	��Ⱦ���̣�
		Fence->SetEventOnCompletion
		ִ������
		�ύ����
		Queue->Signal
		wait
	*/
	ANALYSIS_HRESULT(D3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// 4. ����������С�����������������б�
	D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
	QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;	// �洢����һϵ�пɹ� GPU ֱ��ִ�е�����
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;	// ָʾĬ���������
	ANALYSIS_HRESULT(D3dDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue)));

	ANALYSIS_HRESULT(D3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(CommandAllocator.GetAddressOf()))); // GetAddressof(): �˷��������ú����������� COM�ӿڵ�ָ��

	ANALYSIS_HRESULT(D3dDevice->CreateCommandList(
		0,									// �� GPU ����
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		CommandAllocator.Get(),				// �������б���������������	//Get():�˷��������ڰ�ԭʼ�� COM �ӿ�ָ����Ϊ�������ݸ�����
		nullptr,							// �����nullptr��������ʱ���������ʼ�ܵ�״̬�� �����ϵ�
		IID_PPV_ARGS(GraphicsCommandList.GetAddressOf())));

	// ����Ҫ�������б����ڹر�״̬����Ϊ��һ�����������б�ʱ������Ҫ�����������ã��ڵ������÷���֮ǰ�����Ƚ����ر�
	ANALYSIS_HRESULT(GraphicsCommandList->Close());

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// 5. ����4X MSAA���������֧��
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS QualityLevels;
	QualityLevels.Format = BackBufferFormat;							// �������ݸ�ʽ
	QualityLevels.SampleCount = 4;										// ��������
	QualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;	// ���ڿ�����������ı�־
	QualityLevels.NumQualityLevels = 0;									// ����������

	ANALYSIS_HRESULT(D3dDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &QualityLevels, sizeof(QualityLevels)));
	MSAA4XQualityLevels = QualityLevels.NumQualityLevels;				// 4X MSAA��������

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// 6. ����������������
	SwapChain.Reset(); // �ͷ�֮ǰ�����Ľ��������������ؽ�  
	//Reset():���� ComPtr ʵ������Ϊ nullptr �ͷ���֮��ص��������ã�ͬʱ������ײ� COM �ӿڵ����ü���)���˷����Ĺ����뽫 ComPtr Ŀ��ʵ����ֵΪ nullptr ��Ч����ͬ��

	DXGI_SWAP_CHAIN_DESC SwapChainDesc;																	 // ����������
	SwapChainDesc.BufferDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;				 // �������ֱ��ʿ��
	SwapChainDesc.BufferDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;				 // �������ֱ��ʸ߶�
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = FEngineRenderConfig::GetRenderConfig()->RefreshRate;// ˢ���ʷ���
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;												 // ˢ���ʷ�ĸ
	SwapChainDesc.BufferDesc.Format = BackBufferFormat;													 // �������ݸ�ʽ
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;					 // ɨ���������ʶ����ڱ����ϻ���ɨ���ߵ�ֵ��

	SwapChainDesc.SampleDesc.Count = GetMSAASampleCount();												 // MSAA��������
	SwapChainDesc.SampleDesc.Quality = GetMSAASampleQuality();											 // MSAA������������

	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;										 // ��������Ⱦ����ȾĿ�꣨Render Target��
	SwapChainDesc.BufferCount = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;					 // ������������Ĭ��Ϊ2������˫����
	SwapChainDesc.OutputWindow = MainWindowsHandle;														 // ��Ⱦ���ڵľ��
	SwapChainDesc.Windowed = true;																		 // ture������ģʽ false��ȫ��ģʽ
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;											 // ����������ʱ��������̨������������
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;										 // ʹ�ܹ�ͨ������ ResizeTarget ���л�����/ȫ��ģʽ

	ANALYSIS_HRESULT(DXGIFactory->CreateSwapChain(CommandQueue.Get(), &SwapChainDesc, SwapChain.GetAddressOf()));  //����������

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// 7. ������������
	// RTV��
	D3D12_DESCRIPTOR_HEAP_DESC RTVDescriptorHeapDesc;												// ������������
	RTVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;									// ����������������
	RTVDescriptorHeapDesc.NumDescriptors = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;	// ���е�������������RTV������Ӧ�õ��ڽ���������������
	RTVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;									// ָʾ�ѵ�Ĭ���÷�
	RTVDescriptorHeapDesc.NodeMask = 0;																// ���ڵ�����������������Ϊ��
	ANALYSIS_HRESULT(D3dDevice->CreateDescriptorHeap(&RTVDescriptorHeapDesc, IID_PPV_ARGS(RTVHeap.GetAddressOf())));
	// DSV��
	D3D12_DESCRIPTOR_HEAP_DESC DSVDescriptorHeapDesc;
	DSVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DSVDescriptorHeapDesc.NumDescriptors = 1;		// ���е�������������DSA����Ϊ1
	DSVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DSVDescriptorHeapDesc.NodeMask = 0;
	ANALYSIS_HRESULT(D3dDevice->CreateDescriptorHeap(&DSVDescriptorHeapDesc, IID_PPV_ARGS(DSVHeap.GetAddressOf())));

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/
	return false;
}

void CDirectXRenderingEngine::PostInitDirect3D()
{
	// CPU��GPU
	WaitGPUCommandQueueComplete();

	// ��ʼ�������б�
	ANALYSIS_HRESULT(GraphicsCommandList->Reset(CommandAllocator.Get(), NULL));

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// ��ʼ��������������
	for (int i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChainBuffer[i].Reset();
	}
	DepthStencilBuffer.Reset();

	// ���Ľ������ĺ�̨��������С����ʽ�ͻ����������� Ӧ�ڵ���Ӧ�ó��򴰿ڴ�Сʱ���ô˺�����
	SwapChain->ResizeBuffers(
		FEngineRenderConfig::GetRenderConfig()->SwapChainCount,
		FEngineRenderConfig::GetRenderConfig()->ScreenWidth,
		FEngineRenderConfig::GetRenderConfig()->ScreenHeight,
		BackBufferFormat,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// ����RTV
	RTVDescriptorSize = D3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV); // ��ȡRTV��������С
	CD3DX12_CPU_DESCRIPTOR_HANDLE HeapHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());		 // ��ȡ���������е�һ���������ľ��
	// ��������С��Ϊƫ������ʹ��ƫ�����ҵ���ǰ��̨��������RTV
	for (UINT i = 0; i < FEngineRenderConfig::GetRenderConfig()->SwapChainCount; i++)
	{
		SwapChain->GetBuffer(i, IID_PPV_ARGS(&SwapChainBuffer[i]));						  // ��ý������еĻ�������Դ
		D3dDevice->CreateRenderTargetView(SwapChainBuffer[i].Get(), nullptr, HeapHandle); // Ϊ��ȡ���Ļ���������RTV
		HeapHandle.Offset(1, RTVDescriptorSize);											  // ƫ�Ƶ����������е���һ��������
	}

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// �������/ģ�建����������ͼ
	// �������/ģ�建����
	D3D12_RESOURCE_DESC ResourceDesc;   											// ��Դ����
	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;					// ��Դ��ά�ȣ���ѡ����������1D����2D����3D����
	ResourceDesc.Alignment = 0;														// ���뷽ʽ
	ResourceDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;		// ��Դ�Ŀ�ȣ���������˵��������Ϊ��λ�Ŀ�ȣ��Ի�������˵��ռ�õ��ֽ�����
	ResourceDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;		// ��Դ�ĸ߶�
	ResourceDesc.DepthOrArraySize = 1;												// ��Դ����ȣ����Ϊ 3D���������С������� 1D �� 2D ��Դ�����飩
	ResourceDesc.MipLevels = 1;														// mipmap ����
	ResourceDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;								// ��Դ���ݸ�ʽ
	ResourceDesc.SampleDesc.Count = GetMSAASampleCount();							// MSAA������
	ResourceDesc.SampleDesc.Quality = GetMSAASampleQuality();						// MSAA��������
	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;								// ������
	ResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;					// ����Ϊ��Դ�������/ģ����ͼ


	D3D12_CLEAR_VALUE ClearValue;			// ���������Դ���Ż�ֵ��
	ClearValue.DepthStencil.Depth = 1.f;	// ���������Ȼ�����
	ClearValue.DepthStencil.Stencil = 0;	// �������ģ�建����
	ClearValue.Format = DepthStencilFormat;	// �����ɫ�����ݸ�ʽ��������������ڼ�ʹ�õ���ͼ�ĸ�ʽƥ��

	CD3DX12_HEAP_PROPERTIES Properties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3dDevice->CreateCommittedResource(						// ����һ����Դ��һ���ѣ����Ѹ���Դ�ύ������				
		&Properties,										// Ĭ�϶ѣ�������ѣ���ֻ����GPU���ʣ��������
		D3D12_HEAP_FLAG_NONE,								// ָ����ѡ�������Ƿ���԰��������Լ���Դ�Ƿ���������֮�乲��
		&ResourceDesc,										// ��Դ����
		D3D12_RESOURCE_STATE_COMMON,						// ��Դ�ĳ�ʼ״̬
		&ClearValue,										// ���������Դ���Ż�ֵ
		IID_PPV_ARGS(DepthStencilBuffer.GetAddressOf())		// ��Դ�ӿڵ�COM ID
	);

	D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc;					// �����ɴ����ģ����ͼ���ʵ����������Դ��
	DSVDesc.Format = DepthStencilFormat;					// ��Դ���ݸ�ʽ
	DSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;	// �������ģ����ͼ�е���Դ�ķ�ʽ����Դ����Ϊ 2D ������з���
	DSVDesc.Flags = D3D12_DSV_FLAG_NONE;					// ָ�������Ƿ�Ϊֻ����None
	DSVDesc.Texture2D.MipSlice = 0;							// Texture2DҪʹ�õĵ�һ�� mipmap ����

	// �������/ģ����ͼ
	D3dDevice->CreateDepthStencilView(DepthStencilBuffer.Get(), &DSVDesc, DSVHeap->GetCPUDescriptorHandleForHeapStart());

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// ��Դ״̬ת��
	CD3DX12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(	// ������ͬ�÷�֮������Դת���Ľṹ
		DepthStencilBuffer.Get(),			// ת����ʹ�õ���Դ
		D3D12_RESOURCE_STATE_COMMON,		// ��Դ�ġ�֮ǰ���÷�
		D3D12_RESOURCE_STATE_DEPTH_WRITE);	// ��Դ�ġ����÷�
	GraphicsCommandList->ResourceBarrier(1, &Barrier); // ����ת����Դ�������飬ֻ�Ǽ��������б�����ִ����Ҫ�����������
	GraphicsCommandList->Close();					   // ������������б�֮���ύ�����б�֮ǰ�����������ļ�¼

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// �����ύ
	ID3D12CommandList* CommandList[] = { GraphicsCommandList.Get() };
	CommandQueue->ExecuteCommandLists(_countof(CommandList), CommandList);  //�������б����������ӵ����������  //_countof()���������е�Ԫ����

	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

	// �����ӿ�
	// �����ӿڳߴ�
	ViewportInfo.TopLeftX = 0;		// ���Ͻǵ� x ����
	ViewportInfo.TopLeftY = 0;		// ���Ͻǵ� y ����
	ViewportInfo.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	ViewportInfo.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;
	ViewportInfo.MinDepth = 0.0f;	//�����ֵ������[0,1]ת��������[MinDepth,MaxDepth], ʵ��ĳЩ����Ч��
	ViewportInfo.MaxDepth = 1.0f;

	// ���òü�����
	ViewportRect.left = 0;														// ���Ͻǵ� x ����
	ViewportRect.top = 0;														// ���Ͻǵ� y ����
	ViewportRect.right = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;	// ���½ǵ� x ����
	ViewportRect.bottom = FEngineRenderConfig::GetRenderConfig()->ScreenHeight;	// ���½ǵ� y ����


	/*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/
	WaitGPUCommandQueueComplete();

}


#endif

