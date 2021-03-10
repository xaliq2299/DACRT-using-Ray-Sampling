//
// Created by khalig on 08.03.21.
//

#ifndef MYRAYTRACER_AABB_H
#define MYRAYTRACER_AABB_H

#include "Vec3.h"

class AABB{
public:
    Vec3f minimum, maximum;

    AABB(Vec3f min, Vec3f max){
        minimum = min; maximum = max;
    }

    AABB() {}

    inline Vec3f get_min() { return minimum; }

    inline Vec3f get_max() { return maximum; }

    void print(){
        std::cout << "For the given AABB, minimum=" << minimum << ", maximum=" << maximum << '\n';
    }

    // todo: bool intersect to be changed
//    bool intersect(const Ray &r, float &entry, float &exit)
//    {
//        float tmin = (min[0] - r.m_origin[0]) / r.m_direction[0];
//        float tmax = (max[0] - r.m_origin[0]) / r.m_direction[0];
//
//        if (tmin > tmax) swap(tmin, tmax);
//
//        float tymin = (min[1] - r.m_origin[1]) / r.m_direction[1];
//        float tymax = (max[1] - r.m_origin[1]) / r.m_direction[1];
//
//        if (tymin > tymax) swap(tymin, tymax);
//
//        if ((tmin > tymax) || (tymin > tmax))
//            return false;
//
//        if (tymin > tmin)
//            tmin = tymin;
//
//        if (tymax < tmax)
//            tmax = tymax;
//
//        float tzmin = (min[2] - r.m_origin[2]) / r.m_direction[2];
//        float tzmax = (max[2] - r.m_origin[2]) / r.m_direction[2];
//
//        if (tzmin > tzmax) swap(tzmin, tzmax);
//
//        if ((tmin > tzmax) || (tzmin > tmax))
//            return false;
//
//        if (tzmin > tmin)
//            tmin = tzmin;
//
//        if (tzmax < tmax)
//            tmax = tzmax;
//        //entry = r.m_origin+ tmin*r.m_direction;
//        //exit  = r.m_origin+tmax*r.m_direction;
//        entry = tmin;
//        exit  = tmax;
//
//        return true;
//    }
};

#endif //MYRAYTRACER_AABB_H
