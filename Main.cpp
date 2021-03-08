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

using namespace std;

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

	Image image (args.width (), args.height ());
	Scene scene;
	Camera camera(Vec3f(0.f, 0.f, 1.f),
				  Vec3f(),
				  Vec3f(0.f, 1.f, 0.f),
				  60.f,
				  float(args.width()) / args.height());

	scene.camera() = camera;
	
	// Loading a mesh

	Mesh mesh;
	try {
		mesh.loadOFF("example.off");
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		exit(1);
	}
	scene.meshes ().push_back (mesh);

	// Rendering
	
	RayTracer rayTracer;
	image.fillBackground ();
	std::cout << "Ray tracing: starts";
	rayTracer.render (scene, image);
	std::cout << "ends." << std::endl;
	image.savePPM (args.outputFilename ());
	

	return 0;
}