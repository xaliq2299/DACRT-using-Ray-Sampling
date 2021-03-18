#include <iostream>
#include <cstdlib>
#include <vector>
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

void without_DACRT(Image& image, Scene& scene); // old rendering
void with_DACRT(Image& image, Scene& scene); // new rendering with DACRT

// todo: #pragma stuff
// Usage: argv[0] [-o file.ppm]
int main (int argc, char ** argv) {

	// Parsing the command line arguments
	CommandLine args;
	if (argc <= 1){
//            std::cerr << e.what() << std::endl;
            args.printUsage (argv[0]);
            exit(1);
	}
	else if (argc > 1) {
		try {
			args.parse(argc, argv);
		} catch (const std::exception & e) {
			std::cerr << e.what() << std::endl;
			args.printUsage (argv[0]);
			exit(1);
		}
		if(args.dacrt() == 2){
            args.printUsage (argv[0]);
            exit(1);
		}
	}
	bool withDACRT = args.dacrt();
	std::cout << withDACRT << " hello\n";

	// Initialization
    Image image (args.width (), args.height ());
	Scene scene;
	Camera camera(Vec3f(0.f, 0.f, 1.f),
				  Vec3f(),
				  Vec3f(0.f, 1.f, 0.f),
				  60.f,
				  float(args.width()) / args.height());\
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

	// Rendering
	image.fillBackground ();
//	std::cout << "Ray tracing: starts";
	if(withDACRT){
	    std::cout << "With\n";
        with_DACRT(image, scene);
    }
	else{
	    std::cout << "Without\n";
        without_DACRT(image, scene);
    }
//	std::cout << "ends." << std::endl;

	return 0;
}

void without_DACRT(Image& image, Scene& scene){ // rendering of the professor
    RayTracer rayTracer;
    rayTracer.render (scene, image);
    image.savePPM ("rendering.ppm");
}

void with_DACRT(Image& image, Scene& scene){
    // TODO: should I construct the AABB here in Main, or the way I did is ok?
    // TODO: try not to copy the following code
    size_t w = image.width();
    size_t h = image.height();

    auto& camera = scene.camera();
    auto& mesh = scene.meshes()[0];
    auto& P = mesh.vertexPositions();
    auto& T = mesh.indexedTriangles();

    // todo: ray filling (try not to copy)
    std::vector<Ray> rays;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Ray r = camera.rayAt (float (x) / w, 1.f - float (y) / h);
            r.x = x;
            r.y = y;
            rays.push_back(r);
        }
    }

    // Here starts mine
    int nbBins = 5;
    RayTracer rayTracer;
    DACRT dacrt(mesh, image, scene, rayTracer, nbBins);
    AABB volume(mesh);
    auto& triangles = mesh.indexedTriangles();
    dacrt.run(volume, rays, triangles);
    dacrt.image.savePPM("rendering_DACRT.ppm");
}
