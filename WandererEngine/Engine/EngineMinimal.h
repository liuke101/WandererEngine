#pragma once
// �������ֿ�
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "../simple_library/public/simple_library.h"
#include "Debug/Log/SimpleLog.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")


#include <wrl.h>							// Windows�ṩ�Ŀ⣬����ComPtr��COM���������ָ�룩���¼���
#include <dxgi1_4.h>						// �ṩ��������swapChain���ӿ�
#include <D3Dcompiler.h>					// ������ɫ����
#include <DirectXMath.h>					// DirectXMath��ѧ��
#include <DirectXPackedVector.h>			// ������RGB���� 
#include <Directxcolors.h>					// ������ɫ
#include <Directxcollision.h>				// ��ײ
#include "Rendering/DirectX12/d3dx12.h"		// DX12 API
#include "Debug/EngineDebug.h"



using namespace Microsoft::WRL;				// wrl��������ռ�
using namespace DirectX;                    // DX��ѧ��������ռ�
using namespace DirectX::PackedVector;      // DX��ѧ��������ռ�

extern class CEngine* Engine;