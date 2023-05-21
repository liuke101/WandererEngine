#pragma once
#include "MeshType.h"

// 绘制对象所用的常量数据
struct FObjectTransformation
{
	FObjectTransformation();

	XMFLOAT4X4 M;

	XMFLOAT4X4 TextureTransformation;

	UINT MaterialIndex;
	UINT x1;
	UINT x2;
	UINT x3;
};
