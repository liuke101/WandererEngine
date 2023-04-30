#pragma once
#include "../../EngineMinimal.h"

class FShader
{
public:
    virtual LPVOID GetBufferPointer();
    SIZE_T GetBufferSize();
    // ±àÒë×ÅÉ«Æ÷
    void BuildShaders(const wstring& InFileName, const string& InEntryFunName, const string& InShadersVersion);
private:
    ComPtr<ID3DBlob> ShaderCode;
};