//
// Created by khalig on 08.03.21.
//

#ifndef MYRAYTRACER_BINNING_H
#define MYRAYTRACER_BINNING_H

#include "AABB.h"

struct Bin{
    std::vector<Triangle> T_left, T_right; // triangles in the left and right bounding volumes
    AABB box_left, box_right; // left and right AABBs for a given bin
};


class Binning{
public:
    Mesh mesh;
    AABB aabb; // Axis-aligned bounding box used during the binning
    size_t K;  // number of bins
    float bin_length; // length of a bin
    std::vector<Bin> bins; // all possible bins

    Binning(Mesh& mesh, size_t K){
        this->mesh = mesh;
        this->K = K;
        this->aabb = AABB(mesh);
        this->bin_length = get_bin_length();

        for(size_t i=0;i<K-1;i++){
            Bin bin;
            bins.push_back(bin);
        }
    }

    Binning(Mesh& mesh, AABB aabb, size_t K){
        this->mesh = mesh;
        this->K = K;
        this->aabb = aabb;
        this->bin_length = get_bin_length();

        for(size_t i=0;i<K-1;i++){
            Bin bin;
            bins.push_back(bin);
        }
    }

    /*
    AABB construct_AABB(){
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

//        std::cout << "Constructed AABB\n";
//        std::cout << min_x << " " << min_y << " " << min_z << '\n';
//        std::cout << max_x << " " << max_y << " " << max_z << '\n';
        AABB constructed_aabb({min_x, min_y, min_z}, {max_x, max_y, max_z});
        return constructed_aabb;
    }
     */

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

