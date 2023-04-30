﻿#include "GeometryMap.h"
#include "../../../Buffer/ConstructBuffer.h"
#include "../../../../../Mesh/Core/ObjectTransformation.h"
#include "../../../../../Core/Viewport/ViewportTransformation.h"
#include "../../../../../Mesh/Core/Mesh.h"

bool FGeometry::bRenderingDataExistence(GMesh* InKey)
{
    for(auto &Tmp : DescribeMeshRenderingData)
    {
        if(Tmp.Mesh == InKey)
        {
            return true;
        }
    }
    return false;
}

void FGeometry::BuildMesh(GMesh* InMesh, const FMeshRenderingData& MeshData)
{
    if(!bRenderingDataExistence(InMesh))
    {
        DescribeMeshRenderingData.push_back(FRenderingData());
        FRenderingData& RenderingData = DescribeMeshRenderingData[DescribeMeshRenderingData.size() - 1];

        // 属性设置
        RenderingData.Mesh = InMesh;

        RenderingData.VertexCount = MeshData.VertexData.size();
        RenderingData.IndexCount = MeshData.IndexData.size();

        RenderingData.VertexOffsetPosition = MeshRenderingData.VertexData.size();
        RenderingData.IndexOffsetPosition = MeshRenderingData.IndexData.size();

        // 顶点/索引合并
        MeshRenderingData.VertexData.insert(MeshRenderingData.VertexData.end(), MeshData.VertexData.begin(), MeshData.VertexData.end());
        MeshRenderingData.IndexData.insert(MeshRenderingData.IndexData.end(), MeshData.IndexData.begin(), MeshData.IndexData.end());
    }
}

void FGeometry::Build()
{
    // 计算顶点/索引缓冲区大小（字节）
    UINT VertexSizeInBytes = MeshRenderingData.GetVertexSizeInBytes();
    UINT IndexSizeInBytes = MeshRenderingData.GetIndexSizeInBytes();

    // 创建CPU顶点/索引缓冲区
    ANALYSIS_HRESULT(D3DCreateBlob(VertexSizeInBytes, &CPUVertexBufferPtr));
    memcpy(CPUVertexBufferPtr->GetBufferPointer(), MeshRenderingData.VertexData.data(), VertexSizeInBytes);
    ANALYSIS_HRESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));
    memcpy(CPUIndexBufferPtr->GetBufferPointer(), MeshRenderingData.IndexData.data(), IndexSizeInBytes);

    // 创建GPU顶点/索引缓冲区
    ConstructBuffer::FConstructBuffer ConstructBuffer;
    GPUVertexBufferPtr = ConstructBuffer.ConstructDefaultBuffer(VertexUploadBufferPtr, MeshRenderingData.VertexData.data(), VertexSizeInBytes);
    GPUIndexBufferPtr = ConstructBuffer.ConstructDefaultBuffer(IndexUploadBufferPtr, MeshRenderingData.IndexData.data(), IndexSizeInBytes);
}

D3D12_VERTEX_BUFFER_VIEW FGeometry::GetVertexBufferView()
{
    D3D12_VERTEX_BUFFER_VIEW VBV;
    VBV.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();    // 待创建视图的顶点缓冲区资源虚拟地址
    VBV.SizeInBytes = MeshRenderingData.GetVertexSizeInBytes();         // 待创建视图的顶点缓冲区大小（字节）
    VBV.StrideInBytes = sizeof(FVertex);                                 // 每个顶点元素所占的字节数
    return VBV;
}

D3D12_INDEX_BUFFER_VIEW FGeometry::GetIndexBufferView()
{
    D3D12_INDEX_BUFFER_VIEW IBV;
    IBV.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();
    IBV.SizeInBytes = MeshRenderingData.GetIndexSizeInBytes();
    IBV.Format = DXGI_FORMAT_R16_UINT;
    return IBV;
}

FGeometryMap::FGeometryMap()
{
    Geometrys.insert(pair<int, FGeometry>(0, FGeometry()));
}

void FGeometryMap::PreDraw(float DeltaTime)
{
    DescriptorHeap.PreDraw(DeltaTime);
}

void FGeometryMap::Draw(float DeltaTime)
{
    DrawMesh(DeltaTime);
    DrawViewport(DeltaTime);
}

void FGeometryMap::PostDraw(float DeltaTime)
{
}

