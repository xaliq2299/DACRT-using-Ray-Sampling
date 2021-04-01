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

int main (int argc, char ** argv) {
	// Parsing the command line arguments
	CommandLine args;
	if (argc <= 1){
            // std::cerr << e.what() << std::endl;
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

	// Loading a mesh
	Mesh mesh;
	try {
        mesh.loadOFF(args.inputFilename());
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}
	scene.meshes ().push_back (mesh);

	// Rendering
	image.fillBackground ();
	std::cout << "Ray tracing: starts";
	if(withDACRT)
        with_DACRT(image, scene);
	else
        without_DACRT(image, scene);
	std::cout << "ends." << std::endl;

	return 0;
}

void without_DACRT(Image& image, Scene& scene){
    RayTracer rayTracer;
    rayTracer.render (scene, image);
    image.savePPM ("rendering.ppm");
}

void with_DACRT(Image& image, Scene& scene){
    size_t w = image.width();
    size_t h = image.height();

    auto& camera = scene.camera();
    auto& mesh = scene.meshes()[0];
    std::vector<Ray> rays = camera.fill_rays(w,h);
    int nbBins = 5;
    RayTracer rayTracer;

    DACRT dacrt(mesh, image, scene, rayTracer, nbBins);
    AABB aabb(mesh);
    auto& triangles = mesh.indexedTriangles();
    dacrt.run(aabb, rays, triangles);
    dacrt.image.savePPM("rendering_DACRT.ppm");
}
