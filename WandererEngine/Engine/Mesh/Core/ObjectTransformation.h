#pragma once
#include "MeshType.h"

// 绘制对象所用的常量数据
struct FObjectTransformation
{
	FObjectTransformation();

	XMFLOAT4X4 MVP;

	static XMFLOAT4X4 IdentityMatrix4x4();
};
