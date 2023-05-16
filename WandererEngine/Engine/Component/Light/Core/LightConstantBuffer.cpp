#include "LightConstantBuffer.h"
#include "LightType.h"

FLightConstantBuffer::FLightConstantBuffer()
{
    memset(&SceneLights, 0, sizeof(SceneLights));
}

FLight::FLight()
    : FalloffStart(0.0f)
    , FalloffEnd(10.0f)
    , LightType(ParallelLight)
{
}
