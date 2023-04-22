#pragma once
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../../Shader/Core/Shader.h"
#include "MeshType.h"
#include "Mesh.h"

class FRenderingResourcesUpdate;

class CMeshManage :public CCoreMinimalObject, public IRenderingInterface
{
public:
	CMeshManage();

	virtual void Init();

	virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

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
		float InRadius,
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
	ComPtr<ID3DBlob> CPUVertexBufferPtr;            // CPU顶点缓冲区（存储在系统内存中）
	ComPtr<ID3DBlob> CPUIndexBufferPtr;             // CPU索引缓冲区（存储在系统内存中）

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;      // GPU顶点缓冲区，存在默认堆，只允许GPU访问
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;       // GPU索引缓冲区，存在默认堆，只允许GPU访问

	ComPtr<ID3D12Resource> VertexUploadBufferPtr;   // 顶点上传缓冲区，顶点数据从CPU顶点缓冲区（内存）中复制到该缓冲区，再从该缓冲区复制到GPU顶点缓冲区
	ComPtr<ID3D12Resource> IndexUploadBufferPtr;    // 索引上传缓冲区

	ComPtr<ID3D12DescriptorHeap> CBVHeap;           // CBV描述符堆：常量缓冲区视图（Constant Buffer View）
	shared_ptr<FRenderingResourcesUpdate> objectConstants;   // 对象常量
	ComPtr<ID3D12RootSignature>  RootSignature;     // 根签名

	UINT VertexStrideInBytes;                       // 顶点缓冲区视图，单个顶点的大小(字节)
	UINT VertexSizeInBytes;                         // 顶点缓冲区视图，视图的顶点缓冲区大小（字节)                                         
	UINT IndexCount;                                // 索引缓冲区视图，索引数量
	UINT IndexSizeInBytes;                          // 索引缓冲区视图，视图的顶点缓冲区大小（字节)
	DXGI_FORMAT IndexFormat;                        // 索引缓冲区视图，索引格式

protected:
	FShader VertexShader;                               // 顶点着色器 
	FShader PixelShader;                                // 像素着色器
	vector<D3D12_INPUT_ELEMENT_DESC> InputLayoutDESC;   // 输入布局描述符
	ComPtr<ID3D12PipelineState> PSO;                    // PSO流水线状态对象

protected:
	XMFLOAT4X4 ModelMatrix;
	XMFLOAT4X4 ViewMatrix;
	XMFLOAT4X4 ProjectionMatrix;
};
