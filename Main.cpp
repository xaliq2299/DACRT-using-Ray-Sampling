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

//	Image image (args.width (), args.height ());
//	Scene scene;
//	Camera camera(Vec3f(0.f, 0.f, 1.f),
//				  Vec3f(),
//				  Vec3f(0.f, 1.f, 0.f),
//				  60.f,
//				  float(args.width()) / args.height());
//
//	scene.camera() = camera;
//
	// Loading a mesh

	Mesh mesh;
	try {
		mesh.loadOFF("low_res_mesh.off");
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}
//	scene.meshes ().push_back (mesh);
//
//	// Rendering
//
//	RayTracer rayTracer;
//	image.fillBackground ();
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

    // testing filling of bins (just printing)
    AABB aabb(Vec3f{0,0,3}, Vec3f{22,60,60});
	Binning binning(mesh, 5);
    binning.fill_bins();
    binning.print_summary();

    // testing filling of bins (real filling with meshes)
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


	return 0;
}