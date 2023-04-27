#include "RenderingPipeline.h"
//#include "PipelineState/PipelineState.h"

FRenderingPipeline::FRenderingPipeline()
{
}

void FRenderingPipeline::BuildMesh(CMesh* InMesh, const FMeshRenderingData& MeshData)
{

    GeometryMap.BuildMesh(InMesh, MeshData);
}

void FRenderingPipeline::BuildPipeline()
{
    // start: ������Ⱦ����״̬����
    PipelineState.ResetGPSDesc();

    // ����+�󶨸�ǩ��
    RootSignature.BuildRootSignature();
    PipelineState.BindRootSignature(RootSignature.GetRootSignature());

    // ����+����ɫ��
    VertexShader.BuildShaders(L"../WandererEngine/Shader/Hello.hlsl", "VertexShaderMain", "vs_5_0");
    PixelShader.BuildShaders(L"../WandererEngine/Shader/Hello.hlsl", "PixelShaderMain", "ps_5_0");
    PipelineState.BindShader(VertexShader, PixelShader);

    // ����+�����벼��
    InputLayoutDESC =  // ���벼����������Ҫ��hlsl�е�����ǩ����֮ƥ��
    {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
        {"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0}
    };
    PipelineState.BindInputLayout(InputLayoutDESC.data(), InputLayoutDESC.size());

    // ����ģ��
    GeometryMap.Build();

    // end: ��������״̬����
    PipelineState.BuildPSO();
}
