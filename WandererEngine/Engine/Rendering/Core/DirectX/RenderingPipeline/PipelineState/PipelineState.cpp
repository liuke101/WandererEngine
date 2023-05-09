#include "PipelineState.h"
#include "../../../../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../../../../../Platform/Windows/WindowsEngine.h"

FPipelineState::FPipelineState()
    : PipelineState(Solid)
{
    PSO.insert(pair<int, ComPtr<ID3D12PipelineState>>(4, ComPtr<ID3D12PipelineState>())); // 4:固体
    PSO.insert(pair<int, ComPtr<ID3D12PipelineState>>(5, ComPtr<ID3D12PipelineState>())); // 5:线框
}

void FPipelineState::PreDraw(float DeltaTime)
{
    // 重置命令列表
    GetGraphicsCommandList()->Reset(GetCommandAllocator().Get(), PSO[static_cast<int>(PipelineState)].Get());
}

void FPipelineState::Draw(float DeltaTime)
{
    CaptureKeyboard();
}

void FPipelineState::PostDraw(float DeltaTime)
{
}

void FPipelineState::ResetGPSDesc()
{
    memset(&GPSDesc, 0, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));
}

void FPipelineState::BindInputLayout(const D3D12_INPUT_ELEMENT_DESC* InInputElementDescs, UINT InSize)
{
    // 输入布局描述
    GPSDesc.InputLayout.pInputElementDescs = InInputElementDescs;     
    GPSDesc.InputLayout.NumElements = InSize;
}

void FPipelineState::BindRootSignature(ID3D12RootSignature* InRootSignature)
{
    GPSDesc.pRootSignature = InRootSignature;
}

void FPipelineState::BindShader(const FShader& InVertexShader, const FShader& InPixelShader)
{
    // 绑定着色器
    GPSDesc.VS.pShaderBytecode = reinterpret_cast<BYTE*>(InVertexShader.GetBufferPointer());  // 顶点着色器
    GPSDesc.VS.BytecodeLength = InVertexShader.GetBufferSize();
    GPSDesc.PS.pShaderBytecode = reinterpret_cast<BYTE*>(InPixelShader.GetBufferPointer());   // 像素着色器
    GPSDesc.PS.BytecodeLength = InPixelShader.GetBufferSize();
    // GPSDesc.DS // 绑定域着色器
    // GPSDesc.HS // 绑定外壳着色器
    // GPSDesc.HS // 绑定几何着色器
}

void FPipelineState::BuildPSO()
{
    // GPSDesc.StreamOutput // 实现流输出技术
    GPSDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);                 // 混合状态
    GPSDesc.SampleMask = UINT_MAX;                                          // 混合状态的示例掩码

    // 光栅器的光栅化状态
    GPSDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    //GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;                           // 填充模式(固体/线框,默认为固体)
    //GPSDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;                                // 裁剪模式
    //GPSDesc.RasterizerState.FrontCounterClockwise = FALSE;                                  // 三角形顶点顺序逆时针为正面
    //GPSDesc.RasterizerState.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;                           // 深度偏差
    //GPSDesc.RasterizerState.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;                // 最大深度偏差
    //GPSDesc.RasterizerState.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;   // 给定像素斜率上的标量
    //GPSDesc.RasterizerState.DepthClipEnable = TRUE;                                         // 是否启用基于距离的剪裁
    //GPSDesc.RasterizerState.MultisampleEnable = FALSE;                                      // MSAA使用四边形还是 alpha 线抗锯齿算法，false为alpha线抗锯齿
    //GPSDesc.RasterizerState.AntialiasedLineEnable = FALSE;                                  // 是否启用行抗锯齿，MSAA关闭时才可使用
    //GPSDesc.RasterizerState.ForcedSampleCount = 0;                                          // UAV 渲染或栅格化时强制的样本计数
    //GPSDesc.RasterizerState.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF; // 标识保守栅格化是打开还是关闭

    GPSDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);                    // 深度/模板测试的状态
    GPSDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;                   // 指定图元拓扑类型
    GPSDesc.NumRenderTargets = 1;                                                             // 同时所用的RT数量，即RTVFormats数组中渲染目标格式的数量
    GPSDesc.RTVFormats[0] = GetEngine()->GetRenderingEngine()->GetBackBufferFormat();         // 渲染目标的格式
    GPSDesc.DSVFormat = GetEngine()->GetRenderingEngine()->GetDepthStencilFormat();           // 深度/模板缓冲区的格式
    GPSDesc.SampleDesc.Count = GetEngine()->GetRenderingEngine()->GetMSAASampleCount();       // 多重采样数量
    GPSDesc.SampleDesc.Quality = GetEngine()->GetRenderingEngine()->GetMSAASampleQuality();   // 多重采样质量级别
    // GPSDesc.NodeMask //单GPU设置为0

    // 填充模式：固体
    GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;                            
    ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO[Solid])));
    // 填充模式：线框
    GPSDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;                             
    ANALYSIS_HRESULT(GetD3dDevice()->CreateGraphicsPipelineState(&GPSDesc, IID_PPV_ARGS(&PSO[Wirefram])));
}

void FPipelineState::CaptureKeyboard()
{
    if (GetAsyncKeyState('4') & 0x8000)
    {
        PipelineState = Solid;
    }
    else if (GetAsyncKeyState('5') & 0x8000)
    {
        PipelineState = Wirefram;
    }
}
