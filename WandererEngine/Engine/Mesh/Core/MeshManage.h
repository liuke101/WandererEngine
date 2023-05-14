﻿#pragma once
#include "../../Core/CoreObject/CoreMinimalObject.h"
#include "../../Shader/Core/Shader.h"
#include "MeshType.h"
#include "Mesh.h"
#include "../../Interface/DirectXDeviceInterface.h"
#include "../../Core/Viewport/ViewportInfo.h"
#include "../../Rendering/Core/DirectX/RenderingPipeline/RenderingPipeline.h"

class FRenderingResourcesUpdate;

class CMeshManage
    : public CCoreMinimalObject
    , public IRenderingInterface
    , public IDirectXDeviceInterface
{
public:
	CMeshManage();

	virtual void Init();

	virtual void BuildMesh();

	virtual void UpdateCalculations(float DeltaTime, const FViewportInfo& ViewportInfo);

	virtual void PreDraw(float DeltaTime);
	virtual void Draw(float DeltaTime);
	virtual void PostDraw(float DeltaTime);


public:
	CMeshComponent* CreateBoxMeshComponent(
		float InHeight,
		float InWidth,
		float InDepth);

	CMeshComponent* CreateConeMeshComponent(
		float InBottomRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMeshComponent* CreateCylinderMeshComponent(
		float InTopRadius,
		float InBottomRadius,
		float InHeight,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMeshComponent* CreatePlaneMeshComponent(
		float InHeight,
		float InWidth,
		uint32_t InHeightSubdivide,
		uint32_t InWidthSubdivide);

	CMeshComponent* CreateSphereMeshComponent(
		float InRadius,
		uint32_t InAxialSubdivision,
		uint32_t InHeightSubdivision);

	CMeshComponent* CreateMeshComponent(string& InPath);

protected:
	template<class T, typename ...ParamTypes>
	T* CreateMeshComponent(ParamTypes &&...Params);

protected:
	FRenderingPipeline RenderingPipeline;   // 渲染管线
};
