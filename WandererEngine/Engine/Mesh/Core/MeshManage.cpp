#include "MeshManage.h"
#include "../../Config/EngineRenderConfig.h"
#include "../BoxMesh.h"
#include "../ConeMesh.h"
#include "../CustomMesh.h"
#include "../CylinderMesh.h"
#include "../SphereMesh.h"
#include "../PlaneMesh.h"
#include "ObjectTransformation.h"
#include "../../Rendering/Core/RenderingResourcesUpdate.h"
#include "../../Rendering/Engine/DirectX/Core/DirectXRenderingEngine.h"
#include "../../Rendering/Core/Buffer/ConstructBuffer.h"
#include "../../Math/EngineMath.h"
#include "../../Core/Viewport/ViewportTransformation.h"

CMeshManage::CMeshManage()
{

}

void CMeshManage::Init()
{
    
}

void CMeshManage::BuildMesh()
{
    RenderingPipeline.BuildPipeline();
}

void CMeshManage::UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo)
{
    RenderingPipeline.UpdateCalculations(DeltaTime, ViewportInfo);
}

void CMeshManage::PreDraw(float DeltaTime)
{
    RenderingPipeline.PreDraw(DeltaTime);
}


void CMeshManage::Draw(float DeltaTime)
{
    RenderingPipeline.Draw(DeltaTime);
}

void CMeshManage::PostDraw(float DeltaTime)
{
    RenderingPipeline.PostDraw(DeltaTime);
}


CMesh* CMeshManage::CreateSphereMesh(float InRadius, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
    return CreateMesh<CSphereMesh>(InRadius, InAxialSubdivision, InHeightSubdivision);
}

CMesh* CMeshManage::CreateMesh(string& InPath)
{
    return CreateMesh<CCustomMesh>(InPath);
}

CMesh* CMeshManage::CreateBoxMesh(float InHeight, float InWidth, float InDepth)
{
    return CreateMesh<CBoxMesh>(InHeight, InWidth, InDepth);
}

CMesh* CMeshManage::CreateConeMesh(float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
    return CreateMesh<CConeMesh>(InBottomRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

CMesh* CMeshManage::CreateCylinderMesh(float InTopRadius, float InBottomRadius, float InHeight, uint32_t InAxialSubdivision, uint32_t InHeightSubdivision)
{
    return CreateMesh<CCylinderMesh>(InTopRadius, InBottomRadius, InHeight, InAxialSubdivision, InHeightSubdivision);
}

CMesh* CMeshManage::CreatePlaneMesh(float InHeight, float InWidth, uint32_t InHeightSubdivide, uint32_t InWidthSubdivide)
{
    return CreateMesh<CPlaneMesh>(InHeight, InWidth, InHeightSubdivide, InWidthSubdivide);
}

template<class T, typename ...ParamTypes>
T* CMeshManage::CreateMesh(ParamTypes && ...Params)
{
    T* MyMesh = new T();

    //提取模型资源
    FMeshRenderingData MeshData;
    MyMesh->CreateMesh(MeshData, forward<ParamTypes>(Params)...);

    MyMesh->BeginInit();

    //构建mesh
    RenderingPipeline.BuildMesh(MyMesh, MeshData);

    MyMesh->Init();

    return MyMesh;
}