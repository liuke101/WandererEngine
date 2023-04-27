#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../../../Mesh/Core/MeshType.h"
#include "RenderingData.h"

/* 几何体 */
struct FGeometry : public IDirectXDeviceInterface_Struct
{
	// 判断模型数据是否存在
	bool bRenderingDataExistence(CMesh* InKey);
	void BuildMesh(CMesh* InMesh, const FMeshRenderingData& MeshData);

	// 构建单个模型
	void Build();
protected:
	ComPtr<ID3DBlob> CPUVertexBufferPtr;            // CPU顶点缓冲区（存储在系统内存中）
	ComPtr<ID3DBlob> CPUIndexBufferPtr;             // CPU索引缓冲区（存储在系统内存中）

	ComPtr<ID3D12Resource> GPUVertexBufferPtr;      // GPU顶点缓冲区，存在默认堆，只允许GPU访问
	ComPtr<ID3D12Resource> GPUIndexBufferPtr;       // GPU索引缓冲区，存在默认堆，只允许GPU访问

	ComPtr<ID3D12Resource> VertexUploadBufferPtr;   // 顶点上传缓冲区，顶点数据从CPU顶点缓冲区（内存）中复制到该缓冲区，再从该缓冲区复制到GPU顶点缓冲区
	ComPtr<ID3D12Resource> IndexUploadBufferPtr;    // 索引上传缓冲区

	FMeshRenderingData MeshRenderingData;
	vector<FRenderingData> DescribeMeshRenderingData;	// Mesh渲染数据描述
};

/* 几何体Map数据结构 */
struct FGeometryMap : public IDirectXDeviceInterface_Struct
{

	FGeometryMap();
	void BuildMesh(CMesh* InMesh, const FMeshRenderingData& MeshData);

	// 构建全部模型
	void Build();
public:
	map<int, FGeometry> Geometrys;
};