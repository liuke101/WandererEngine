#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"

class CMeshComponent;
/* 渲染数据 */
struct FRenderingData : public IDirectXDeviceInterface_Struct
{
public:
	FRenderingData();
public:
	UINT VertexCount;								// 顶点缓冲区视图，顶点数量
	UINT VertexTypeSize;							// 顶点缓冲区视图，单个顶点的大小(字节)  原UINT VertexStrideInBytes;
	UINT VertexOffsetPosition;

	UINT IndexCount;                                // 索引缓冲区视图，索引数量
	UINT IndexTypeSize;                             // 索引缓冲区视图，视图的索引缓冲区大小（字节) 原UINT IndexSizeInBytes;  
	UINT IndexOffsetPosition;						
	DXGI_FORMAT IndexFormat;                        // 索引缓冲区视图，索引格式

public:
	// 获取VBV的顶点缓冲区大小（字节) 
	UINT GetVertexSizeInBytes() const { return VertexCount * VertexTypeSize; };
	// 获取IBV的顶点缓冲区大小（字节) 
	UINT GetIndexSizeInBytes() const { return IndexCount * IndexTypeSize; };

	XMFLOAT4X4 ModelMatrix;							// 模型变换矩阵
	CMeshComponent* Mesh;
};