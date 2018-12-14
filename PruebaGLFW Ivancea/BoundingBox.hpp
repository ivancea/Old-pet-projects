#include "Vec.hpp"

#ifndef BOUNDINGBOX_HPP
#define BOUNDINGBOX_HPP

struct _BB{
    Vec3d v[8];
};

struct _AABB{
    Vec3d Min;
    Vec3d Max;

    Vec3d GetCenter();
};

#endif // BOUNDINGBOX_HPP
