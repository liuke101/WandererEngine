#pragma once
#include "../../../../Interface/DirectXDeviceInterface.h"
#include "Geometry/GeometryMap.h"
#include "../../../../Shader/Core/Shader.h"
#include "PipelineState/PipelineState.h"
#include "RootSignature/RootSignature.h"
#include "../../../../Core/Viewport/ViewportInfo.h"

class CMeshComponent;

/* 渲染管线 */
class FRenderingPipeline : public IDirectXDeviceInterface
{
public:
    FRenderingPipeline();

    // 构建Mesh
    void BuildMesh(CMeshComponent* InMesh, const FMeshRenderingData& MeshData);

    virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

    // 构建渲染管线
    void BuildPipeline();


    virtual void PreDraw(float DeltaTime);
    virtual void Draw(float DeltaTime);
    virtual void PostDraw(float DeltaTime);
protected:
    FPipelineState PipelineState;                       // 管线状态
    FRootSignature RootSignature;                       // 根签名
    FShader VertexShader;                               // 顶点着色器 
    FShader PixelShader;                                // 像素着色器
    vector<D3D12_INPUT_ELEMENT_DESC> InputLayoutDESC;   // 输入布局描述符

    FGeometryMap GeometryMap;

};
