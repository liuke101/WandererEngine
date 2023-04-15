#pragma once
// 包含各种库
#include <vector>

#include "../simple_library/public/simple_library.h"
#include "Debug/Log/SimpleLog.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")


#include <wrl.h>							// Windows提供的库，包含ComPtr（COM对象的智能指针）、事件等
#include <dxgi1_4.h>						// 提供交换链（swapChain）接口
#include <D3Dcompiler.h>					// 编译着色代码
#include <DirectXMath.h>					// DirectXMath数学库
#include <DirectXPackedVector.h>			// 向量、RGB计算 
#include <Directxcolors.h>					// 各种颜色
#include <Directxcollision.h>				// 碰撞
#include "Rendering/DirectX12/d3dx12.h"		// DX12 API
#include "Debug/EngineDebug.h"



using namespace Microsoft::WRL;				// wrl库的命名空间
using namespace DirectX;                    // DX数学库的命名空间
using namespace DirectX::PackedVector;      // DX数学库的命名空间

extern class FEngine* Engine;