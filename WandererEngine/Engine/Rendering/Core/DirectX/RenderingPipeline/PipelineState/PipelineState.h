#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../../../../../Shader/Core/Shader.h"
#include "../RenderingPipelineType.h"
/* 管线状态 */
struct FPipelineState : public IDirectXDeviceInterface_Struct
{
public:
    FPipelineState();

    void PreDraw(float DeltaTime);
    void Draw(float DeltaTime);
    void PostDraw(float DeltaTime);

    // 重置管线状态描述
    void ResetGPSDesc();

    // 绑定输入布局
    void BindInputLayout(const D3D12_INPUT_ELEMENT_DESC* InInputElementDescs, UINT InSize);

    // 绑定根签名
    void BindRootSignature(ID3D12RootSignature* InRootSignature);

    // 绑定着色器
    void BindShader(const FShader& InVertexShader, const FShader& InPixelShader);

    // 构建管线状态对象
    void BuildPSO();

private:
    void CaptureKeyboard();     // 捕获键盘输入
private:
    unordered_map<int, ComPtr<ID3D12PipelineState>> PSO;    // PSO管线状态对象
    D3D12_GRAPHICS_PIPELINE_STATE_DESC GPSDesc;             // 管线状态描述
    EPipelineState PipelineState;                           // 枚举管线状态，用于切换管线状态
};