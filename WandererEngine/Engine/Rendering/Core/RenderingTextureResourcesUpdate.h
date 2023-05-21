﻿#pragma once
#include "../../EngineMinimal.h"
#include "../../Interface/DirectXDeviceInterface.h"
struct FRenderingTexture
{
    FRenderingTexture();
    ComPtr<ID3D12Resource> UploadBuffer;
    ComPtr<ID3D12Resource> Data;

    wstring Name;        // 贴图名称
    wstring Filename;        // 路径
    wstring AssetFilename;       // 资源路径Textrue'Asset/Texture/test.png'
    wstring SimpleAssetFilename; // 简单的资源路径 Asset/Texture/test.png
    UINT RenderingTextureID;
};
class FRenderingTextureResourcesUpdate
    : public enable_shared_from_this<FRenderingTextureResourcesUpdate>
    , public IDirectXDeviceInterface
{
public:
    void LoadTextureResources(const wstring& InFilename);
    void BuildSRV(ID3D12DescriptorHeap* InHeap, int Offset = 0);

    UINT GetTextureMappingSize() const { return TexturesMapping.size(); };

    std::unique_ptr<FRenderingTexture>* FindRenderingTexture(const std::string &InKey);
protected:
    std::unordered_map<std::wstring, std::unique_ptr<FRenderingTexture>> TexturesMapping;
};