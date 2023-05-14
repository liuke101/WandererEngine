#pragma once
enum EMaterialType
{
    // 光照模型
    Lambert = 0,
    HalfLambert = 1,
    Phong = 2,
    BlinnPhong = 3,
    WrapLight = 4,
    Minnaert = 5,
    Banded = 6,


    // 单独显示
    BaseColor = 12,
    LocalNormal = 13,
    WorldNormal = 14,
    Fresnel = 100
};


enum EMaterialDisplayStatusType
{
    // 枚举顺序按照D3D_PRIMITIVE_TOPOLOGY对应顺序
    DefaultDisplay = 0,
    PointDisplay = 1, 
    WireframeDisplay = 2,

    TriangleDisplay = 4
};