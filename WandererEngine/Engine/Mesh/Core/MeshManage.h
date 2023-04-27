#pragma once
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../../Shader/Core/Shader.h"
#include "MeshType.h"
#include "Mesh.h"
#include "../../Interface/DirectXDeviceInterface.h"
#include "../../Interface/DirectXDeviceInterface.h"
#include "../../../Engine/Rendering/Core/Buffer/ConstructBuffer.h"
#include "../../Core/Viewport/ViewportInfo.h"
#include "../../Rendering/Core/DirectX/RenderingPipeline/RenderingPipeline.h"

class FRenderingResourcesUpdate;

class CMeshManage :public CCoreMinimalObject, public IRenderingInterface, public IDirectXDeviceInterface
{
public:
	CMeshManage();

	virtual void Init();

	virtual void BuildMesh();
	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);
	virtual void PreDraw(float DeltaTime);
	virtual void Draw(float DeltaTime);
	virtual void PostDraw(float DeltaTime);

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView(); // 创建顶点缓冲区视图，与RTV不同，无需创建描述符堆
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();	// 创建索引缓冲区视图，无需创建描述符堆

public:
	CMesh* CreateBoxMesh(
		float InHeight,
		float InWidth,
		float InDepth);

	CMesh* CreateConeMesh(
		float InBottomRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMesh* CreateCylinderMesh(
		float InTopRadius,
		float InBottomRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMesh* CreatePlaneMesh(
		float InHeight,
		float InWidth,
		uint32_t InHeightSubdivide,
		uint32_t InWidthSubdivide);

	CMesh* CreateSphereMesh(
		float InRadius,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMesh* CreateMesh(string& InPath);

protected:
	template<class T, typename ...ParamTypes>
	T* CreateMesh(ParamTypes &&...Params);

protected:
	FRenderingPipeline RenderingPipeline;   // 渲染管线

	ComPtr<ID3D12DescriptorHeap> CBVHeap;           // CBV描述符堆：常量缓冲区视图（Constant Buffer View）
	shared_ptr<FRenderingResourcesUpdate> ObjectConstants;	   // 对象常量
	shared_ptr<FRenderingResourcesUpdate> ViewportConstants;   //  视口常量
	

	

	

	
};
