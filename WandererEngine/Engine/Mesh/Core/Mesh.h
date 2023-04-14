#pragma once
/* */
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"
class FMesh : public IRenderingInterface
{
public:
    virtual void Init();

    virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

    virtual void Draw(float DeltaTime);

    static FMesh *CreateMesh(const FMeshRenderingData * InRenderingData);

protected:
    ComPtr<ID3DBlob> CPUVertexBufferPrt;            // CPU���㻺����
    ComPtr<ID3DBlob> CPUIndexBufferPrt;             // CPU����������

    ComPtr<ID3D12Resource> GPUVertexBufferPrt;      // GPU���㻺����������Ĭ�϶ѣ�ֻ����GPU����
    ComPtr<ID3D12Resource> GPUIndexBufferPrt;       // GPU����������������Ĭ�϶ѣ�ֻ����GPU����

    ComPtr<ID3D12Resource> GPUVertexBufferTmpPrt;   // �ϴ����������������ݴ��ڴ渴�Ƶ��û��������ٴӸû��������Ƶ�GPU���㻺����
    ComPtr<ID3D12Resource> GPUIndexBufferTmpPrt;    // �ϴ�������
};