    void fill_bins(std::vector<Triangle>& triangles){
//        std::vector<Triangle> indexedTriangles = mesh.indexedTriangles();
        std::vector<Vec3f> vertexPositions = mesh.vertexPositions();

        size_t axis_ind = find_binning_axis();
        float start = aabb.get_min()[axis_ind];
        for(size_t k=0;k<K-1;k++){
//            std::cout << "Bin " << k << ": ";
            float min_x_left = vertexPositions[triangles[0][0]][0],
            min_y_left = vertexPositions[triangles[0][0]][1],
            min_z_left = vertexPositions[triangles[0][0]][2];
            float min_x_right = vertexPositions[triangles[0][0]][0],
            min_y_right = vertexPositions[triangles[0][0]][1],
            min_z_right = vertexPositions[triangles[0][0]][2];
            float max_x_left = vertexPositions[triangles[0][0]][0],
            max_y_left = vertexPositions[triangles[0][0]][1],
            max_z_left = vertexPositions[triangles[0][0]][2];
            float max_x_right = vertexPositions[triangles[0][0]][0],
            max_y_right = vertexPositions[triangles[0][0]][1],
            max_z_right = vertexPositions[triangles[0][0]][2];

            // std::cout << "Bin " << k << ": position " << position+k*bin_length << '\n';
            float position = start+(k+1)*bin_length;
            for(size_t tr_ind=0;tr_ind<triangles.size();tr_ind++){
//                std::cout << "Triangle " << tr_ind+1 << ", ";
                float a = vertexPositions[triangles[tr_ind][0]][axis_ind],
                      b = vertexPositions[triangles[tr_ind][1]][axis_ind],
                      c = vertexPositions[triangles[tr_ind][2]][axis_ind];
                float barycenter = (a+b+c)/3;
                // todo: what about the equal case?
                float x1 = vertexPositions[triangles[tr_ind][0]][0];
                float x2 = vertexPositions[triangles[tr_ind][1]][0];
                float x3 = vertexPositions[triangles[tr_ind][2]][0];
                float y1 = vertexPositions[triangles[tr_ind][0]][1];
                float y2 = vertexPositions[triangles[tr_ind][1]][1];
                float y3 = vertexPositions[triangles[tr_ind][2]][1];
                float z1 = vertexPositions[triangles[tr_ind][0]][2];
                float z2 = vertexPositions[triangles[tr_ind][1]][2];
                float z3 = vertexPositions[triangles[tr_ind][2]][2];
//                std::cout << vertexPositions[indexedTriangles[tr_ind][0]] << " "
//                          << vertexPositions[indexedTriangles[tr_ind][1]] << " "
//                          << vertexPositions[indexedTriangles[tr_ind][2]] << '\n';

                if(barycenter > position) {
//                    std::cout << "goes to right\n";
//                    std::cout << "Inside barycenter check right\n";
                    bins[k].T_right.push_back(triangles[tr_ind]);
                    float cur_max_x_right = std::max(std::max(x1,x2), x3);
                    float cur_min_x_right = std::min(std::min(x1,x2), x3);
                    float cur_max_y_right = std::max(std::max(y1,y2), y3);
                    float cur_min_y_right = std::min(std::min(y1,y2), y3);
                    float cur_max_z_right = std::max(std::max(z1,z2), z3);
                    float cur_min_z_right = std::min(std::min(z1,z2), z3);
                    if(cur_max_x_right > max_x_right)
                        max_x_right = cur_max_x_right;
                    if(cur_max_y_right > max_y_right)
                        max_y_right = cur_max_y_right;
                    if(cur_max_z_right > max_z_right)
                        max_z_right = cur_max_z_right;

                    if(cur_min_x_right < min_x_right)
                        min_x_right = cur_min_x_right;
                    if(cur_min_y_right < min_y_right)
                        min_y_right = cur_min_y_right;
                    if(cur_min_z_right < min_z_right)
                        min_z_right = cur_min_z_right;
                    // std::cout << "Inside barycenter check right\n";
                }
                else{
//                    std::cout << "goes to left\n";
//                    std::cout << "Inside barycenter check left\n";
                    bins[k].T_left.push_back(triangles[tr_ind]);
                    float cur_min_x_left = std::min(std::min(x1,x2), x3);
                    float cur_max_x_left = std::max(std::max(x1,x2), x3);
                    float cur_min_y_left = std::min(std::min(y1,y2), y3);
                    float cur_max_y_left = std::max(std::max(y1,y2), y3);
                    float cur_min_z_left = std::min(std::min(z1,z2), z3);
                    float cur_max_z_left = std::max(std::max(z1,z2), z3);
                    if(cur_min_x_left < min_x_left) {
                        min_x_left = cur_min_x_left;
                    }
                    if(cur_min_y_left < min_y_left)
                        min_y_left = cur_min_y_left;
                    if(cur_min_z_left < min_z_left)
                        min_z_left = cur_min_z_left;

                    if(cur_max_x_left > max_x_left) {
                        max_x_left = cur_max_x_left;
                    }
                    if(cur_max_y_left > max_y_left)
                        max_y_left = cur_max_y_left;
                    if(cur_max_z_left > max_z_left)
                        max_z_left = cur_max_z_left;
                    // std::cout << "Inside barycenter check left\n";
                }
            }
            // todo: fill also the left and right aabbs (V_left and V_right volumes) for the bin SOLVED!
            bins[k].box_left  = AABB({min_x_left, min_y_left, min_z_left}, {max_x_left, max_y_left, max_z_left});
            bins[k].box_right = AABB({min_x_right, min_y_right, min_z_right}, {max_x_right, max_y_right, max_z_right});
//            std::cout << "Left box min: " << min_x_left << " " << min_y_left << " " << min_z_left << '\n';
//            std::cout << "Left box max: " << max_x_left << " " << max_y_left << " " << max_z_left << '\n';
            // todo: fix problem in the following. SOLVED!
//            std::cout << "Right box min: " << min_x_right << " " << min_y_right << " " << min_z_right << '\n';
//            std::cout << "Right box max: " << max_x_right << " " << max_y_right << " " << max_z_right << '\n';
        }
    }

    void print_summary(){ // for debugging purposes
        for(size_t k=0;k<bins.size();k++){ // Binning should start from k=1
            std::cout << "\nBin " << k+1 << '\n';

            std::cout << "Left AABB: ";
            bins[k].box_left.print();
            std::cout << "Left AABB Triangles: {";
            for(size_t tr_ind = 0;tr_ind<bins[k].T_left.size();tr_ind++){
                std::cout << bins[k].T_left[tr_ind] << ", ";
            }
            std::cout << "}.\n";


            std::cout << "Right AABB: ";
            bins[k].box_right.print();
            std::cout << "Right AABB Triangles: {";
            for(size_t tr_ind = 0;tr_ind<bins[k].T_right.size();tr_ind++){
                std::cout << bins[k].T_right[tr_ind] << ", ";
            }
            std::cout << "}.\n";
        }
    }
};


#endif //MYRAYTRACER_BINNING_H