void FGeometryMap::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
    XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);
    XMMATRIX ProjectMatrix = XMLoadFloat4x4(&ViewportInfo.ProjectionMatrix);

    // 更新HLSL中的ObjectConstBuffer数据
    for (auto& temp : Geometrys)
    {
        for (size_t i = 0; i < temp.second.DescribeMeshRenderingData.size(); ++i)
        {
            
            FRenderingData& RenderingData = temp.second.DescribeMeshRenderingData[i];
            
            // 构造模型变换矩阵
            XMFLOAT3& Position = RenderingData.Mesh->GetPosition();
            fvector_3d Scale = RenderingData.Mesh->GetScale();
            
            XMFLOAT3 RightVector = RenderingData.Mesh->GetRightVector();
            XMFLOAT3 UpVector = RenderingData.Mesh->GetUpVector();
            XMFLOAT3 LookatVector = RenderingData.Mesh->GetLookatVector();
            
            RenderingData.ModelMatrix = {
                    RightVector.x * Scale.x,	UpVector.x,				LookatVector.x ,			0.f,
                    RightVector.y,				UpVector.y * Scale.y,	LookatVector.y,			    0.f,
                    RightVector.z,				UpVector.z ,			LookatVector.z * Scale.z,	0.f,
                    Position.x,					Position.y,				Position.z,					1.f };
            
            
            // 设置模型位置
            XMMATRIX M_M = XMLoadFloat4x4(&RenderingData.ModelMatrix);
            
            FObjectTransformation ObjectTransformation;
            XMStoreFloat4x4(&ObjectTransformation.M, XMMatrixTranspose(M_M));
            ObjectCBV.Update(i, &ObjectTransformation);  //更新hlsl中的数据
        }
    }

    //更新HLSL中的ViewportConstBuffer数据
    XMMATRIX M_VP = XMMatrixMultiply(ViewMatrix, ProjectMatrix);
    FViewportTransformation ViewportTransformation;
    XMStoreFloat4x4(&ViewportTransformation.VP, XMMatrixTranspose(M_VP));
    ViewportCBV.Update(0, &ViewportTransformation);


}


void FGeometryMap::BuildMesh(GMesh* InMesh, const FMeshRenderingData& MeshData)
{
    FGeometry& Geometry = Geometrys[0];
    Geometry.BuildMesh(InMesh, MeshData);
}


void FGeometryMap::Build()
{
    for(auto &Tmp : Geometrys)
    {
        Tmp.second.Build();
    }
}

void FGeometryMap::BuildCBVHeap()
{
    // +1 是因为Viewport的数据也要存在缓冲区
    DescriptorHeap.Build(GetDrawObjectNumber() + 1);
}

void FGeometryMap::BuildObjectCBV()
{
    // 创建常量缓冲区
    ObjectCBV.CreateConstant(sizeof(FObjectTransformation), GetDrawObjectNumber());

    // 描述符句柄
    CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

    // 构建ObjectCBV
    ObjectCBV.BuildCBV(DescriptorHandle,GetDrawObjectNumber(), 0);
}


UINT FGeometryMap::GetDrawObjectNumber()
{
    return Geometrys[0].GetDrawObjectNumber();
}

void FGeometryMap::BuildViewportCBV()
{
    // 创建常量缓冲区
    ViewportCBV.CreateConstant(sizeof(FViewportTransformation), 1);

    // 描述符句柄
    CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

    // 构建ViewportCBV
    ViewportCBV.BuildCBV(DescriptorHandle, 1, GetDrawObjectNumber());
}


void FGeometryMap::DrawMesh(float DeltaTime)
{
    UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);//获取指定类型描述符堆的句柄增量的大小

    for(auto &temp : Geometrys)
    {
        // 在顶点缓冲区及其对应视图创建完成后，将它与渲染管线上的一个输入槽(input slot)相绑定。
        // 绑定后就能向管线中的【输入装配器阶段】传递顶点数据了。
        D3D12_VERTEX_BUFFER_VIEW VBV = temp.second.GetVertexBufferView();
        D3D12_INDEX_BUFFER_VIEW IBV = temp.second.GetIndexBufferView();
       
        for(int i = 0; i < temp.second.DescribeMeshRenderingData.size(); ++i)
        {
            CD3DX12_GPU_DESCRIPTOR_HANDLE DescriptorHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
            FRenderingData& RenderingData = temp.second.DescribeMeshRenderingData[i];

            GetGraphicsCommandList()->IASetVertexBuffers(
                0,                          // 在绑定多个顶点缓冲区时，所用的起始输入槽（若仅有一个顶点缓冲区，则将其绑定至此槽)。输入槽共有16个，索引为0~15。
                1,                          // 与输入槽绑定的顶点缓冲区数量（即视图数组中视图的数量)，如果起始输入槽的索引值为k, 且我们要绑定n个顶点缓冲区，那么这些缓冲区将                                      依次与输入槽k, k+1...k+n-1相绑定。
                &VBV);                      // 指向顶点缓冲区视图数组中第一个元素的指针

            GetGraphicsCommandList()->IASetIndexBuffer(&IBV);

            // 指定图元（又称基元）拓扑，告知D3D如何用顶点数据表示几何图元
            GetGraphicsCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


            DescriptorHandle.Offset(i, DescriptorOffset); //⚠从0开始偏移
            GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, DescriptorHandle); // 令描述符表与渲染管线绑定（⚠寄存器编号，描述符首地址）

            // 绘制索引，实例化图元
            GetGraphicsCommandList()->DrawIndexedInstanced(
                RenderingData.IndexCount,           // 每个实例将要绘制的索引数量
                1,                                  // 用于实例化技术，表示绘制实例数量
                RenderingData.IndexOffsetPosition,  // 指向索引缓冲区的某个元素，将其标记为起始索引
                RenderingData.VertexOffsetPosition, // 在本次绘制调用读取顶点之前，为每个索引加上该整数值
                0);                                 // 用于实例化技术
        }
    }
}


void FGeometryMap::DrawViewport(float DeltaTime)
{
    UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    CD3DX12_GPU_DESCRIPTOR_HANDLE DescriptorHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

    DescriptorHandle.Offset(GetDrawObjectNumber(), DescriptorOffset); //⚠偏移起始于最后一个Object之后
    GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, DescriptorHandle);// 令描述符表与渲染管线绑定（⚠寄存器编号，描述符首地址）
}