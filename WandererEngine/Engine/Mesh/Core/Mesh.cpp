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
    : VertexSizeInBytes(0)
    , VertexStrideInBytes(0)
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

    // CBV��������
    D3D12_DESCRIPTOR_HEAP_DESC CBVHeapDesc;
    CBVHeapDesc.NumDescriptors = 1;
    CBVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    CBVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    CBVHeapDesc.NodeMask = 0;
    GetD3dDevice()->CreateDescriptorHeap(&CBVHeapDesc, IID_PPV_ARGS(&CBVHeap));
    
    // ��������������
    objectConstants = make_shared<FRenderingResourceUpdate>();
    objectConstants->Init(GetD3dDevice().Get(), sizeof(FObjectTransformation), 1);
    D3D12_GPU_VIRTUAL_ADDRESS ObAddr = objectConstants.get()->GetBuffer()->GetGPUVirtualAddress();
    
    D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc;
    CBVDesc.BufferLocation = ObAddr;
    CBVDesc.SizeInBytes = objectConstants->GetConstantBufferByteSize();
    
    GetD3dDevice()->CreateConstantBufferView(&CBVDesc,CBVHeap->GetCPUDescriptorHandleForHeapStart());

    /*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

    // ��ȡģ�����ݵĴ�С
    VertexSizeInBytes = InRenderingData->VertexData.size() * sizeof(FVertex);
    VertexStrideInBytes = sizeof(FVertex);
    IndexSizeInBytes = InRenderingData->IndexData.size() * sizeof(uint16_t);

    // ����CPU����/����������
    ANALYSIS_HRESULT(D3DCreateBlob(VertexSizeInBytes, &CPUVertexBufferPtr));
    memcpy(CPUVertexBufferPtr->GetBufferPointer(), InRenderingData->VertexData.data(), VertexSizeInBytes);
    ANALYSIS_HRESULT(D3DCreateBlob(IndexSizeInBytes, &CPUIndexBufferPtr));
    memcpy(CPUIndexBufferPtr->GetBufferPointer(), InRenderingData->IndexData.data(), IndexSizeInBytes);

    GPUVertexBufferPtr = ConstructDefaultBuffer(VertexUploadBufferPtr, InRenderingData->VertexData.data(), VertexSizeInBytes);
    GPUIndexBufferPtr = ConstructDefaultBuffer(IndexUploadBufferPtr, InRenderingData->IndexData.data(), IndexSizeInBytes);

    /*����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������*/

}

void FMesh::Draw(float DeltaTime)
{
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


