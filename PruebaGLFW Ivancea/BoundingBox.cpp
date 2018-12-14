#include "BoundingBox.hpp"
#include "Vec.hpp"

_AABB BB_to_AABB(_BB BB, Vec3d Rotacion){
    // PFF
}

Vec3d _AABB::GetCenter(){
    return (Max+Min)/2;
}
