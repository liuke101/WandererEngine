#include "RenderingTextureResourcesUpdate.h"
const wchar_t DDS[] = L".dds";
const wchar_t Asset[] = L"/Asset/";
const wchar_t Project[] = L"/Project/";

FRenderingTexture::FRenderingTexture()
    : RenderingTextureID(0)
{
}

void FRenderingTextureResourcesUpdate::LoadTextureResources(const wstring& InFilename)
{
    unique_ptr<FRenderingTexture> MyTexture = std::make_unique<FRenderingTexture>();
    MyTexture->Filename = InFilename;

    //sd.txt
    wchar_t Filename[1024] = { 0 };
    get_path_clean_filename_w(Filename, MyTexture->Filename.c_str());
    wremove_string_start(Filename, DDS);

    MyTexture->Name = Filename;

    //读取DDS数据
    CreateDDSTextureFromFile12(
        GetD3dDevice().Get(),
        GetGraphicsCommandList().Get(),
        MyTexture->Filename.c_str(),
        MyTexture->Data,
        MyTexture->UploadBuffer);

    MyTexture->RenderingTextureID = TexturesMapping.size();

    //Texture'/Project/Texture/Hello.Hello'
    wchar_t AssetFilenameBuff[1024] = { 0 };
    {
        wchar_t AssetFilenameBuff1[1024] = { 0 };
        wchar_t* AssetFilenamePtr = const_cast<wchar_t*>(MyTexture->Filename.c_str());

        int Pos = wfind_string(AssetFilenamePtr, Asset);

        wchar_t* Value = wstring_mid(AssetFilenamePtr, AssetFilenameBuff1, Pos, wcslen(AssetFilenamePtr));

        wreplace_string_inline(Value, Asset, Project);
        wreplace_string_inline(Value, DDS, (L"." + MyTexture->Name).c_str());

        wget_printf_s(AssetFilenameBuff, L"Texture'%s'", Value);
    }

    MyTexture->AssetFilename = AssetFilenameBuff;

    TexturesMapping[MyTexture->Name] = std::move(MyTexture);
}

void FRenderingTextureResourcesUpdate::BuildSRV(ID3D12DescriptorHeap* InHeap, int Offset)
{
    // 获取指定类型描述符堆的句柄增量的大小作为偏移
    UINT DescriptorOffset = GetD3dDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    // 描述符句柄
    CD3DX12_CPU_DESCRIPTOR_HANDLE Handle(InHeap->GetCPUDescriptorHandleForHeapStart());
    Handle.Offset(Offset, DescriptorOffset);

    // 着色器资源（SRV）描述符
    D3D12_SHADER_RESOURCE_VIEW_DESC SRVDesc = {};           // 初始化
    SRVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;            // SRV描述符格式    
    SRVDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;  // 纹理的维数
    SRVDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING; //用于改变RGBA向量分量的顺序，这里用默认顺序
    SRVDesc.Texture2D.MostDetailedMip = 0;                  // 指定第一层Mipmap的索引
    SRVDesc.Texture2D.MipLevels = 1;                        // Mipmap层级数量
    SRVDesc.Texture2D.ResourceMinLODClamp = 0.0f;           // 可以访问的最小Mipmap的层级，0.0表示可以访问所有层级

    for (auto& temp : TexturesMapping)
    {
        SRVDesc.Format = temp.second->Data->GetDesc().Format;
        SRVDesc.Texture2D.MipLevels = temp.second->Data->GetDesc().MipLevels;
        GetD3dDevice()->CreateShaderResourceView(temp.second->Data.Get(), &SRVDesc, Handle);

        Handle.Offset(1, DescriptorOffset);
    }
}

std::unique_ptr<FRenderingTexture>* FRenderingTextureResourcesUpdate::FindRenderingTexture(const std::string& InKey)
{
    if (!InKey.empty())
    {
        std::unique_ptr<FRenderingTexture>* RenderingTexturePtr = nullptr;

        // 宽字符转化
        const char* InString = InKey.c_str();
        wchar_t TexturePath[1024] = { 0 };
        char_to_wchar_t(TexturePath, 1024, InString);

        if (TexturesMapping.find(TexturePath) != TexturesMapping.end())
        {
            return &TexturesMapping[TexturePath];
        }
        else
        {
            for (auto& temp : TexturesMapping)
            {
                if (temp.second->Filename == TexturePath)
                {
                    return &temp.second;
                }
                if (temp.second->AssetFilename == TexturePath)
                {
                    return &temp.second;
                }
                if (temp.second->SimpleAssetFilename == TexturePath)
                {
                    return &temp.second;
                }
            }
        }
    }

    return nullptr;
}
