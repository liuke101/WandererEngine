#include "MeshManage.h"
#include "../../Config/EngineRenderConfig.h"
#include "../BoxMesh.h"
#include "../ConeMesh.h"
#include "../CustomMesh.h"
#include "../CylinderMesh.h"
#include "../SphereMesh.h"
#include "../PlaneMesh.h"
#include "ObjectTransformation.h"
#include "../../Rendering/Core/RenderingResourcesUpdate.h"
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../../Rendering/Core/Buffer/ConstructBuffer.h"
#include "../../Math/EngineMath.h"
#include "../../Core/Viewport/ViewportTransformation.h"

CMeshManage::CMeshManage()
    : VertexStrideInBytes(0)
    , VertexSizeInBytes(0)
    , IndexCount(0)
    , IndexSizeInBytes(0)
    , IndexFormat(DXGI_FORMAT_R16_UINT)
    , ModelMatrix(EngineMath::IdentityMatrix4x4())
{

}

void CMeshManage::Init()
{
    
}

void CMeshManage::BuildMesh()
{
    // 构建渲染管线
    RenderingPipeline.BuildPipeline();

    return;

    /*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/
    // 【常量缓冲区】
    //—————————————————————————————————————————————————————————————————
    // 创建CBV描述符堆
    D3D12_DESCRIPTOR_HEAP_DESC CBVHeapDesc; // 描述符堆描述
    CBVHeapDesc.NumDescriptors = 2;         // ⚠描述符数量
    CBVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    CBVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    CBVHeapDesc.NodeMask = 0;
    GetD3dDevice()->CreateDescriptorHeap(&CBVHeapDesc, IID_PPV_ARGS(&CBVHeap));

    //—————————————————————————————————————————————————————————————————
    // 创建CBV描述符

    UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);//获取指定类型描述符堆的句柄增量的大小

    // 创建ObjectCBV描述符
    ObjectConstants = make_shared<FRenderingResourcesUpdate>();
    ObjectConstants->Init(GetD3dDevice().Get(), sizeof(FObjectTransformation), 1);
    D3D12_GPU_VIRTUAL_ADDRESS ObjectGVA = ObjectConstants.get()->GetBuffer()->GetGPUVirtualAddress();    // 缓冲区的起始地址(即索引为0的那个常量缓冲区的地址)

    D3D12_CONSTANT_BUFFER_VIEW_DESC ObjectCBVDesc;  // 描述常量缓冲区视图
    ObjectCBVDesc.BufferLocation = ObjectGVA;
    ObjectCBVDesc.SizeInBytes = ObjectConstants->GetConstantBufferByteSize();

    CD3DX12_CPU_DESCRIPTOR_HANDLE DescriptorHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(CBVHeap->GetCPUDescriptorHandleForHeapStart());
    DescriptorHandle.Offset(0, DescriptorOffset);   //⚠从0开始偏移
    GetD3dDevice()->CreateConstantBufferView(&ObjectCBVDesc, DescriptorHandle); // 创建用于访问资源数据的常量缓冲区视图

    // 创建ViewportCBV描述符
    ViewportConstants = make_shared<FRenderingResourcesUpdate>();
    ViewportConstants->Init(GetD3dDevice().Get(), sizeof(FViewportTransformation), 1);
    D3D12_GPU_VIRTUAL_ADDRESS ViewportGVA = ViewportConstants.get()->GetBuffer()->GetGPUVirtualAddress();    

    D3D12_CONSTANT_BUFFER_VIEW_DESC ViewportCBVDesc;    
    ViewportCBVDesc.BufferLocation = ViewportGVA;
    ViewportCBVDesc.SizeInBytes = ViewportConstants->GetConstantBufferByteSize();

    DescriptorHandle.Offset(1, DescriptorOffset); 
    GetD3dDevice()->CreateConstantBufferView(&ViewportCBVDesc, DescriptorHandle);
   
    

    
    
}

void CMeshManage::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
    XMMATRIX ViewMatrix = XMLoadFloat4x4(&ViewportInfo.ViewMatrix);
    XMMATRIX ProjectMatrix = XMLoadFloat4x4(&ViewportInfo.ProjectionMatrix);
    XMMATRIX M_M = XMLoadFloat4x4(&ModelMatrix);
    XMMATRIX M_VP = XMMatrixMultiply(ViewMatrix, ProjectMatrix);

    // 更新HLSL中的ObjectConstBuffer数据
    FObjectTransformation ObjectTransformation;
    XMStoreFloat4x4(&ObjectTransformation.M, XMMatrixTranspose(M_M));
    ObjectConstants->Update(0, &ObjectTransformation);  //更新hlsl中的数据

    // 更新HLSL中的ViewportConstBuffer数据
    FViewportTransformation ViewportTransformation;
    XMStoreFloat4x4(&ViewportTransformation.VP, XMMatrixTranspose(M_VP));
    ViewportConstants->Update(0, &ViewportTransformation);  
}

void CMeshManage::PreDraw(float DeltaTime)
{
    // 重置命令列表
    ANALYSIS_HRESULT(GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), PSO.Get()));
}


void CMeshManage::Draw(float DeltaTime)
{
    //将CBV堆/根签名设置到命令列表
    ID3D12DescriptorHeap* DescriptorHeap[] = { CBVHeap.Get() };
    GetGraphicsCommandList()->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap);
    GetGraphicsCommandList()->SetGraphicsRootSignature(RootSignature.Get());

    // 在顶点缓冲区及其对应视图创建完成后，将它与渲染管线上的一个输入槽(input slot)相绑定。
    // 绑定后就能向管线中的【输入装配器阶段】传递顶点数据了。
    D3D12_VERTEX_BUFFER_VIEW VBV = GetVertexBufferView();
    GetGraphicsCommandList()->IASetVertexBuffers(
        0,                          // 在绑定多个顶点缓冲区时，所用的起始输入槽（若仅有一个顶点缓冲区，则将其绑定至此槽)。输入槽共有16个，索引为0~15。
        1,                          // 与输入槽绑定的顶点缓冲区数量（即视图数组中视图的数量)，如果起始输入槽的索引值为k, 且我们要绑定n个顶点缓冲区，那么这些缓冲区将                                      依次与输入槽k, k+1...k+n-1相绑定。
        &VBV);                      // 指向顶点缓冲区视图数组中第一个元素的指针

    D3D12_INDEX_BUFFER_VIEW IBV = GetIndexBufferView();
    GetGraphicsCommandList()->IASetIndexBuffer(&IBV);

    // 指定图元（又称基元）拓扑，告知D3D如何用顶点数据表示几何图元
    GetGraphicsCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);//获取指定类型描述符堆的句柄增量的大小
    CD3DX12_GPU_DESCRIPTOR_HANDLE DescriptorHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(CBVHeap->GetGPUDescriptorHandleForHeapStart());

    DescriptorHandle.Offset(0, DescriptorOffset); //⚠从0开始偏移
    GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, DescriptorHandle); // 令描述符表与渲染管线绑定（⚠寄存器编号，描述符首地址）

    DescriptorHandle.Offset(1, DescriptorOffset); 
    GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(1, DescriptorHandle);

    // 绘制索引，实例化图元
    GetGraphicsCommandList()->DrawIndexedInstanced(
        IndexCount,     // 每个实例将要绘制的索引数量
        1,              // 用于实例化技术，表示绘制实例数量
        0,              // 指向索引缓冲区的某个元素，将其标记为起始索引
        0,              // 在本次绘制调用读取顶点之前，为每个索引加上该整数值
        0);             // 用于实例化技术
}

void CMeshManage::PostDraw(float DeltaTime)
{
    
}

D3D12_VERTEX_BUFFER_VIEW CMeshManage::GetVertexBufferView()
{
    D3D12_VERTEX_BUFFER_VIEW VBV;
    VBV.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();    // 待创建视图的顶点缓冲区资源虚拟地址
    VBV.SizeInBytes = VertexSizeInBytes;                                // 待创建视图的顶点缓冲区大小（字节）
    VBV.StrideInBytes = VertexStrideInBytes;                            // 每个顶点元素所占的字节数
    return VBV;
}

D3D12_INDEX_BUFFER_VIEW CMeshManage::GetIndexBufferView()
{
    D3D12_INDEX_BUFFER_VIEW IBV;
    IBV.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();
    IBV.SizeInBytes = IndexSizeInBytes;
    IBV.Format = IndexFormat;
    return IBV;
}

CMesh* CMeshManage::CreateSphereMesh(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
    return CreateMesh<CSphereMesh>(InRadius, InAxialSubdivision, InHeightSubdivision);
}

CMesh* CMeshManage::CreateMesh(string& InPath)
{
    return CreateMesh<CCustomMesh>(InPath);
}

CMesh* CMeshManage::CreateBoxMesh(float InHeight, float InWidth, float InDepth)
{
    return CreateMesh<CBoxMesh>(InHeight, InWidth, InDepth);
}

CMesh* CMeshManage::CreateConeMesh(float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
    return CreateMesh<CConeMesh>(InBottomRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

CMesh* CMeshManage::CreateCylinderMesh(float InTopRadius, float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
    return CreateMesh<CCylinderMesh>(InTopRadius, InBottomRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

CMesh* CMeshManage::CreatePlaneMesh(float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide)
{
    return CreateMesh<CPlaneMesh>(InHeight, InWidth, InHeightSubdivide, InWidthSubdivide);
}

template<class T, typename ...ParamTypes>
T* CMeshManage::CreateMesh(ParamTypes && ...Params)
{
    T* MyMesh = new T();

    //提取模型资源
    FMeshRenderingData MeshData;
    MyMesh->CreateMesh(MeshData, forward<ParamTypes>(Params)...);

    MyMesh->BeginInit();

    //构建mesh
    RenderingPipeline.BuildMesh(MyMesh, &MeshData);

    MyMesh->Init();

    return MyMesh;
}