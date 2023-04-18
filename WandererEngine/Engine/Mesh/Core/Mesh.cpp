#include "Mesh.h"
FObjectTransformation::FObjectTransformation()
    :World(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f)
{
}

FMesh::FMesh()
    : VertexStrideInBytes(0)
    , VertexSizeInBytes(0)
    , IndexCount(0)
    , IndexSizeInBytes(0)
    , IndexFormat(DXGI_FORMAT_R16_UINT)
{
}

void FMesh::Init()
{

}

void FMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
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
    objectConstants = make_shared<FRenderingResourceUpdate>();
    objectConstants->Init(GetD3dDevice().Get(), sizeof(FObjectTransformation), 1);
    D3D12_GPU_VIRTUAL_ADDRESS ObAddr = objectConstants.get()->GetBuffer()->GetGPUVirtualAddress();    // ����������ʼ��ַ(������Ϊ0���Ǹ������������ĵ�ַ)
    
    D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc;    // ����Ҫ�鿴�ĳ���������
    CBVDesc.BufferLocation = ObAddr;            
    CBVDesc.SizeInBytes = objectConstants->GetConstantBufferByteSize(); 
    
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
    VertexShader.BuildShaders(L"../WandererEngine/Shader/Hello.hlsl", "VertexShader", "vs_5_0");
    PixelShader.BuildShaders(L"../WandererEngine/Shader/Hello.hlsl", "PixelShader", "ps_5_0");

    // ���벼����������Ҫ��hlsl�е�����ǩ����֮ƥ��
    InputLayoutDESC =
    {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
        {"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,12}
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
    // �����벼��
    D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc;
    memset(&GPSDesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
    GPSDesc.InputLayout.pInputElementDescs = InputLayoutDESC.data();
    GPSDesc.InputLayout.NumElements = (UINT)InputLayoutDESC.size();
    // �󶨸�ǩ��
    GPSDesc.pRootSignature = RootSignature.Get();
    // �󶨶���/������ɫ������
    GPSDesc.VS.pShaderBytecode = reinterpret_cast<BYTE*>(VertexShader.GetBufferPointer());
    GPSDesc.VS.BytecodeLength = VertexShader.GetBufferSize();
    GPSDesc.PS.pShaderBytecode = reinterpret_cast<BYTE*>(PixelShader.GetBufferPointer());
    GPSDesc.PS.BytecodeLength = PixelShader.GetBufferSize();

    // ��դ��״̬
    GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);   //����ʹ��Ĭ��״̬��,���ԶԲ�����������
    GPSDesc.SampleMask = UINT_MAX;
    GPSDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    GPSDesc.NumRenderTargets = 1;

    GPSDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    GPSDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

    GPSDesc.SampleDesc.Count = GetEngine()->GetMSAASampleCount();
    GPSDesc.SampleDesc.Quality = GetEngine()->GetMSAASampleQuality();

    GPSDesc.RTVFormats[0] = GetEngine()->GetBackBufferFormat();
    GPSDesc.DSVFormat = GetEngine()->GetDepthStencilFormat();


    ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO)));

}

void FMesh::Draw(float DeltaTime)
{
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

    // ����������ʵ����ͼԪ
    GetGraphicsCommandList()->DrawIndexedInstanced(
        IndexCount,     // ÿ��ʵ����Ҫ���Ƶ���������
        1,              // ����ʵ������������ʾ����ʵ������
        0,              // ָ��������������ĳ��Ԫ�أ�������Ϊ��ʼ����
        0,              // �ڱ��λ��Ƶ��ö�ȡ����֮ǰ��Ϊÿ���������ϸ�����ֵ
        0);             // ����ʵ��������
}

FMesh* FMesh::CreateMesh(const FMeshRenderingData* InRenderingData)
{
    FMesh *InMesh = new FMesh(); 
    InMesh->BuildMesh(InRenderingData);
    return InMesh;
}

D3D12_VERTEX_BUFFER_VIEW FMesh::GetVertexBufferView()
{
    D3D12_VERTEX_BUFFER_VIEW VBV;
    VBV.BufferLocation = GPUVertexBufferPtr->GetGPUVirtualAddress();    // ��������ͼ�Ķ��㻺������Դ�����ַ
    VBV.SizeInBytes = VertexSizeInBytes;                                // ��������ͼ�Ķ��㻺������С���ֽڣ�
    VBV.StrideInBytes = VertexStrideInBytes;                            // ÿ������Ԫ����ռ���ֽ���
    return VBV;
}

D3D12_INDEX_BUFFER_VIEW FMesh::GetIndexBufferView()
{
    D3D12_INDEX_BUFFER_VIEW IBV;
    IBV.BufferLocation = GPUIndexBufferPtr->GetGPUVirtualAddress();
    IBV.SizeInBytes = IndexSizeInBytes;
    IBV.Format = IndexFormat;
    return IBV;
}


