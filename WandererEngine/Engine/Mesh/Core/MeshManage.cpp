#include "MeshManage.h"
#include "../../Config/EngineRenderConfig.h"
#include "../BoxMesh.h"
#include "../ConeMesh.h"
#include "../CustomMesh.h"
#include "../CylinderMesh.h"
#include "../SphereMesh.h"
#include "../PlaneMesh.h"
#include "ObjectTransformation.h"
#include "../../Core/Viewport/ViewportTransformation.h"
#include "../../Rendering/Core/RenderingResourcesUpdate.h"
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"


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

void CMeshManage::BuildMesh(const FMeshRenderingData* InRenderingData)
{
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

    D3D12_CONSTANT_BUFFER_VIEW_DESC ObjectCBVDesc;  // 描述要查看的常量缓冲区
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
    //—————————————————————————————————————————————————————————————————
    // 创建描述符表
    // 创建ObjectCBV描述符表
    CD3DX12_DESCRIPTOR_RANGE ObjectCBVTable;
    ObjectCBVTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0); //（描述符表的类型，表中的描述符数量，⚠寄存器编号）

    // 创建ViewportCBV描述符表
    CD3DX12_DESCRIPTOR_RANGE ViewportCBVTable;
    ViewportCBVTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1); 

    //—————————————————————————————————————————————————————————————————
    // 根参数绑定
    // 根参数可以是根常量、根描述符、描述符表
    // 根参数将描述符表绑定到常量缓冲区寄存器register(x#)，以供着色器程序访问。register(x#)表示寄存器传递的资源类型
    // t:着色器资源视图  s:采样器  b：常量缓冲区视图  #：寄存器编号
    CD3DX12_ROOT_PARAMETER RootParam[2];                    // ⚠描述符表大小=使用的寄存器槽数量
    RootParam[0].InitAsDescriptorTable(1, &ObjectCBVTable); //（描述符区域的数量，指向描述符区域数组的指针）
    RootParam[1].InitAsDescriptorTable(1, &ViewportCBVTable);

    //—————————————————————————————————————————————————————————————————
    // 创建根签名
    CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
        2,  // ⚠大小=使用的寄存器槽数量
        RootParam,
        0, 
        nullptr, 
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3D10Blob> SerializeRootSignature;
    ComPtr<ID3D10Blob> ErrorBlob;
    D3D12SerializeRootSignature(    // 必须先将根签名的描述进行序列化处理，才可传入CreateRootSignature方法，正是创建根签名
        &RootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        SerializeRootSignature.GetAddressOf(),
        ErrorBlob.GetAddressOf());

    if (ErrorBlob)
    {
        Engine_Log_Error("%s", (char*)ErrorBlob->GetBufferPointer());
    }

    GetD3dDevice()->CreateRootSignature(
        0,
        SerializeRootSignature->GetBufferPointer(),
        SerializeRootSignature->GetBufferSize(),
        IID_PPV_ARGS(&RootSignature));

    /*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/
    // 【着色器Shader】
    //—————————————————————————————————————————————————————————————————
    // 编译shader
    VertexShader.BuildShaders(L"../WandererEngine/Shader/Hello.hlsl", "VertexShaderMain", "vs_5_0");
    PixelShader.BuildShaders(L"../WandererEngine/Shader/Hello.hlsl", "PixelShaderMain", "ps_5_0");

    // 输入布局描述符，要求hlsl中的输入签名与之匹配
    InputLayoutDESC =
    {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
        {"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0}
    };

    /*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/
    // 【顶点/索引缓冲区】
    //—————————————————————————————————————————————————————————————————
    // 获取模型数据的大小
    VertexStrideInBytes = sizeof(FVertex);
    VertexSizeInBytes = InRenderingData->VertexData.size() * VertexStrideInBytes;
    IndexCount = InRenderingData->IndexData.size();
    IndexSizeInBytes = IndexCount * sizeof(uint16_t);

    // 创建CPU顶点/索引缓冲区
    ANALYSIS_HRESULT(D3DCreateBlob(VertexSizeInBytes, &CPUVertexBufferPtr));
    memcpy(CPUVertexBufferPtr->GetBufferPointer(), InRenderingData->VertexData.data(), VertexSizeInBytes);
    ANALYSIS_HRESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));
    memcpy(CPUIndexBufferPtr->GetBufferPointer(), InRenderingData->IndexData.data(), IndexSizeInBytes);

    // 创建GPU顶点/索引缓冲区
    GPUVertexBufferPtr = ConstructBuffer.ConstructDefaultBuffer(VertexUploadBufferPtr, InRenderingData->VertexData.data(), VertexSizeInBytes);
    GPUIndexBufferPtr = ConstructBuffer.ConstructDefaultBuffer(IndexUploadBufferPtr, InRenderingData->IndexData.data(), IndexSizeInBytes);

    /*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/
    // 【PSO流水线状态对象】
    //—————————————————————————————————————————————————————————————————
    // 流水线绑定
    D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc;
    memset(&GPSDesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
    // 绑定根签名
    GPSDesc.pRootSignature = RootSignature.Get();

    // 绑定着色器
    GPSDesc.VS.pShaderBytecode = reinterpret_cast<BYTE*>(VertexShader.GetBufferPointer());  // 顶点着色器
    GPSDesc.VS.BytecodeLength = VertexShader.GetBufferSize();
    GPSDesc.PS.pShaderBytecode = reinterpret_cast<BYTE*>(PixelShader.GetBufferPointer());   // 像素着色器
    GPSDesc.PS.BytecodeLength = PixelShader.GetBufferSize();
    // GPSDesc.DS // 绑定域着色器
    // GPSDesc.HS // 绑定外壳着色器
    // GPSDesc.HS // 绑定几何着色器
    // GPSDesc.StreamOutput // 实现流输出技术
    GPSDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);                 // 混合状态
    GPSDesc.SampleMask = UINT_MAX;                                          // 混合状态的示例掩码

    // 光栅器的光栅化状态
    GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;                             // 填充模式(固体/线框)
    //GPSDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;                                // 裁剪模式
    //GPSDesc.RasterizerState.FrontCounterClockwise = FALSE;                                  // 三角形顶点顺序逆时针为正面
    //GPSDesc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;                           // 深度偏差
    //GPSDesc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;                // 最大深度偏差
    //GPSDesc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;   // 给定像素斜率上的标量
    //GPSDesc.RasterizerState.DepthClipEnable = TRUE;                                         // 是否启用基于距离的剪裁
    //GPSDesc.RasterizerState.MultisampleEnable = FALSE;                                      // MSAA使用四边形还是 alpha 线抗锯齿算法，false为alpha线抗锯齿
    //GPSDesc.RasterizerState.AntialiasedLineEnable = FALSE;                                  // 是否启用行抗锯齿，MSAA关闭时才可使用
    //GPSDesc.RasterizerState.ForcedSampleCount = 0;                                          // UAV 渲染或栅格化时强制的样本计数
    //GPSDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF; // 标识保守栅格化是打开还是关闭

    GPSDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);                    // 深度/模板测试的状态
    GPSDesc.InputLayout.pInputElementDescs = InputLayoutDESC.data();                          // 输入布局描述
    GPSDesc.InputLayout.NumElements = static_cast<UINT>(InputLayoutDESC.size());
    GPSDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;                   // 指定图元拓扑类型
    GPSDesc.NumRenderTargets = 1;                                                             // 同时所用的RT数量，即RTVFormats数组中渲染目标格式的数量
    GPSDesc.RTVFormats[0] = GetEngine()->GetRenderingEngine()->GetBackBufferFormat();         // 渲染目标的格式
    GPSDesc.DSVFormat = GetEngine()->GetRenderingEngine()->GetDepthStencilFormat();           // 深度/模板缓冲区的格式
    GPSDesc.SampleDesc.Count = GetEngine()->GetRenderingEngine()->GetMSAASampleCount();       // 多重采样数量
    GPSDesc.SampleDesc.Quality = GetEngine()->GetRenderingEngine()->GetMSAASampleQuality();   // 多重采样质量级别
    // GPSDesc.NodeMask //单GPU设置为0

    ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO)));
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

    // 在顶点缓冲区及其对应视图创建完成后，将它与渲染流水线上的一个输入槽(input slot)相绑定。
    // 绑定后就能向流水线中的【输入装配器阶段】传递顶点数据了。
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
    GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, DescriptorHandle); // 令描述符表与渲染流水线绑定（⚠寄存器编号，描述符首地址）

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

    //构建mesh
    BuildMesh(&MeshData);
    MyMesh->Init();

    return MyMesh;
}