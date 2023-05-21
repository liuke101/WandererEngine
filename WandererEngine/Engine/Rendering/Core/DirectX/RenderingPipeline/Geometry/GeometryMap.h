#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../../../Mesh/Core/MeshType.h"
#include "RenderingData.h"
#include "../DescriptorHeap/DescriptorHeap.h"
#include "../ConstantBufferView/ConstantBufferView.h"
#include "../../../../../Core/Viewport/ViewportInfo.h"


class CMaterial;
class FRenderingTextureResourcesUpdate;

/* 几何体 */
struct FGeometry : public IDirectXDeviceInterface_Struct
{
	friend struct FGeometryMap;
public:
	// 判断模型数据是否存在
	bool bRenderingDataExistence(CMeshComponent* InKey);

	// 构建单个模型的Mesh
	void BuildMesh(CMeshComponent* InMesh, const FMeshRenderingData& MeshData);

	// 构建单个模型
	void Build();

	// 获得绘制object的数量
	UINT GetDrawObjectNumber() const { return DescribeMeshRenderingData.size(); }

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView(); // 创建顶点缓冲区视图，与RTV不同，无需创建描述符堆
	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();	// 创建索引缓冲区视图，无需创建描述符堆
protected:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;            // CPU顶点缓冲区（存储在系统内存中）
	ComPtr<ID3DBlob> CPUIndexBufferPtr;             // CPU索引缓冲区（存储在系统内存中）

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;      // GPU顶点缓冲区，存在默认堆，只允许GPU访问
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;       // GPU索引缓冲区，存在默认堆，只允许GPU访问

	ComPtr<ID3D12Resource> VertexUploadBufferPtr;   // 顶点上传缓冲区，顶点数据从CPU顶点缓冲区（内存）中复制到该缓冲区，再从该缓冲区复制到GPU顶点缓冲区
	ComPtr<ID3D12Resource> IndexUploadBufferPtr;    // 索引上传缓冲区

	FMeshRenderingData MeshRenderingData;			// 渲染数据

	vector<FRenderingData> DescribeMeshRenderingData;	// Mesh渲染数据描述
};

/* 几何体Map数据结构 */
struct FGeometryMap : public IDirectXDeviceInterface_Struct
{
public:
	FGeometryMap();

	void PreDraw(float DeltaTime);

    void Draw(float DeltaTime);

	void PostDraw(float DeltaTime);

	void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);
	void UpdateMaterialSRV(float DeltaTime, const FViewportInfo& ViewportInfo);
	// 构建全部模型的Mesh
	void BuildMesh(CMeshComponent* InMesh, const FMeshRenderingData& MeshData);

	// 读取贴图
	void LoadTexture();

	// 构建全部模型
	void Build();

	// 构建描述符堆
	void BuildDescriptorHeap();

	// 构建CBV（常量缓冲区视图）
	void BuildMeshObjectCBV();
	void BuildLightObjectCBV();
	void BuildViewportCBV();

	// 获得绘制Object的数量
	UINT GetDrawMeshObjectNumber();
	UINT GetDrawMaterialObjectNumber();
	UINT GetDrawLightObjectNumber();

	// 获得贴图数量
	UINT GetDrawTextureResourcesNumber();

	// 构建SRV
	void BuildMaterialSRV();
	void BuildTextureSRV();
public:
	// 渲染Mesh
	void DrawMesh(float DeltaTime);
	// 渲染Material
	void DrawMaterial(float DeltaTime);
	// 渲染Light
	void DrawLight(float DeltaTime);
	// 渲染Viewport
	void DrawViewport(float DeltaTime);
	// 渲染Texture
	void DrawTexture(float DeltaTime);
	
public:
	ID3D12DescriptorHeap* GetHeap() const { return DescriptorHeap.GetHeap(); }
	
protected:
	map<int, FGeometry> Geometrys;
	FDescriptorHeap DescriptorHeap;	  // CBV堆
	FConstantBufferView MeshCBV;    
	FConstantBufferView ViewportCBV;
	FConstantBufferView MaterialCBV;
	FConstantBufferView LightCBV;

	std::shared_ptr<FRenderingTextureResourcesUpdate> RenderingTextureResources;
	std::vector<CMaterial*> Materials;
};