#include "Mesh.h"
#include "../../Config/EngineRenderConfig.h"

FObjectTransformation::FObjectTransformation()
    :MVP(IdentifyMatrix4x4())
{
}

// 4x4��λ����
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

    // ͸��ͶӰ����
    XMMATRIX Project = XMMatrixPerspectiveFovLH(
        0.25f * XM_PI,   //FOV���û����Ʊ�ʾ�Ĵ�ֱ�ӳ���
        AspectRattio,    //�ݺ��=���/�߶�
        1.0f,            //�����ü�ƽ��ľ���
        1000.0f);        //��Զ�ü�ƽ��ľ���

    XMStoreFloat4x4(&ProjectionMatrix, Project);
}

void CMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
    /*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/
    // ��������������
    // ����CBV��������
    D3D12_DESCRIPTOR_HEAP_DESC CBVHeapDesc; // ������������
    CBVHeapDesc.NumDescriptors = 1;
    CBVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    CBVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    CBVHeapDesc.NodeMask = 0;
    GetD3dDevice()->CreateDescriptorHeap(&CBVHeapDesc, IID_PPV_ARGS(&CBVHeap));
    
    // ����CBV������
    objectConstants = make_shared<FRenderingResourcesUpdate>();
    objectConstants->Init(GetD3dDevice().Get(), sizeof(FObjectTransformation), 1);
    D3D12_GPU_VIRTUAL_ADDRESS ObAddr = objectConstants.get()->GetBuffer()->GetGPUVirtualAddress();    // ����������ʼ��ַ(������Ϊ0���Ǹ������������ĵ�ַ)

    D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc;    // ����Ҫ�鿴�ĳ���������
    CBVDesc.BufferLocation = ObAddr;            
    CBVDesc.SizeInBytes = objectConstants->GetConstantBufferByteSize(); 

    // BUG��ObAddr̫����
    GetD3dDevice()->CreateConstantBufferView(&CBVDesc,CBVHeap->GetCPUDescriptorHandleForHeapStart()); // �������ڷ�����Դ���ݵĳ�����������ͼ
           
    // ������������ֻ����һ��CBV
    // ��ͬ���͵���Դ�ᱻ�󶨵��ض��ļĴ����ۣ��Թ���ɫ��������ʡ�register(x#)��ʾ�Ĵ������ݵ���Դ����
    // t:��ɫ����Դ��ͼ  s:������  b��������������ͼ  #���Ĵ������
    // ��δ��봴����һ����������Ŀ���ǽ�����һ��CBV����������󶨵������������Ĵ���0����register(b0)
    CD3DX12_ROOT_PARAMETER RootParam[1];    // �����������Ǹ�������������������������
    CD3DX12_DESCRIPTOR_RANGE CBVTable;           
    CBVTable.Init(
        D3D12_DESCRIPTOR_RANGE_TYPE_CBV,    // ������������ͣ�ָ��CBV�ķ�Χ
        1,                                  // ���е�����������
        0);                                 // �������������������˻�ַ��ɫ���Ĵ���
    RootParam[0].InitAsDescriptorTable(
        1,                                  // ���������������
        &CBVTable);                         // ָ�����������������ָ��

    // ������ǩ��������һ����λ(ָ������ֻ����һ��CBV����������)
    CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(1, RootParam, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
    ComPtr<ID3D10Blob> SerializeRootSignature;
    ComPtr<ID3D10Blob> ErrorBlob;
    D3D12SerializeRootSignature(    // �����Ƚ���ǩ���������������л������ſɴ���CreateRootSignature���������Ǵ�����ǩ��
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

    /*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

    // ����ɫ��Shader��
    
    // ����shader
    VertexShader.BuildShaders(L"../WandererEngine/Shader/Hello.hlsl", "VertexShaderMain", "vs_5_0");
    PixelShader.BuildShaders(L"../WandererEngine/Shader/Hello.hlsl", "PixelShaderMain", "ps_5_0");

    // ���벼����������Ҫ��hlsl�е�����ǩ����֮ƥ��
    InputLayoutDESC =
    {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
        {"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0}
    };

    /*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

    // ������/������������
    // ��ȡģ�����ݵĴ�С
    VertexStrideInBytes = sizeof(FVertex);
    VertexSizeInBytes = InRenderingData->VertexData.size() * VertexStrideInBytes;
    IndexCount = InRenderingData->IndexData.size();
    IndexSizeInBytes = IndexCount * sizeof(uint16_t);
    
    // ����CPU����/����������
    ANALYSIS_HRESULT(D3DCreateBlob(VertexSizeInBytes, &CPUVertexBufferPtr));
    memcpy(CPUVertexBufferPtr->GetBufferPointer(), InRenderingData->VertexData.data(), VertexSizeInBytes);
    ANALYSIS_HRESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));
    memcpy(CPUIndexBufferPtr->GetBufferPointer(), InRenderingData->IndexData.data(), IndexSizeInBytes);

    // ����GPU����/����������
    GPUVertexBufferPtr = ConstructDefaultBuffer(VertexUploadBufferPtr, InRenderingData->VertexData.data(), VertexSizeInBytes);
    GPUIndexBufferPtr = ConstructDefaultBuffer(IndexUploadBufferPtr, InRenderingData->IndexData.data(), IndexSizeInBytes);

    /*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

    // ��PSO��ˮ��״̬����
    // ��ˮ�߰�
    D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc;
    memset(&GPSDesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
    // �󶨸�ǩ��
    GPSDesc.pRootSignature = RootSignature.Get();                           

    // ����ɫ��
    GPSDesc.VS.pShaderBytecode = reinterpret_cast<BYTE*>(VertexShader.GetBufferPointer());  // ������ɫ��
    GPSDesc.VS.BytecodeLength = VertexShader.GetBufferSize();
    GPSDesc.PS.pShaderBytecode = reinterpret_cast<BYTE*>(PixelShader.GetBufferPointer());   // ������ɫ��
    GPSDesc.PS.BytecodeLength = PixelShader.GetBufferSize();
    // GPSDesc.DS // ������ɫ��
    // GPSDesc.HS // �������ɫ��
    // GPSDesc.HS // �󶨼�����ɫ��
    // GPSDesc.StreamOutput // ʵ�����������
    GPSDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);                 // ���״̬
    GPSDesc.SampleMask = UINT_MAX;                                          // ���״̬��ʾ������

    // ��դ���Ĺ�դ��״̬
    GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);                       
    GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;                             // ���ģʽ(����/�߿�)
    //GPSDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;                                // �ü�ģʽ
    //GPSDesc.RasterizerState.FrontCounterClockwise = FALSE;                                  // �����ζ���˳����ʱ��Ϊ����
    //GPSDesc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;                           // ���ƫ��
    //GPSDesc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;                // ������ƫ��
    //GPSDesc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;   // ��������б���ϵı���
    //GPSDesc.RasterizerState.DepthClipEnable = TRUE;                                         // �Ƿ����û��ھ���ļ���
    //GPSDesc.RasterizerState.MultisampleEnable = FALSE;                                      // MSAAʹ���ı��λ��� alpha �߿�����㷨��falseΪalpha�߿����
    //GPSDesc.RasterizerState.AntialiasedLineEnable = FALSE;                                  // �Ƿ������п���ݣ�MSAA�ر�ʱ�ſ�ʹ��
    //GPSDesc.RasterizerState.ForcedSampleCount = 0;                                          // UAV ��Ⱦ��դ��ʱǿ�Ƶ���������
    //GPSDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF; // ��ʶ����դ���Ǵ򿪻��ǹر�


    GPSDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);  // ���/ģ����Ե�״̬
    GPSDesc.InputLayout.pInputElementDescs = InputLayoutDESC.data();        // ���벼������
    GPSDesc.InputLayout.NumElements = (UINT)InputLayoutDESC.size();
    GPSDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE; // ָ��ͼԪ��������
    GPSDesc.NumRenderTargets = 1;                                           // ͬʱ���õ�RT��������RTVFormats��������ȾĿ���ʽ������
    GPSDesc.RTVFormats[0] = GetEngine()->GetBackBufferFormat();             // ��ȾĿ��ĸ�ʽ
    GPSDesc.DSVFormat = GetEngine()->GetDepthStencilFormat();               // ���/ģ�建�����ĸ�ʽ
    GPSDesc.SampleDesc.Count = GetEngine()->GetMSAASampleCount();           // ���ز�������
    GPSDesc.SampleDesc.Quality = GetEngine()->GetMSAASampleQuality();       // ���ز�����������
    // GPSDesc.NodeMask //��GPU����Ϊ0
    
    ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO)));

}

void CMesh::PreDraw(float DeltaTime)
{
    // ���������б�
    ANALYSIS_HRESULT(GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), PSO.Get()));
}

void CMesh::Draw(float DeltaTime)
{
    //��CBV��/��ǩ�����õ������б�
    ID3D12DescriptorHeap* DescriptorHeap[] = { CBVHeap.Get() };
    GetGraphicsCommandList()->SetDescriptorHeaps(_countof(DescriptorHeap), DescriptorHeap); 
    GetGraphicsCommandList()->SetGraphicsRootSignature(RootSignature.Get());                

    // �ڶ��㻺���������Ӧ��ͼ������ɺ󣬽�������Ⱦ��ˮ���ϵ�һ�������(input slot)��󶨡�
    // �󶨺��������ˮ���еġ�����װ�����׶Ρ����ݶ��������ˡ�
    D3D12_VERTEX_BUFFER_VIEW VBV = GetVertexBufferView();
    GetGraphicsCommandList()->IASetVertexBuffers(
        0,                          // �ڰ󶨶�����㻺����ʱ�����õ���ʼ����ۣ�������һ�����㻺��������������˲�)������۹���16��������Ϊ0~15��
        1,                          // ������۰󶨵Ķ��㻺��������������ͼ��������ͼ������)�������ʼ����۵�����ֵΪk, ������Ҫ��n�����㻺��������ô��Щ��������                                      �����������k, k+1...k+n-1��󶨡�
        &VBV);                      // ָ�򶥵㻺������ͼ�����е�һ��Ԫ�ص�ָ��

    D3D12_INDEX_BUFFER_VIEW IBV = GetIndexBufferView();
    GetGraphicsCommandList()->IASetIndexBuffer(&IBV);

    // ָ��ͼԪ���ֳƻ�Ԫ�����ˣ���֪D3D����ö������ݱ�ʾ����ͼԪ
    GetGraphicsCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // ��������������Ⱦ��ˮ�߰�
    GetGraphicsCommandList()->SetGraphicsRootDescriptorTable(0, CBVHeap->GetGPUDescriptorHandleForHeapStart());

    // ����������ʵ����ͼԪ
    GetGraphicsCommandList()->DrawIndexedInstanced(
        IndexCount,     // ÿ��ʵ����Ҫ���Ƶ���������
        1,              // ����ʵ������������ʾ����ʵ������
        0,              // ָ��������������ĳ��Ԫ�أ�������Ϊ��ʼ����
        0,              // �ڱ��λ��Ƶ��ö�ȡ����֮ǰ��Ϊÿ���������ϸ�����ֵ
        0);             // ����ʵ��������
}

void CMesh::PostDraw(float DeltaTime)
{
    XMUINT3 MeshPos = XMUINT3(5.0f, 5.0f, 5.0f);

    XMVECTOR Pos = XMVectorSet(MeshPos.x, MeshPos.y, MeshPos.z, 1.0f);

    // �۲�任����
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

    objectConstants->Update(0, &ObjectTransformation);  //����hlsl�е�����

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
    VBV.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();    // ��������ͼ�Ķ��㻺������Դ�����ַ
    VBV.SizeInBytes = VertexSizeInBytes;                                // ��������ͼ�Ķ��㻺������С���ֽڣ�
    VBV.StrideInBytes = VertexStrideInBytes;                            // ÿ������Ԫ����ռ���ֽ���
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


