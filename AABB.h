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

    AABB(Mesh& mesh){
        std::vector<Triangle> indexedTriangles = mesh.indexedTriangles();
        std::vector<Vec3f> vertexPositions = mesh.vertexPositions();

//        float min_x = numeric_limits<float>::max(), min_y = numeric_limits<float>::max(), min_z = numeric_limits<float>::max();
//        float max_x = -numeric_limits<float>::min(), max_y = -numeric_limits<float>::min(), max_z = -numeric_limits<float>::min();
        float min_x = vertexPositions[0][0], min_y = vertexPositions[0][1], min_z = vertexPositions[0][2];
        float max_x = vertexPositions[0][0], max_y = vertexPositions[0][1], max_z = vertexPositions[0][2];
        for(size_t ver_ind=1;ver_ind<vertexPositions.size();ver_ind++){
            float cur_x = vertexPositions[ver_ind][0];
            float cur_y = vertexPositions[ver_ind][1];
            float cur_z = vertexPositions[ver_ind][2];

            if(cur_x < min_x){
                min_x = cur_x;
            }
            if(cur_y < min_y){
                min_y = cur_y;
            }
            if(cur_z < min_z){
                min_z = cur_z;
            }

            if(cur_x > max_x){
                max_x = cur_x;
            }
            if(cur_y > max_y){
                max_y = cur_y;
            }
            if(cur_z > max_z){
                max_z = cur_z;
            }
        }

        minimum = {min_x, min_y, min_z};
        maximum = {max_x, max_y, max_z};
    }

    AABB() {}

    inline Vec3f get_min() { return minimum; }

    inline Vec3f get_max() { return maximum; }

    void print(){
        std::cout << "For the given AABB, minimum=" << minimum << ", maximum=" << maximum << '\n';
    }

    bool intersect(const Ray &r, float &entry, float &exit)
    {
        Vec3f min = get_min(), max = get_max(), origin = r.origin(), direction = r.direction();
        float tmin = (min[0] - origin[0]) / direction[0];
        float tmax = (max[0] - origin[0]) / direction[0];

        if (tmin > tmax) swap(tmin, tmax);

        float tymin = (min[1] - origin[1]) / direction[1];
        float tymax = (max[1] - origin[1]) / direction[1];

        if (tymin > tymax) swap(tymin, tymax);

        if ((tmin > tymax) || (tymin > tmax))
            return false;

        if (tymin > tmin)
            tmin = tymin;

        if (tymax < tmax)
            tmax = tymax;

        float tzmin = (min[2] - origin[2]) / direction[2];
        float tzmax = (max[2] - origin[2]) / direction[2];

        if (tzmin > tzmax) swap(tzmin, tzmax);

        if ((tmin > tzmax) || (tzmin > tmax))
            return false;

        if (tzmin > tmin)
            tmin = tzmin;

        if (tzmax < tmax)
            tmax = tzmax;

        entry = tmin;
        exit  = tmax;

        return true;
    }
};

#endif //MYRAYTRACER_AABB_H
