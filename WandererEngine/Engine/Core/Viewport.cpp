#include "Viewport.h"
#include "../Mesh/Core/ObjectTransformation.h"
#include "../Config/EngineRenderConfig.h"

FViewport::FViewport()
    : ViewMatrix(FObjectTransformation::IdentityMatrix4x4())
    , ProjectionMatrix(FObjectTransformation::IdentityMatrix4x4())
{
    
}


void FViewport::ViewportInit()
{
    float AspectRattio = static_cast<float>(FEngineRenderConfig::GetRenderConfig()->ScreenWidth) / static_cast<float>(FEngineRenderConfig::GetRenderConfig()->ScreenHeight);

    // 透视投影矩阵
    XMMATRIX Project = XMMatrixPerspectiveFovLH(
        0.25f * XM_PI,   //FOV：用弧度制表示的垂直视场角
        AspectRattio,    //纵横比=宽度/高度
        1.0f,            //到近裁剪平面的距离
        1000.0f);        //到远裁剪平面的距离

    XMStoreFloat4x4(&ProjectionMatrix, Project);
}
