#include "RootSignature.h"

FRootSignature::FRootSignature()
{
}

void FRootSignature::BuildRootSignature()
{
    // 创建描述符表
    CD3DX12_ROOT_PARAMETER RootParam[2];                        // ⚠描述符表大小=使用的寄存器槽数量
    // 创建ObjectCBV描述符表
    CD3DX12_DESCRIPTOR_RANGE ObjectCBVTable;
    ObjectCBVTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0); //（描述符表的类型，表中的描述符数量，⚠寄存器编号）
    // 创建ViewportCBV描述符表
    CD3DX12_DESCRIPTOR_RANGE ViewportCBVTable;
    ViewportCBVTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 1);

    //—————————————————————————————————————————————————————————————————
    // 根参数绑定
    // 根参数可以是根常量、根描述符、描述符表
    // 根参数将描述符表绑定到常量缓冲区寄存器register(x#)，以供着色器程序访问。register(x#)表示寄存器传递的资源类型
    // t:着色器资源视图  s:采样器  b：常量缓冲区视图  #：寄存器编号
    RootParam[0].InitAsDescriptorTable(1, &ObjectCBVTable); //（描述符区域的数量，指向描述符区域数组的指针）
    RootParam[1].InitAsDescriptorTable(1, &ViewportCBVTable);

    //—————————————————————————————————————————————————————————————————
    // 创建根签名
    // 根签名定义了drawcall之前，需要绑定到渲染管线上的资源以及这些资源应该如何映射到着色器的输入寄存器中
    CD3DX12_ROOT_SIGNATURE_DESC RootSignatureDesc(
        2,  // ⚠大小=使用的寄存器槽数量
        RootParam,
        0,
        nullptr,
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

    ComPtr<ID3D10Blob> SerializeRootSignature;
    ComPtr<ID3D10Blob> ErrorBlob;
    D3D12SerializeRootSignature(    // 必须先将根签名的描述进行序列化处理，才可传入CreateRootSignature方法，正是创建根签名
        &RootSignatureDesc,
        D3D_ROOT_SIGNATURE_VERSION_1,
        SerializeRootSignature.GetAddressOf(),
        ErrorBlob.GetAddressOf());

    if (ErrorBlob)
    {
        Engine_Log_Error("%s", reinterpret_cast<char*>(ErrorBlob->GetBufferPointer()));
    }

    GetD3dDevice()->CreateRootSignature(
        0,
        SerializeRootSignature->GetBufferPointer(),
        SerializeRootSignature->GetBufferSize(),
        IID_PPV_ARGS(&RootSignature));
}
