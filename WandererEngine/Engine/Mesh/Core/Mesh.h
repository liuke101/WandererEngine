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
    FMesh();

    virtual void Init();

    virtual void BuildMesh(const FMeshRenderingData* InRenderingData);

    virtual void Draw(float DeltaTime);

    static FMesh* CreateMesh(const FMeshRenderingData * InRenderingData);

    
    D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView(); // �������㻺������ͼ����RTV��ͬ�����贴����������
    D3D12_INDEX_BUFFER_VIEW GetIndexBufferView();   // ����������������ͼ
protected:
    ComPtr<ID3DBlob> CPUVertexBufferPtr;            // CPU���㻺�������洢��ϵͳ�ڴ��У�
    ComPtr<ID3DBlob> CPUIndexBufferPtr;             // CPU�������������洢��ϵͳ�ڴ��У�

    ComPtr<ID3D12Resource> GPUVertexBufferPtr;      // GPU���㻺����������Ĭ�϶ѣ�ֻ����GPU����
    ComPtr<ID3D12Resource> GPUIndexBufferPtr;       // GPU����������������Ĭ�϶ѣ�ֻ����GPU����

    ComPtr<ID3D12Resource> VertexUploadBufferPtr;   // �ϴ����������������ݴ��ڴ渴�Ƶ��û��������ٴӸû��������Ƶ�GPU���㻺����
    ComPtr<ID3D12Resource> IndexUploadBufferPtr;    // �ϴ�������

    ComPtr<ID3D12DescriptorHeap> CBVHeap;           // CBV�������ѣ�������������ͼ��Constant Buffer View��
    shared_ptr<FRenderingResourceUpdate> objectConstants;   // ������

    UINT VertexSizeInBytes;                         // ���㻺������ͼ����ͼ�Ķ��㻺������С���ֽ�)                                         
    UINT VertexStrideInBytes;                       // ���㻺������ͼ��ÿ������Ԫ����ռ���ֽ���
    UINT IndexSizeInBytes;                          // ������������ͼ����ͼ�Ķ��㻺������С���ֽ�)
    DXGI_FORMAT IndexFormat;                                    // ������������ͼ��������ʽ
};