﻿#include "PlaneMeshComponent.h"
#include "../../Mesh/Core/MeshType.h"

CPlaneMeshComponent::CPlaneMeshComponent()
{

}


void CPlaneMeshComponent::CreateMesh(FMeshRenderingData& MeshData, float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide)
{
    // 获取细分后每段的属性值
    auto SubdivideValue = [&](float InValue, uint32_t InSubdivideValue)->float
    {
        // 细分值小于等于1时，不进行细分。细分值为2时，细分一次，以此类推...
        if (InSubdivideValue <= 1)
        {
            return InValue;
        }

        return InValue / (static_cast<float>(InSubdivideValue) - 1);
    };

    // 除以2便于将模型设置为以世界坐标原点为中心
    float Height = 0.5f * InHeight;
    float Width = 0.5 * InWidth;

    // 获取细分后每段的属性值
    float HeightSubdivideValue = SubdivideValue(InHeight, InHeightSubdivide);
    float WidthSubdivideValue = SubdivideValue(InWidth, InWidthSubdivide);

    // 用于展uv
    float HorizontalAverageSubdivision = 1.0f / (static_cast<float>(InWidthSubdivide) - 1.0f);
    float VerticalAverageSubdivision = 1.0f / (static_cast<float>(InHeightSubdivide) - 1.0f);

    /*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/
    // 【构建顶点】
    for (uint32_t i = 0; i < InHeightSubdivide; ++i)
    {
        float z = Height - static_cast<float>(i) * HeightSubdivideValue;
        for (uint32_t j = 0; j < InWidthSubdivide; ++j)
        {
            float x = Width - static_cast<float>(j) * WidthSubdivideValue;
            MeshData.VertexData.push_back(FVertex(
                XMFLOAT3(
                    x,   // x
                    0.0f,   // y
                    z),  // z
                XMFLOAT4(Colors::White),
                XMFLOAT3(0.0f, 1.0f, 0.0f),
                XMFLOAT2(VerticalAverageSubdivision * i,HorizontalAverageSubdivision * j))); //uv
        }
    }

    /*———————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————————*/
    // 【构建索引】
    for (uint32_t i = 0; i < InHeightSubdivide - 1; ++i)
    {
        for (uint32_t j = 0; j < InWidthSubdivide - 1; ++j)
        {
            // 绘制四边形（由两个三角形组成）
            // 三角形1
            MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j);
            MeshData.IndexData.push_back(i * InWidthSubdivide + j + 1);
            MeshData.IndexData.push_back(i * InWidthSubdivide + j);

            // 三角形2
            MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j + 1);
            MeshData.IndexData.push_back(i * InWidthSubdivide + j + 1);
            MeshData.IndexData.push_back((i + 1) * InWidthSubdivide + j);
        }
    }
}
