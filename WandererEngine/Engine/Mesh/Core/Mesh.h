#pragma once
/* Mesh���� */
#include "../../Rendering/Core/Rendering.h"
#include "MeshType.h"

struct FObjectTransformation
{
    FObjectTransformation();
    XMFLOAT4X4 World;
};

class FMesh : public IRenderingInterface
{
public:
    virtual void Init();

    virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

    virtual void Draw(float DeltaTime);

    static FMesh* CreateMesh(const FMeshRenderingData * InRenderingData);

protected:
    ComPtr<ID3DBlob> CPUVertexBufferPrt;            // CPU���㻺�������洢��ϵͳ�ڴ��У�
    ComPtr<ID3DBlob> CPUIndexBufferPrt;             // CPU�������������洢��ϵͳ�ڴ��У�

    ComPtr<ID3D12Resource> GPUVertexBufferPrt;      // GPU���㻺����������Ĭ�϶ѣ�ֻ����GPU����
    ComPtr<ID3D12Resource> GPUIndexBufferPrt;       // GPU����������������Ĭ�϶ѣ�ֻ����GPU����

    ComPtr<ID3D12Resource> VertexUploadBufferPrt;   // �ϴ����������������ݴ��ڴ渴�Ƶ��û��������ٴӸû��������Ƶ�GPU���㻺����
    ComPtr<ID3D12Resource> IndexUploadBufferPrt;    // �ϴ�������

    ComPtr<ID3D12DescriptorHeap> CBVHeap;           // CBV�������ѣ�������������ͼ��Constant Buffer View��
    shared_ptr<FRenderingResourceUpdate> objectConstants;   // ������
};