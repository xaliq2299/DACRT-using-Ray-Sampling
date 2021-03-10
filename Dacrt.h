// Header file of Divide-and-Conquer Ray Tracing (DACRT)
#include "Ray.h"
#include <limits>

/*
 V - bounding volume
 R - set of active rays
 T - set of triangles
*/
class DACRT {
public:
    Image image;
//    std::vector<Ray> Rays; // todo needed or no?
    Mesh mesh; // mesh data
    int K; // number of bins
    DACRT(Mesh& mesh, Image image, int K){
        this->mesh = mesh;
        this->K = K;
    }

    void run(AABB V, std::vector<Ray> rays, std::vector<Triangle> triangles) {
        // todo: naive RT

        Binning binning(mesh, K); // subdivide V into K bins
        binning.fill_bins(); // calculate data for each bin Bj

        // initialize arrays c_left, c_right, n_left, n_right
        std::vector<int> c_left(K-1, 0), c_right(K-1, 0), n_left(K-1, 0), n_right(K-1, 0);

        // ray sampling
        for(int i=0;i<rays.size();i++){
            Ray r = rays[i];
//            Intersect(r, );
        }
    }

    void Intersect(Ray r, AABB& V_left, AABB& V_right, std::vector<int>& c_left, std::vector<int>& c_right,
                   std::vector<int>& n_left, std::vector<int>& n_right){
        float entry, exit;
        for(size_t j=1;j<=K-1;j++){
            // todo: should d_left and d_right be arrays?
            float d_left = std::numeric_limits<float>::infinity(), d_right = std::numeric_limits<float>::infinity();
            if(V_left.intersect(r, entry, exit)){
                c_left[j] += 1;
                d_left = entry;
            }
            if(V_right.intersect(r, entry, exit)){
                c_right[j] += 1;
                d_right = entry;
            }
            if(d_left < d_right)
                n_left[j] += 1;
            else
                n_right[j] += 1;
        }
    }
};
