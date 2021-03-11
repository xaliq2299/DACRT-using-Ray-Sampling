#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <exception>

#include "CommandLine.h"
#include "Image.h"
#include "Ray.h"
#include "Camera.h"
#include "Mesh.h"
#include "Scene.h"
#include "RayTracer.h"

#include "AABB.h"
#include "Binning.h"
#include "Dacrt.h"

using namespace std;
// todo: #pragma stuff
// Usage: argv[0] [-o file.ppm]
int main (int argc, char ** argv) {
	
	// Parsing the command line arguments

	CommandLine args;
	if (argc > 1) {
		try {
			args.parse(argc, argv);
		} catch (const std::exception & e) {
			std::cerr << e.what() << std::endl;
			args.printUsage (argv[0]);
			exit(1);
		}
	}

	// Initialization

	Image image(480, 270); // todo 1: try not to copy. todo 2: try to adapt to other dimension as well
//	Image image (args.width (), args.height ());
	Scene scene;
	Camera camera(Vec3f(0.f, 0.f, 1.f),
				  Vec3f(),
				  Vec3f(0.f, 1.f, 0.f),
				  60.f,
				  float(args.width()) / args.height());

	scene.camera() = camera;

	// todo: light source (needed for shading, maybe won't add)

	// Loading a mesh

	Mesh mesh;
	try {
		mesh.loadOFF("low_res_mesh.off");
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}
	scene.meshes ().push_back (mesh);

	// Rendering (prof)

	RayTracer rayTracer;
//	image.fillBackground (); todo: needed?
//	std::cout << "Ray tracing: starts";
//	rayTracer.render (scene, image);
//	std::cout << "ends." << std::endl;
//	image.savePPM (args.outputFilename ());

    // ==================================================
    // Testing new functions
    // ==================================================

    // testing binning axis
//    AABB aabb(Vec3f{0,22,3}, Vec3f{22,48,60});
//	Binning binning(aabb);
//	std::cout << binning.find_binning_axis() << '\n';

    // testing bin length
//    AABB aabb(Vec3f{0,0,3}, Vec3f{22,60,60});
//	Binning binning(aabb, 80);
//    std::cout << binning.get_bin_length() << '\n';

    // testing filling of bins (random AABB)
//    AABB aabb(Vec3f{0,0,3}, Vec3f{22,60,60});
//	Binning binning(mesh, 5);
//    binning.fill_bins();
//    binning.print_summary();

    // testing filling of bins (real AABB with a certain mesh)
//    std::vector<Triangle> m_indexedTriangles = mesh.indexedTriangles();
//    std::vector<Vec3f> m_vertexPositions = mesh.vertexPositions();

//    for(int i=0;i<m_indexedTriangles.size();i++){
//	    std::cout << "Triangle " << i+1 << ": " << m_indexedTriangles[i][0] << " "
//	                                            << m_indexedTriangles[i][1] << " "
//                                                << m_indexedTriangles[i][2] << ". ";
//	    std::cout << m_indexedTriangles[i][0] << "=" << m_vertexPositions[m_indexedTriangles[i][0]] << ", "
//                  << m_indexedTriangles[i][1] << "=" << m_vertexPositions[m_indexedTriangles[i][0]] << ", "
//                  << m_indexedTriangles[i][2] << "=" << m_vertexPositions[m_indexedTriangles[i][0]] << '\n';
//    }

    // TODO: should I construct the AABB here in Main, or the way I did is ok?

    // TODO: try not to copy the following
    size_t w = image.width();
    size_t h = image.height();
    //const Camera& camera = scene.camera();

    //auto& mesh = scene.meshes()[0];
    auto& P = mesh.vertexPositions();
    auto& T = mesh.indexedTriangles();

    // todo: ray filling (try not to copy)
    std::vector<Ray> rays;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Ray r = camera.rayAt (float (x) / w, 1.f - float (y) / h);
            rays.push_back(r);
        }
    }

    // Here starts mine
    int nbBins = 5;
    DACRT dacrt(mesh, image, scene, rayTracer, nbBins);
	AABB volume(mesh);
	auto& triangles = mesh.indexedTriangles();
	dacrt.run(volume, rays, triangles);

	return 0;
}