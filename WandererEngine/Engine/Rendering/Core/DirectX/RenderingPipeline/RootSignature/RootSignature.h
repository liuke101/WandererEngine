#pragma once
#include "../../../../../Interface/DirectXDeviceInterface.h"

/* 根签名 */
struct FRootSignature  : public IDirectXDeviceInterface_Struct
{
public:
    FRootSignature();

    void PreDraw(float DeltaTime);

    void Draw(float DeltaTime);

    void PostDraw(float DeltaTime);

    void BuildRootSignature();

    ID3D12RootSignature* GetRootSignature() { return RootSignature.Get(); }
private:
    ComPtr<ID3D12RootSignature> RootSignature;
};