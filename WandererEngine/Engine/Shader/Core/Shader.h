#pragma once
#include "../../EngineMinimal.h"

class FShader
{
public:
    virtual LPVOID GetBufferPointer() const;
    SIZE_T GetBufferSize() const;
    // ������ɫ��
    void BuildShaders(
        const wstring& InFileName, 
        const string& InEntryFunName, 
        const string& InShadersVersion,
        const D3D_SHADER_MACRO* InShaderMacro = NULL);
private:
    ComPtr<ID3DBlob> ShaderCode;
};


