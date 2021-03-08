//
// Created by khalig on 08.03.21.
//

#ifndef MYRAYTRACER_BINNING_H
#define MYRAYTRACER_BINNING_H

#include "AABB.h"

struct Bin{
    std::vector<size_t> T_left, T_right; // triangles in the left and right bounding volumes
    AABB box_left, box_right; // left and right AABBs for a given bin
};


class Binning{
public:
    AABB aabb; // Axis-aligned bounding box used during the binning
    size_t K;  // number of bins
    float bin_length; // length of a bin

    Binning(AABB aabb, size_t K){
        this->aabb = aabb;
        this->K = K;
        this->bin_length = get_bin_length();
    }

    size_t find_binning_axis(){ // todo: binning is done based on the longest axis
        Vec3f min = aabb.get_min(), max = aabb.get_max();
        std::vector<int> diff;
        for(int i=0;i<3;i++) {
            diff.push_back(abs(min[i] - max[i]));
        }
        size_t max_in = 0;
        for(int i=1;i<3;i++){
            if(diff[i] > diff[max_in])
                max_in = i;
        }

        return max_in;
    }

    float get_bin_length(){
        auto axis_ind = find_binning_axis();
        return abs(aabb.get_min()[axis_ind] - aabb.get_max()[axis_ind])/K;
    }

    void fill_bins(Mesh& mesh){
        std::vector<Bin> bins;
        std::vector<Triangle> indexedTriangles = mesh.indexedTriangles();
        std::vector<Vec3f> vertexPositions = mesh.vertexPositions();

        size_t axis_ind = find_binning_axis();
        float start = aabb.get_min()[axis_ind];
        for(size_t k=1;k<K;k++){
            // std::cout << "Bin " << k << ": position " << position+k*bin_length << '\n';
            float position = start+k*bin_length;
            for(size_t tr_ind=0;tr_ind<indexedTriangles.size();tr_ind){
                float a = vertexPositions[indexedTriangles[tr_ind][0]][axis_ind],
                      b = vertexPositions[indexedTriangles[tr_ind][1]][axis_ind],
                      c = vertexPositions[indexedTriangles[tr_ind][2]][axis_ind];
                float barycenter = (a+b+c)/3;
                // todo: what about the equal case?
                if(barycenter > position) {
                    bins[k].T_right.push_back(tr_ind);
                }
                else{
                    bins[k].T_left.push_back(tr_ind);
                }
                // todo: fill also the left and right aabbs (V_left and V_right volumes) for the bin
                // FILL HERE
                // ...
            }
        }
    }

};

#endif //MYRAYTRACER_BINNING_H
