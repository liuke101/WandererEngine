#include "Shader.h"

LPVOID FShader::GetBufferPointer() const
{
    return ShaderCode->GetBufferPointer();
}

SIZE_T FShader::GetBufferSize() const
{
    return ShaderCode->GetBufferSize();
}

void FShader::BuildShaders(
    const wstring& InFileName, 
    const string& InEntryFunName, 
    const string& InShadersVersion,
    const D3D_SHADER_MACRO* InShaderMacro)
{
    ComPtr<ID3DBlob> ErrorShaderMsg;
        
    HRESULT CompileFromFile = D3DCompileFromFile(          // ��HLSL�������Ϊ����Ŀ����ֽ��롣
        InFileName.c_str(),                                // HLSLԴ�����ļ���
        InShaderMacro,                                     // ������ɫ���� D3D_SHADER_MACRO �ṹ�Ŀ�ѡ����
        D3D_COMPILE_STANDARD_FILE_INCLUDE,                 // ָ�������������������ļ��� ID3DInclude �ӿڵĿ�ѡָ�룬����ָ��Ĭ�ϰ�����������ָ��
        InEntryFunName.c_str(),                            // ��ɫ������ں�����
        InShadersVersion.c_str(),                          // ��ɫ�����ͺͰ汾
#if defined _DEBUG
        D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,   // �õ���ģʽ��������ɫ�� | ָʾ�����������Ż��׶Σ��Ե��Ժ����ô���
#else
        0                                                  // ��ʹ�õ���ģʽ
#endif
        0,                                                 // ָ����α���Ч���ļ������ﲻʹ��
        &ShaderCode,                                       // ����һ��ָ��ID3DBlob���ݽṹ��ָ�룬���洢�ű���õ���ɫ�������ֽ���
        &ErrorShaderMsg);                                  // ���ڷ��ʱ�����������Ϣ�����û�д��󷵻�NULL

    if(ErrorShaderMsg)
    {
        Engine_Log_Error("%s", (char*)ErrorShaderMsg->GetBufferPointer());
    }

    ANALYSIS_HRESULT(CompileFromFile);
}
