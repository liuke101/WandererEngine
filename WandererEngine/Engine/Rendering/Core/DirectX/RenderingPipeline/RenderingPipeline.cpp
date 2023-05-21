#include "RenderingPipeline.h"

FRenderingPipeline::FRenderingPipeline()
{
}

void FRenderingPipeline::BuildMesh(CMeshComponent* InMesh, const FMeshRenderingData& MeshData)
{

    GeometryMap.BuildMesh(InMesh, MeshData);
}

void FRenderingPipeline::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
    GeometryMap.UpdateCalculations(DeltaTime, ViewportInfo);
}

void FRenderingPipeline::BuildPipeline()
{
    // start: 重置渲染管线状态描述
    PipelineState.ResetGPSDesc();

    // 读取纹理贴图
    GeometryMap.LoadTexture();

    // 构建+绑定根签名
    RootSignature.BuildRootSignature(GeometryMap.GetDrawTextureResourcesNumber());
    PipelineState.BindRootSignature(RootSignature.GetRootSignature());

    // 构建+绑定着色器（Shader）
    // CPU宏定义传递给GPU
    char TextureNumBuff[10] = { 0 };
    D3D_SHADER_MACRO ShaderMacro[] =
    {
        "TEXTURE2D_MAP_NUM",_itoa(GeometryMap.GetDrawTextureResourcesNumber(),TextureNumBuff,10),
        NULL,NULL,
    };


    VertexShader.BuildShaders(L"../WandererEngine/Shader/Hello.hlsl", "VertexShaderMain", "vs_5_1", ShaderMacro);
    PixelShader.BuildShaders(L"../WandererEngine/Shader/Hello.hlsl", "PixelShaderMain", "ps_5_1", ShaderMacro);
    PipelineState.BindShader(VertexShader, PixelShader);

    // 描述+绑定输入布局
    InputLayoutDESC =  // 输入布局描述符，要求hlsl中的输入签名与之匹配
    {
        {"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
        {"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
        {"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,28,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
        {"TANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,0,40,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
        {"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,52,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0}
    };

    PipelineState.BindInputLayout(InputLayoutDESC.data(), InputLayoutDESC.size());
    // 构建模型
    GeometryMap.Build();

    // 构建CBV堆（常量缓冲区描述符堆）
    GeometryMap.BuildDescriptorHeap();

    // 构建常量缓冲区和CBV
    GeometryMap.BuildMeshObjectCBV();
    GeometryMap.BuildMaterialSRV();
    GeometryMap.BuildLightObjectCBV();
    GeometryMap.BuildViewportCBV();

    // 构建SRV
    GeometryMap.BuildTextureSRV();

    // end: 构建管线状态对象
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


