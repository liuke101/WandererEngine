#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"
#include "../../../RenderingResourcesUpdate.h"
#include "../../../../../Shader/Core/Shader.h"

class CMesh;

/* 根签名 */
struct FRootSignature : public IDirectXDeviceInterface_Struct
{
public:
    FRootSignature();

    void BuildRootSignature();
    ID3D12RootSignature* GetRootSignature() { return RootSignature.Get(); }
private:
    ComPtr<ID3D12RootSignature> RootSignature; 
}