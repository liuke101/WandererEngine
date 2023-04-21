#include "Mesh.h"
#include "../../Config/EngineRenderConfig.h"

FObjectTransformation::FObjectTransformation()
    :MVP(IdentifyMatrix4x4())
{
}

// 4x4单位矩阵
XMFLOAT4X4 FObjectTransformation::IdentifyMatrix4x4()
{
    return XMFLOAT4X4{
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};
}

CMesh::CMesh()
    : VertexStrideInBytes(0)
    , VertexSizeInBytes(0)
    , IndexCount(0)
    , IndexSizeInBytes(0)
    , IndexFormat(DXGI_FORMAT_R16_UINT)
    , ModelMatrix(FObjectTransformation::IdentifyMatrix4x4())
    , ViewMatrix(FObjectTransformation::IdentifyMatrix4x4())
    , ProjectionMatrix(FObjectTransformation::IdentifyMatrix4x4())
{

}


void CMesh::Init()
{
    float AspectRattio = static_cast<float>(FEngineRenderConfig::GetRenderConfig()->ScreenWidth) / static_cast<float>(FEngineRenderConfig::GetRenderConfig()->ScreenHeight);

    // 透视投影矩阵
    XMMATRIX Project = XMMatrixPerspectiveFovLH(
        0.25f * XM_PI,   //FOV：用弧度制表示的垂直视场角
        AspectRattio,    //纵横比=宽度/高度
        1.0f,            //到近裁剪平面的距离
        1000.0f);        //到远裁剪平面的距离

    XMStoreFloat4x4(&ProjectionMatrix, Project);
}

void CMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
    /*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/
    // 【常量缓冲区】
    // 创建CBV描述符堆
    D3D12_DESCRIPTOR_HEAP_DESC CBVHeapDesc; // 描述符堆描述
    CBVHeapDesc.NumDescriptors = 1;
    CBVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    CBVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    CBVHeapDesc.NodeMask = 0;
    GetD3dDevice()->CreateDescriptorHeap(&CBVHeapDesc, IID_PPV_ARGS(&CBVHeap));
    
    // 创建CBV描述符
    objectConstants = make_shared<FRenderingResourcesUpdate>();
    objectConstants->Init(GetD3dDevice().Get(), sizeof(FObjectTransformation), 1);
    D3D12_GPU_VIRTUAL_ADDRESS ObAddr = objectConstants.get()->GetBuffer()->GetGPUVirtualAddress();    // 缓冲区的起始地址(即索引为0的那个常量缓冲区的地址)

    D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc;    // 描述要查看的常量缓冲区
    CBVDesc.BufferLocation = ObAddr;            
    CBVDesc.SizeInBytes = objectConstants->GetConstantBufferByteSize(); 

    // BUG：ObAddr太大了
    GetD3dDevice()->CreateConstantBufferView(&CBVDesc,CBVHeap->GetCPUDescriptorHandleForHeapStart()); // 创建用于访问资源数据的常量缓冲区视图
           
    // 创建描述符表，只存有一个CBV
    // 不同类型的资源会被绑定到特定的寄存器槽，以供着色器程序访问。register(x#)表示寄存器传递的资源类型
    // t:着色器资源视图  s:采样器  b：常量缓冲区视图  #：寄存器编号
    // 这段代码创建了一个根参数，目的是将含有一个CBV的描述符表绑定到常量缓冲区寄存器0，即register(b0)
    CD3DX12_ROOT_PARAMETER RootParam[1];    // 根参数可以是根常量、根描述符、描述符表
    CD3DX12_DESCRIPTOR_RANGE CBVTable;           
    CBVTable.Init(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,    // 描述符表的类型，指定CBV的范围
        1,                                  // 表中的描述符数量
        0);                                 // 将这段描述符区域绑定至此基址着色器寄存器
    RootParam[0].InitAsDescriptorTable(
        1,                                  // 描述符区域的数量
        &CBVTable);                         // 指向描述符区域数组的指针

    // 创建根签名，仅含一个槽位(指向上述只存有一个CBV的描述符表)
    CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(1, RootParam, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
    ComPtr<ID3D10Blob> SerializeRootSignature;
    ComPtr<ID3D10Blob> ErrorBlob;
    D3D12SerializeRootSignature(    // 必须先将根签名的描述进行序列化处理，才可传入CreateRootSignature方法，正是创建根签名
        &RootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        SerializeRootSignature.GetAddressOf(),
        ErrorBlob.GetAddressOf());

    if(ErrorBlob)
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
    GPUVertexBufferPtr = ConstructDefaultBuffer(VertexUploadBufferPtr, InRenderingData->VertexData.data(), VertexSizeInBytes);
    GPUIndexBufferPtr = ConstructDefaultBuffer(IndexUploadBufferPtr, InRenderingData->IndexData.data(), IndexSizeInBytes);

    /*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/

    // 【PSO流水线状态对象】
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


    GPSDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);  // 深度/模板测试的状态
    GPSDesc.InputLayout.pInputElementDescs = InputLayoutDESC.data();        // 输入布局描述
    GPSDesc.InputLayout.NumElements = (UINT)InputLayoutDESC.size();
    GPSDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // 指定图元拓扑类型
    GPSDesc.NumRenderTargets = 1;                                           // 同时所用的RT数量，即RTVFormats数组中渲染目标格式的数量
    GPSDesc.RTVFormats[0] = GetEngine()->GetBackBufferFormat();             // 渲染目标的格式
    GPSDesc.DSVFormat = GetEngine()->GetDepthStencilFormat();               // 深度/模板缓冲区的格式
    GPSDesc.SampleDesc.Count = GetEngine()->GetMSAASampleCount();           // 多重采样数量
    GPSDesc.SampleDesc.Quality = GetEngine()->GetMSAASampleQuality();       // 多重采样质量级别
    // GPSDesc.NodeMask //单GPU设置为0
    
    ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO)));

}

void CMesh::PreDraw(float DeltaTime)
{
    // 重置命令列表
    ANALYSIS_HRESULT(GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), PSO.Get()));
}

void CMesh::Draw(float DeltaTime)
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

    // 令描述符表与渲染流水线绑定
    GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, CBVHeap->GetGPUDescriptorHandleForHeapStart());

    // 绘制索引，实例化图元
    GetGraphicsCommandList()->DrawIndexedInstanced(
        IndexCount,     // 每个实例将要绘制的索引数量
        1,              // 用于实例化技术，表示绘制实例数量
        0,              // 指向索引缓冲区的某个元素，将其标记为起始索引
        0,              // 在本次绘制调用读取顶点之前，为每个索引加上该整数值
        0);             // 用于实例化技术
}

void CMesh::PostDraw(float DeltaTime)
{
    XMUINT3 MeshPos = XMUINT3(5.0f, 5.0f, 5.0f);

    XMVECTOR Pos = XMVectorSet(MeshPos.x, MeshPos.y, MeshPos.z, 1.0f);

    // 观察变换矩阵
    XMVECTOR ViewTarget = XMVectorZero();
    XMVECTOR ViewUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMMATRIX ViewLookAt = XMMatrixLookAtLH(Pos, ViewTarget, ViewUp);
    XMStoreFloat4x4(&ViewMatrix, ViewLookAt);

    // MVP
    XMMATRIX M_Model = XMLoadFloat4x4(&ModelMatrix);
    XMMATRIX M_View = ViewLookAt;
    XMMATRIX M_Projection = XMLoadFloat4x4(&ProjectionMatrix);
    XMMATRIX M_MVP = M_Model * M_View * M_Projection;

    FObjectTransformation ObjectTransformation;
    XMStoreFloat4x4(&ObjectTransformation.MVP, XMMatrixTranspose(M_MVP));

    objectConstants->Update(0, &ObjectTransformation);  //更新hlsl中的数据

}

CMesh* CMesh::CreateMesh(const FMeshRenderingData* InRenderingData)
{
    CMesh *InMesh = new CMesh(); 
    InMesh->BuildMesh(InRenderingData);
    return InMesh;
}

D3D12_VERTEX_BUFFER_VIEW CMesh::GetVertexBufferView()
{
    D3D12_VERTEX_BUFFER_VIEW VBV;
    VBV.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();    // 待创建视图的顶点缓冲区资源虚拟地址
    VBV.SizeInBytes = VertexSizeInBytes;                                // 待创建视图的顶点缓冲区大小（字节）
    VBV.StrideInBytes = VertexStrideInBytes;                            // 每个顶点元素所占的字节数
    return VBV;
}

D3D12_INDEX_BUFFER_VIEW CMesh::GetIndexBufferView()
{
    D3D12_INDEX_BUFFER_VIEW IBV;
    IBV.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();
    IBV.SizeInBytes = IndexSizeInBytes;
    IBV.Format = IndexFormat;
    return IBV;
}


