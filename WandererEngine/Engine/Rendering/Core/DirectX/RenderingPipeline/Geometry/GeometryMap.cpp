#include "GeometryMap.h"
#include "../../../Buffer/ConstructBuffer.h"
#include "../../../../../Mesh/Core/ObjectTransformation.h"
#include "../../../../../Core/Viewport/ViewportTransformation.h"
#include "../../../../../Mesh/Core/Mesh.h"
#include "../../../../../Mesh/Core/Material/MaterialConstantBuffer.h"
#include "../../../../../Component/Light/Core/LightConstantBuffer.h"
#include "../../../../../Mesh/Core/Material/Material.h"
#include "../../../../../Component/Mesh/Core/MeshComponent.h"
#include "../../../../../Manage/LightManage.h"
#include "../../../../../Component/Light/Core/LightComponent.h"
#include "../../../../../Component/Light/PointLightComponent.h"
#include "../../../../../Component/Light/SpotLightComponent.h"
bool FGeometry::bRenderingDataExistence(CMeshComponent* InKey)
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

void FGeometry::BuildMesh(CMeshComponent* InMesh, const FMeshRenderingData& MeshData)
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
    // 这里必须先DrawViewport，为什么？
    DrawViewport(DeltaTime);
    DrawLight(DeltaTime);
    DrawMesh(DeltaTime);
    
}

void FGeometryMap::PostDraw(float DeltaTime)
{
}

void FGeometryMap::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
    XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);
    XMMATRIX ProjectMatrix = XMLoadFloat4x4(&ViewportInfo.ProjectionMatrix);

    // 更新HLSL中的ConstantBuffer数据
    for (auto& temp : Geometrys)
    {
        for (size_t i = 0; i < temp.second.DescribeMeshRenderingData.size(); ++i)
        {
            
            FRenderingData& InRenderingData = temp.second.DescribeMeshRenderingData[i];
            
            // 构造模型变换矩阵
            XMFLOAT3& Position = InRenderingData.Mesh->GetPosition();
            fvector_3d Scale = InRenderingData.Mesh->GetScale();
            
            XMFLOAT3 RightVector = InRenderingData.Mesh->GetRightVector();
            XMFLOAT3 UpVector = InRenderingData.Mesh->GetUpVector();
            XMFLOAT3 LookatVector = InRenderingData.Mesh->GetLookatVector();
            
            InRenderingData.ModelMatrix = {
                    RightVector.x * Scale.x,	UpVector.x,				LookatVector.x ,			0.f,
                    RightVector.y,				UpVector.y * Scale.y,	LookatVector.y,			    0.f,
                    RightVector.z,				UpVector.z ,			LookatVector.z * Scale.z,	0.f,
                    Position.x,					Position.y,				Position.z,					1.f };
            
            
            // 更新Mesh位置
            XMMATRIX M_M = XMLoadFloat4x4(&InRenderingData.ModelMatrix);
            FObjectTransformation ObjectTransformation;
            {
                XMStoreFloat4x4(&ObjectTransformation.M, XMMatrixTranspose(M_M));
            }
            MeshCBV.Update(i, &ObjectTransformation);  //更新hlsl中的数据

            // 更新材质
            FMaterialConstantBuffer MaterialConstantBuffer;
            if(CMaterial* InMaterial = (*InRenderingData.Mesh->GetMaterials())[0])
            {
                // 从材质中获取basecolor并传给常量缓冲区
                fvector_4d InBaseColor = InMaterial->GetBaseColor();
                MaterialConstantBuffer.BaseColor = XMFLOAT4(InBaseColor.x, InBaseColor.y,InBaseColor.z, InBaseColor.w); // BaseColor
                MaterialConstantBuffer.Roughness = InMaterial->GetRoughness();          // 粗糙度
                MaterialConstantBuffer.MateiralType = InMaterial->GetMaterialType();    // 材质类型
            }
           
            MaterialCBV.Update(i, &MaterialConstantBuffer);
        }
    }

    // 更新灯光
    FLightConstantBuffer LightConstantBuffer;
    for (int i = 0; i < GetLightManage()->Lights.size(); i++)
    {
        if(CLightComponent* InLightComponent = GetLightManage()->Lights[i])
        {
            fvector_3d LightIntensity = InLightComponent->GetLightintensity();
            LightConstantBuffer.SceneLights[i].LightIntensity = XMFLOAT3(LightIntensity.x, LightIntensity.y, LightIntensity.z);
            LightConstantBuffer.SceneLights[i].LightDirection = InLightComponent->GetLookatVector();
            LightConstantBuffer.SceneLights[i].Position = InLightComponent->GetPosition();
            LightConstantBuffer.SceneLights[i].LightType = InLightComponent->GetLightType();

            switch(InLightComponent->GetLightType())
            {
            case PointLight:
                {
                    if(CPointLightComponent* InPointLightComponent = dynamic_cast<CPointLightComponent*>(InLightComponent))
                    {
                        LightConstantBuffer.SceneLights[i].FalloffStart = InPointLightComponent->GetFalloffStart();
                        LightConstantBuffer.SceneLights[i].FalloffEnd = InPointLightComponent->GetFalloffEnd();
                    }
                }
            case SpotLight:
                {
                    if (CSpotLightComponent* InSpotLightComponent = dynamic_cast<CSpotLightComponent*>(InLightComponent))
                    {
                        LightConstantBuffer.SceneLights[i].FalloffStart = InSpotLightComponent->GetFalloffStart();
                        LightConstantBuffer.SceneLights[i].FalloffEnd = InSpotLightComponent->GetFalloffEnd();

                        LightConstantBuffer.SceneLights[i].LightConeInnerRadians = XMConvertToRadians(InSpotLightComponent->GetLightConeInnerDegrees());
                        LightConstantBuffer.SceneLights[i].LightConeOuterRadians = XMConvertToRadians(InSpotLightComponent->GetLightConeOuterDegrees());
                    }
                }
            }
        }
    }
    LightCBV.Update(0, &LightConstantBuffer);
    

    // 更新视口
    XMMATRIX M_VP = XMMatrixMultiply(ViewMatrix, ProjectMatrix);
    FViewportTransformation ViewportTransformation;
    {
        XMStoreFloat4x4(&ViewportTransformation.VP, XMMatrixTranspose(M_VP));       // M_VP矩阵
        ViewportTransformation.ViewportPosition = ViewportInfo.ViewportPosition;    // 视口位置
    }
    ViewportCBV.Update(0, &ViewportTransformation);
}


