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
    // start: 重置渲染管线状态描述
    PipelineState.ResetGPSDesc();

    // 构建+绑定根签名
    RootSignature.BuildRootSignature();
    PipelineState.BindRootSignature(RootSignature.GetRootSignature());

    // 构建+绑定着色器
    VertexShader.BuildShaders(L"../WandererEngine/Shader/Hello.hlsl", "VertexShaderMain", "vs_5_0");
    PixelShader.BuildShaders(L"../WandererEngine/Shader/Hello.hlsl", "PixelShaderMain", "ps_5_0");
    PipelineState.BindShader(VertexShader, PixelShader);

    // 描述+绑定输入布局
    InputLayoutDESC =  // 输入布局描述符，要求hlsl中的输入签名与之匹配
    {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
        {"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0}
    };
    PipelineState.BindInputLayout(InputLayoutDESC.data(), InputLayoutDESC.size());

    // 构建模型
    GeometryMap.Build();

    // end: 构建管线状态对象
    PipelineState.BuildPSO();
}
