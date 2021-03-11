// Header file of Divide-and-Conquer Ray Tracing (DACRT)
#include "Ray.h"

class DACRT {
public:
    Image image;
    Scene scene;
    RayTracer rayTracer;
//    std::vector<Ray> Rays; // todo needed or no? passing to 'run' method directly while calling it
    Mesh mesh; // mesh data
    int K; // number of bins
    int i;

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
    void run(AABB V, std::vector<Ray> rays, std::vector<Triangle> triangles) {
        // debugging
        i+=1;
//        std::cout << "Recursive step " << i << '\n';

        int nbActiveRays = rays.size(), nbTriangles = triangles.size(), nbSampleRays;
        std::vector<Ray> sampledRays;
        // what about # triangles?

//        std::cout << "# active rays=" << nbActiveRays << " # triangles=" << nbTriangles << '\n';

        // The recursion starts here - the following is the basic step
        bool small_enough = nbActiveRays < 10 || nbTriangles < 10; // todo: which values?
        if(small_enough){
            // todo: naive RT
            std::cout << "Entered\n";
            rayTracer.render(scene, image, rays);
            return;
        }

        // Ray sampling
        if(nbActiveRays > 1000){ // sample 100 rays if enough active rays
            sample(rays, sampledRays);
            nbSampleRays = 100;
        }
        else{
            sampledRays = rays;
            nbSampleRays = nbActiveRays;
        }

        Binning binning(mesh, K); // subdivide V into K bins
        binning.fill_bins(); // calculate data for each bin Bj
//        binning.print_summary();

        // initialize arrays c_left, c_right, n_left, n_right
        std::vector<int> c_left(K-1, 0), c_right(K-1, 0), n_left(K-1, 0), n_right(K-1, 0);

        for(int i=0;i<sampledRays.size();i++) {
            Ray r = sampledRays[i];
            Intersect(r, binning.bins, c_left, c_right, n_left, n_right);
        }

        // SOLVED! TODO: check why first bin has always 100 sample rays in the right bin. Problem with index during the binning process

        // Partitioning using Cost function
        float C_min = std::numeric_limits<float>::infinity(); size_t j_min = 1;
        std::vector<float> alpha_left(K-1, 0), alpha_right(K-1, 0);
        for(int j=0;j<K-1;j++){
//            std::cout << "Bin " << j+1 << '\n';
            alpha_left[j] = float(c_left[j])/float(nbSampleRays);
            alpha_right[j] = float(c_right[j])/float(nbSampleRays);
            // calculating cost C
//            float C_T = 1.0, C_I = 1.0; // todo: constants
            int N_L = binning.bins[j].T_left.size(), N_R = binning.bins[j].T_right.size();
            // debugging
//            std::cout << "alpha_left=" << alpha_left[j] << " , alpha_right[j]=" << alpha_right[j] << '\n';
//            std::cout << "N_L = " << N_L << ", N_R = " << N_R << '\n';
            float C = alpha_left[j]*float(N_L) + alpha_right[j]*float(N_R);
//            std::cout << " C = " << C << '\n';
            if(C <= C_min){
                j_min = j;
                C_min = C;
            }
        }

        // SOLVED! TODO: why the entries for the first bin are always splitted as 0 in the left and 100 in the right
        // Printing for debugging
        /*
        std::cout << "j_min = " << j_min << ", C_min = " << C_min << '\n';
        std::cout << "C_left = {";
        for(int i=0;i<c_left.size();i++) {
            std::cout << c_left[i] << ", ";
        }
        std::cout << "}.\n";
        std::cout << "C_right = {";
        for(int i=0;i<c_right.size();i++) {
            std::cout << c_right[i] << ", ";
        }
        std::cout << "}.\n";
        std::cout << "n_left = {";
        for(int i=0;i<n_left.size();i++)
            std::cout << n_left[i] << ", ";
        std::cout << "}.\n";
        std::cout << "n_right = {";
        for(int i=0;i<n_right.size();i++)
            std::cout << n_right[i] << ", ";
        std::cout << "}.\n";
        */

        // TODO: try to avoid non-reference passing for the functions
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
        // TODO: now we are using 0.5 as a threshold, try to calculate the cost later?
        std::cout << "alpha0 = " << alpha0 << '\n';
        if(alpha0 > 0.5){ // skip ray filtering
            std::cout << ("Entered 0 left\n");
            run(V0, rays, T0);
        }
        else{ // apply ray filtering
            std::cout << ("Entered 0 right\n");
            std::vector<Ray> R0;
            sample(rays, R0);
            run(V0, R0, T0);
        }
        if(alpha1 > 0.5){ // skip ray filtering
            std::cout << ("Entered 1 left\n");
            run(V1, rays, T1);
        }
        else{
            std::cout << ("Entered 1 right\n");
            std::vector<Ray> R1;
            sample(rays, R1);
            run(V1, R1, T1);
        }
    }

    void Intersect(Ray& r, std::vector<Bin>& bins, std::vector<int>& c_left, std::vector<int>& c_right,
                   std::vector<int>& n_left, std::vector<int>& n_right){
        float entry, exit;
        for(size_t j=0;j<K-1;j++){ // todo: be precise about the indices of the bins
            // todo: should d_left and d_right be arrays?
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
        for(int i=0;i<sampling_indices.size();i++)
            sampledRays.push_back(rays[sampling_indices[i]]);
    }

    void print_summary(){ // for debugging purposes

    }
};
