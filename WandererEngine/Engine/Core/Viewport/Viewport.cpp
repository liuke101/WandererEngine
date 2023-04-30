#include "Viewport.h"
#include "../../Mesh/Core/ObjectTransformation.h"
#include "../../Config/EngineRenderConfig.h"
#include "../../Math/EngineMath.h"

FViewport::FViewport()
    : ViewMatrix(EngineMath::IdentityMatrix4x4())
    , ProjectionMatrix(EngineMath::IdentityMatrix4x4())
{
    
}


void FViewport::ViewportInit()
{
    float AspectRattio = static_cast<float>(FEngineRenderConfig::GetRenderConfig()->ScreenWidth) / static_cast<float>(FEngineRenderConfig::GetRenderConfig()->ScreenHeight);

    // ͸��ͶӰ����
    XMMATRIX Project = XMMatrixPerspectiveFovLH(
        0.25f * XM_PI,   //FOV���û����Ʊ�ʾ�Ĵ�ֱ�ӳ���
        AspectRattio,    //�ݺ��=���/�߶�
        1.0f,            //�����ü�ƽ��ľ���
        1000.0f);        //��Զ�ü�ƽ��ľ���

    XMStoreFloat4x4(&ProjectionMatrix, Project);
}
