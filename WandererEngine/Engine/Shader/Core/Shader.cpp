#include "Shader.h"

LPVOID FShader::GetBufferPointer() const
{
    return ShaderCode->GetBufferPointer();
}

SIZE_T FShader::GetBufferSize() const
{
    return ShaderCode->GetBufferSize();
}

void FShader::BuildShaders(const wstring& InFileName, const string& InEntryFunName, const string& InShadersVersion)
{
    ComPtr<ID3DBlob> ErrorShaderMsg;
        
    HRESULT CompileFromFile = D3DCompileFromFile(          // 将HLSL代码编译为给定目标的字节码。
        InFileName.c_str(),                                // HLSL源代码文件名
        nullptr,                                           // 定义着色器宏 D3D_SHADER_MACRO 结构的可选数组，这里不使用
        D3D_COMPILE_STANDARD_FILE_INCLUDE,                 // 指向编译器用来处理包含文件的 ID3DInclude 接口的可选指针，这里指向默认包含处理程序的指针
        InEntryFunName.c_str(),                            // 着色器的入口函数名
        InShadersVersion.c_str(),                          // 着色器类型和版本
#if defined _DEBUG
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,   // 用调试模式来编译着色器 | 指示编译器跳过优化阶段（对调试很有用处）
#else
        0                                                  // 不使用调试模式
#endif
        0,                                                 // 指定如何编译效果文件，这里不使用
        &ShaderCode,                                       // 返回一个指向ID3DBlob数据结构的指针，它存储着编译好的着色器对象字节码
        &ErrorShaderMsg);                                  // 用于访问编译器错误信息，如果没有错误返回NULL

    if(ErrorShaderMsg)
    {
        Engine_Log_Error("%s", (char*)ErrorShaderMsg->GetBufferPointer());
    }

    ANALYSIS_HRESULT(CompileFromFile);
}