void FGeometryMap::BuildMesh(CMeshComponent* InMesh, const FMeshRenderingData& MeshData)
{
    FGeometry& Geometry = Geometrys[0];
    Geometry.BuildMesh(InMesh, MeshData);
}


void FGeometryMap::Build()
{
    for(auto &temp : Geometrys)
    {
        temp.second.Build();
    }
}

void FGeometryMap::BuildCBVHeap()
{
    // +1 +1 是因为Viewport和Light（暂定为1个）的数据也要存在缓冲区
    DescriptorHeap.Build(GetDrawMeshObjectNumber() + GetDrawMaterialObjectNumber() + 1 + 1);
}

void FGeometryMap::BuildMeshObjectCBV()
{
    // 创建常量缓冲区
    MeshCBV.CreateConstant(sizeof(FObjectTransformation), GetDrawMeshObjectNumber());

    // 描述符句柄
    CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());

    // 构建ObjectCBV
    MeshCBV.BuildCBV(DescriptorHandle,GetDrawMeshObjectNumber(), 0);
}

void FGeometryMap::BuildMaterialObjectCBV()
{
    MaterialCBV.CreateConstant(sizeof(FMaterialConstantBuffer), GetDrawMaterialObjectNumber());
    CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());
    MaterialCBV.BuildCBV(DescriptorHandle, GetDrawMaterialObjectNumber(), GetDrawMeshObjectNumber());
}

