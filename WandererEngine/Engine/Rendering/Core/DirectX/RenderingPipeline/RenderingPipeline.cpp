#include "RenderingPipeline.h"

FRenderingPipeline::FRenderingPipeline()
{
}

void FRenderingPipeline::BuildMesh(GMesh* InMesh, const FMeshRenderingData& MeshData)
{

    GeometryMap.BuildMesh(InMesh, MeshData);
}

void FRenderingPipeline::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
    GeometryMap.UpdateCalculations(DeltaTime, ViewportInfo);
}

void FRenderingPipeline::BuildPipeline()
{
    // start: ������Ⱦ����״̬����
    PipelineState.ResetGPSDesc();

    // ����+�󶨸�ǩ��
    RootSignature.BuildRootSignature();
    PipelineState.BindRootSignature(RootSignature.GetRootSignature());

    // ����+����ɫ����Shader��
    VertexShader.BuildShaders(L"../WandererEngine/Shader/Hello.hlsl", "VertexShaderMain", "vs_5_0");
    PixelShader.BuildShaders(L"../WandererEngine/Shader/Hello.hlsl", "PixelShaderMain", "ps_5_0");
    PipelineState.BindShader(VertexShader, PixelShader);

    // ����+�����벼��
    InputLayoutDESC =  // ���벼����������Ҫ��hlsl�е�����ǩ����֮ƥ��
    {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
        {"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
        {"NORMAL",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,28,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0}
    };
    PipelineState.BindInputLayout(InputLayoutDESC.data(), InputLayoutDESC.size());

    // ����ģ��
    GeometryMap.Build();

    // ����CBV�ѣ������������������ѣ�
    GeometryMap.BuildCBVHeap();

    // ����������������CBV
    GeometryMap.BuildMeshObjectCBV();
    GeometryMap.BuildMaterialObjectCBV();
    GeometryMap.BuildLightObjectCBV();
    GeometryMap.BuildViewportCBV();

    // end: ��������״̬����
    PipelineState.BuildPSO();
}

void FRenderingPipeline::PreDraw(float DeltaTime)
{
    PipelineState.PreDraw(DeltaTime);
}

void FRenderingPipeline::Draw(float DeltaTime)
{
    GeometryMap.PreDraw(DeltaTime);
    RootSignature.PreDraw(DeltaTime);
    GeometryMap.Draw(DeltaTime);
    PipelineState.Draw(DeltaTime);
}

void FRenderingPipeline::PostDraw(float DeltaTime)
{
    GeometryMap.PostDraw(DeltaTime);
    PipelineState.PostDraw(DeltaTime);
}


