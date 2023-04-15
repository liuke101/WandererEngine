#include "Mesh.h"

FObjectTransformation::FObjectTransformation()
    :World(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f)
{
}

void FMesh::Init()
{

}

void FMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
    /*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

    // CBV宙峰憲均
    D3D12_DESCRIPTOR_HEAP_DESC CBVHeapDesc;
    CBVHeapDesc.NumDescriptors = 1;
    CBVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
    CBVHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
    CBVHeapDesc.NodeMask = 0;
    GetD3dDevice()->CreateDescriptorHeap(&CBVHeapDesc, IID_PPV_ARGS(&CBVHeap));

    // 更秀械楚産喝曝
    objectConstants = make_shared<FRenderingResourceUpdate>();
    objectConstants->Init(GetD3dDevice().Get(), sizeof(FObjectTransformation), 1);
    D3D12_GPU_VIRTUAL_ADDRESS ObAddr = objectConstants.get()->GetBuffer()->GetGPUVirtualAddress();

    D3D12_CONSTANT_BUFFER_VIEW_DESC CBVDesc;
    CBVDesc.BufferLocation = ObAddr;
    CBVDesc.SizeInBytes = objectConstants->GetConstantBufferByteSize();

    GetD3dDevice()->CreateConstantBufferView(&CBVDesc,CBVHeap->GetCPUDescriptorHandleForHeapStart());

    /*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

    // 資函庁侏方象議寄弌
    UINT VertexSize = InRenderingData->VertexData.size() * sizeof(FVertex);
    UINT IndexSize = InRenderingData->IndexData.size() * sizeof(uint16_t);

    // 幹秀CPU競泣/沫哈産喝曝
    ANALYSIS_HRESULT(D3DCreateBlob(VertexSize, &CPUVertexBufferPrt));
    memcpy(CPUVertexBufferPrt->GetBufferPointer(), InRenderingData->VertexData.data(), VertexSize);
    ANALYSIS_HRESULT(D3DCreateBlob(IndexSize, &CPUIndexBufferPrt));
    memcpy(CPUIndexBufferPrt->GetBufferPointer(), InRenderingData->IndexData.data(), IndexSize);

    // 更秀GPU競泣/沫哈産喝曝
    GPUVertexBufferPrt = ConstructDefaultBuffer(VertexUploadBufferPrt, InRenderingData->VertexData.data(), VertexSize);
    GPUIndexBufferPrt = ConstructDefaultBuffer(IndexUploadBufferPrt, InRenderingData->IndexData.data(), IndexSize);

    /*！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！*/

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