void FGeometryMap::BuildLightObjectCBV()
{
    LightCBV.CreateConstant(sizeof(FLightConstantBuffer), GetDrawLightObjectNumber());
    CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());
    LightCBV.BuildCBV(DescriptorHandle, GetDrawLightObjectNumber(), GetDrawMeshObjectNumber() + GetDrawMaterialObjectNumber());
}

void FGeometryMap::BuildViewportCBV()
{
    ViewportCBV.CreateConstant(sizeof(FViewportTransformation), 1);
    CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(GetHeap()->GetCPUDescriptorHandleForHeapStart());
    ViewportCBV.BuildCBV(DescriptorHandle, 1, GetDrawMeshObjectNumber() + GetDrawMaterialObjectNumber() + GetDrawLightObjectNumber());
}

UINT FGeometryMap::GetDrawMeshObjectNumber()
{
    return Geometrys[0].GetDrawObjectNumber();
}

UINT FGeometryMap::GetDrawMaterialObjectNumber()
{
    UINT MaterialNum = 0;
    for(auto &temp1 : Geometrys)
    {
        for(auto &temp2 : temp1.second.DescribeMeshRenderingData)
        {
            MaterialNum += temp2.Mesh->GetMaterialNum();
        }
    }
    return MaterialNum;
}

UINT FGeometryMap::GetDrawLightObjectNumber()
{
    // 一个数组
    return 1;
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
            CD3DX12_GPU_DESCRIPTOR_HANDLE MeshDescriptorHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());
            CD3DX12_GPU_DESCRIPTOR_HANDLE MaterialDescriptorHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

            FRenderingData& RenderingData = temp.second.DescribeMeshRenderingData[i];
            
            GetGraphicsCommandList()->IASetVertexBuffers(
                0,                          // 在绑定多个顶点缓冲区时，所用的起始输入槽（若仅有一个顶点缓冲区，则将其绑定至此槽)。输入槽共有16个，索引为0~15。
                1,                          // 与输入槽绑定的顶点缓冲区数量（即视图数组中视图的数量)，如果起始输入槽的索引值为k, 且我们要绑定n个顶点缓冲区，那么这些缓冲区将                                      依次与输入槽k, k+1...k+n-1相绑定。
                &VBV);                      // 指向顶点缓冲区视图数组中第一个元素的指针

            GetGraphicsCommandList()->IASetIndexBuffer(&IBV);
            
            
            // 指定图元（又称基元）拓扑，告知D3D如何用顶点数据表示几何图元
            EMaterialDisplayStatusType DisplayStatus = (*RenderingData.Mesh->GetMaterials())[0]->GetMaterialDisplayStatus();
            GetGraphicsCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY(DisplayStatus)); //D3D_PRIMITIVE_TOPOLOGY

            // 起始地址偏移
            MeshDescriptorHandle.Offset(i, DescriptorOffset); //⚠从0开始偏移
            GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, MeshDescriptorHandle); // 令描述符表与渲染管线绑定（⚠寄存器编号，描述符首地址）

            MaterialDescriptorHandle.Offset(i + GetDrawMeshObjectNumber(), DescriptorOffset);
            GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(2, MaterialDescriptorHandle);

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

void FGeometryMap::DrawLight(float DeltaTime)
{
    UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    CD3DX12_GPU_DESCRIPTOR_HANDLE DescriptorHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

    DescriptorHandle.Offset(GetDrawMeshObjectNumber() + GetDrawMaterialObjectNumber(), DescriptorOffset); //⚠偏移起始于最后一个Object之后
    GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(3, DescriptorHandle);// 令描述符表与渲染管线绑定（⚠寄存器编号，描述符首地址）
}

void FGeometryMap::DrawViewport(float DeltaTime)
{
    UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    CD3DX12_GPU_DESCRIPTOR_HANDLE DescriptorHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(GetHeap()->GetGPUDescriptorHandleForHeapStart());

    DescriptorHandle.Offset(GetDrawMeshObjectNumber() + GetDrawMaterialObjectNumber() + GetDrawLightObjectNumber(), DescriptorOffset); //⚠偏移起始于最后一个Object之后
    GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, DescriptorHandle);// 令描述符表与渲染管线绑定（⚠寄存器编号，描述符首地址）
}