
#include "CustomMesh.h"
#include "Core/MeshType.h"

void CCustomMesh::Init()
{
    Super::Init();
}

void CCustomMesh::BuildMesh(const FMeshRenderingData* InRenderingData)
{
    Super::BuildMesh(InRenderingData);
}

void CCustomMesh::Draw(float DeltaTime)
{
    Super::Draw(DeltaTime);
}

CCustomMesh* CCustomMesh::CreateMesh(string& InPath)
{
    FMeshRenderingData MeshData;

    // 获取文件大小
    unsigned int FileSize = get_file_size_by_filename(InPath.c_str());
    // 根据文件大小创建Buffer
    char* Buffer = new char[FileSize + 1];
    // 将Buffer初始化为0
    memset(Buffer, 0, FileSize + 1); 
    // 将文件内容存入Buffer
    get_file_buf(InPath.c_str(), Buffer);

    if(!LoadObjFromBuffer(Buffer, FileSize, MeshData))
    {

    }

    delete Buffer;

    /*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/
    // 【构建模型】
    CCustomMesh* CustomMesh = new CCustomMesh;
    CustomMesh->BuildMesh(&MeshData);
    CustomMesh->Init();

    return CustomMesh;
}

bool CCustomMesh::LoadObjFromBuffer(char* InBuffer, uint32_t InBufferSize, FMeshRenderingData& InMeshData)
{
    if (InBufferSize > 0)
    {
        stringstream BufferStream(InBuffer);
        char TempLine[256] = { 0 };
        string space; //空格

        for (; !BufferStream.eof();)
        {
            memset(TempLine, 0, 256);
            BufferStream.getline(TempLine, 256);
            if (strlen(TempLine) > 0)
            {
                // 判断第一个字符
                if (TempLine[0] == 'v')
                {
                    stringstream LineStream(TempLine);
                    LineStream >> space;

                    // 判断第二个字符
                    // vt为uv坐标
                    if (TempLine[1] == 't')
                    {

                    }
                    // vn为法线
                    else if (TempLine[1] == 'n')
                    {

                    }
                    // 第二个字符为空则判断是v为顶点坐标
                    else
                    {
                        // 先添加一个顶点进行初始化
                        InMeshData.VertexData.push_back(FVertex(
                            XMFLOAT3(), XMFLOAT4(Colors::White)));

                        // 拿到添加后的坐标
                        int TopIndex = InMeshData.VertexData.size() - 1;
                        XMFLOAT3 &VertexPos = InMeshData.VertexData[TopIndex].Position;

                        // 解析顶点坐标
                        LineStream >> VertexPos.x;
                        LineStream >> VertexPos.y;
                        LineStream >> VertexPos.z;
                    }
                }
                // f 即 face，每组数字依次表示 顶点v索引/纹理坐标vt索引/法线vn索引，每组三个顶点构成一个三角形
                else if (TempLine[0] == 'f')
                {
                    stringstream LineStream(TempLine);
                    LineStream >> space;

                    char SaveLineString[256] = { 0 };
                    char TempBuffer[256] = { 0 };
                    for (size_t i = 0; i < 3; i++)
                    {
                        memset(SaveLineString, 0, 256);

                        // 输入一行数据
                        LineStream >> SaveLineString;

                        // 找到索引位置
                        // v索引
                        int StringPosA = find_string(SaveLineString, "/", 0); 
                        memset(TempBuffer, 0, 256);
                        char* vIndex = string_mid(SaveLineString, TempBuffer, 0, StringPosA); 
                        InMeshData.IndexData.push_back(atoi(vIndex)-1);
                        // vt索引
                        int StringPosB = find_string(SaveLineString, "/", StringPosA + 1); 
                        memset(TempBuffer, 0, 256);
                        char* vtIndex = string_mid(SaveLineString, TempBuffer, StringPosA + 1, StringPosB - (StringPosA + 1));
                        // vn索引
                        memset(TempBuffer, 0, 256);
                        char* vnIndex = string_mid(SaveLineString, TempBuffer, StringPosB + 1, strlen(SaveLineString) - (StringPosB + 1));
                        
                    }
                }
            }
        }
    }
    return false;
}

