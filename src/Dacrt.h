// Header file of Divide-and-Conquer Ray Tracing (DACRT)
#include "Ray.h"

#define MAX_RAYS 10
#define MAX_TRIANGLES 20

class DACRT {
public:
    Image image;
    Scene scene;
    RayTracer rayTracer;
    int sentinal_nb_triangles = 0;
    Mesh mesh; // mesh data
    size_t K; // number of bins
    int i; // counter

    DACRT(Mesh& mesh, Image image, Scene scene, RayTracer rayTracer, int K){
        i = 0;
        this->mesh = mesh;
        this->K = K;
        this->image = image;
        this->rayTracer = rayTracer;
        this->scene = scene;
    }

    /*
     V - bounding volume
     R - set of active rays
     T - set of triangles
    */
    void run(AABB& V, std::vector<Ray>& rays, std::vector<Triangle>& triangles) {
        i+=1;
        int nbActiveRays = rays.size(), nbTriangles = triangles.size(), nbSampleRays;
        std::vector<Ray> sampledRays;

        // Ray sampling
        if(nbActiveRays > 1000){ // sample 100 rays if enough active rays
            sample(rays, sampledRays);
            nbSampleRays = 100;
        }
        else{
            sampledRays = rays;
            nbSampleRays = nbActiveRays;
        }

        if(nbTriangles == sentinal_nb_triangles){
            rayTracer.render(scene, image, rays);
            return;
        }
        else{
            sentinal_nb_triangles = nbTriangles;
        }

        // The recursion starts here - the following is the basic step
        bool small_enough = nbActiveRays < MAX_RAYS || nbTriangles < MAX_TRIANGLES;
        if(small_enough){
            // std::cout << "Entered Naive RT\n";
            rayTracer.render(scene, image, rays); // Naive RT
            return;
        }

        Binning binning(mesh, V, K); // apply K-binning
        binning.fill_bins(triangles); // calculate data for each bin Bj

        // initialize arrays c_left, c_right, n_left, n_right
        std::vector<int> c_left(K-1, 0), c_right(K-1, 0), n_left(K-1, 0), n_right(K-1, 0);

        for(size_t i=0;i<sampledRays.size();i++) {
            Ray r = sampledRays[i];
            Intersect(r, binning.bins, c_left, c_right, n_left, n_right);
        }

        // Partitioning using Cost function
        float C_min = std::numeric_limits<float>::infinity(); size_t j_min = 0;
        std::vector<float> alpha_left(K-1, 0), alpha_right(K-1, 0);
        for(size_t j=0;j<K-1;j++){
            alpha_left[j] = float(c_left[j])/float(nbSampleRays);
            alpha_right[j] = float(c_right[j])/float(nbSampleRays);
            // calculating cost C
            float C_T = 1.0, C_I = 1.0;
            int N_L = binning.bins[j].T_left.size(), N_R = binning.bins[j].T_right.size();
            float C = C_T + C_I*(alpha_left[j]*float(N_L) + alpha_right[j]*float(N_R));
            if(C <= C_min){
                j_min = j;
                C_min = C;
            }
        }

        // Traversal order
        AABB V0, V1;
        float alpha0, alpha1;
        std::vector<Triangle> T0, T1;
        if(n_left[j_min] >= n_right[j_min]){ // determining traversal order
            V0 = binning.bins[j_min].box_left; V1 = binning.bins[j_min].box_right;
            alpha0 = alpha_left[j_min];        alpha1 = alpha_right[j_min];
            T0 = binning.bins[j_min].T_left;   T1 = binning.bins[j_min].T_right;
        }
        else{
            V0 = binning.bins[j_min].box_right; V1 = binning.bins[j_min].box_left;
            alpha0 = alpha_right[j_min];        alpha1 = alpha_left[j_min];
            T0 = binning.bins[j_min].T_right;   T1 = binning.bins[j_min].T_left;
        }

        // Step 4 - Ray filtering
        float entry, exit;
        if(alpha0 > 0.5){ // skip ray filtering
            run(V0, rays, T0);
            i-=1;
        }
        else{ // apply ray filtering
            std::vector<Ray> R0;
            for(size_t i=0;i<rays.size();i++){
                if(V0.intersect(rays[i], entry, exit)){
                    R0.push_back(rays[i]);
                }
            }
            run(V0, R0, T0);
            i-=1;
        }
        if(alpha1 > 0.5){ // skip ray filtering
            run(V1, rays, T1);
            i-=1;
        }
        else{
            std::vector<Ray> R1;
            for(size_t i=0;i<rays.size();i++){
                if(V1.intersect(rays[i], entry, exit)){
                    R1.push_back(rays[i]);
                }
            }

            run(V1, R1, T1);
            i-=1;
        }
    }

    void Intersect(Ray& r, std::vector<Bin>& bins, std::vector<int>& c_left, std::vector<int>& c_right,
                   std::vector<int>& n_left, std::vector<int>& n_right){
        float entry, exit;
        for(size_t j=0;j<K-1;j++){
            float d_left = std::numeric_limits<float>::infinity(), d_right = std::numeric_limits<float>::infinity();
            if(bins[j].box_left.intersect(r, entry, exit)){
                c_left[j] += 1;
                d_left = entry;
            }
            if(bins[j].box_right.intersect(r, entry, exit)){
                c_right[j] += 1;
                d_right = entry;
            }
            if(d_left < d_right)
                n_left[j] += 1;
            else
                n_right[j] += 1;
        }
    }

    void sample(const std::vector<Ray>& rays, std::vector<Ray>& sampledRays){
        int size = rays.size();
        std::vector<int> all_indices(size), sampling_indices;
        std::iota(all_indices.begin(), all_indices.end(), 0);
        std::sample(all_indices.begin(), all_indices.end(), std::back_inserter(sampling_indices),
                    100, std::mt19937{std::random_device{}()});
        for(size_t i=0;i<sampling_indices.size();i++)
            sampledRays.push_back(rays[sampling_indices[i]]);
    }
};
