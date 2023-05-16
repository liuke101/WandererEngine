#include "LightConstantBuffer.h"
#include "LightType.h"

FLightConstantBuffer::FLightConstantBuffer()
{
    memset(&SceneLight, 0, sizeof(SceneLight));
}

FLight::FLight()
    : FalloffStart(0.0f)
    , FalloffEnd(10.0f)
    , LightType(ParallelLight)
{
}
