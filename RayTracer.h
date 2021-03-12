#pragma once

#include <random>
#include <cmath>
#include <algorithm>
#include <limits>

#include "Vec3.h"
#include "Image.h"
#include "Camera.h"
#include "Scene.h"
#define ROW 480
#define COLUMN 270

using namespace std;

class RayTracer {
public:
	RayTracer () {}
	virtual ~RayTracer() {}
	
	// Returns true if an intersection found, filling the barycentric coordinate {u,v} and the distance d in this case.   
	inline bool rayTrace (const Ray & ray, 
						  const Scene & scene, 
						  size_t & meshIndex, 
						  size_t & triangleIndex, 
						  float & u, 
						  float & v, 
						  float & d) {
		const auto& meshes = scene.meshes();
		float closest = std::numeric_limits<float>::max();
		bool intersectionFound = false;
		for (size_t mIndex = 0; mIndex < meshes.size(); mIndex++) {
			const auto& P = meshes[mIndex].vertexPositions();
			const auto& T = meshes[mIndex].indexedTriangles();
			for (size_t tIndex = 0; tIndex < T.size(); tIndex++) {
				const Vec3i& triangle = T[tIndex];
				float ut, vt, dt;
				if (ray.triangleIntersect(P[triangle[0]], P[triangle[1]], P[triangle[2]], ut, vt, dt) == true) {
					if (dt > 0.f && dt < closest) {
						intersectionFound = true;
						closest = dt;
						meshIndex = mIndex;
						triangleIndex = tIndex;
						u = ut;
						v = vt;
						d = dt;
					}
				}
			}
		}
		return intersectionFound;
	}

	// Returns the color response associated to point defined as a barycentric interpolation over a triangle of a mesh in the scene.
	inline Vec3f shade (const Scene & scene, size_t meshIndex, size_t triangleIndex, float u, float v) {
		const auto& mesh = scene.meshes()[meshIndex];
		const auto& P = mesh.vertexPositions();
		const auto& N = mesh.vertexNormals();
		const Vec3i& triangle = mesh.indexedTriangles()[triangleIndex];
		Vec3f hitNormal = normalize((1.f - u - v) * N[triangle[0]] + u * N[triangle[1]] + v * N[triangle[2]]);

		// No shading so far, only normal visualization
		return Vec3f(0.5f, 0.5f, 0.5f) + hitNormal / 2.f;
	}

	// Fills the image pixels with color response which are ray traced in the scene
	inline void render (const Scene& scene, Image& image) {
		size_t w = image.width();
		size_t h = image.height();
		const Camera& camera = scene.camera();
		for (int y = 0; y < h; y++) {
			static int progress = 0;
			progress++;
			if (progress % 10 == 0)
				std::cout << ".";
#pragma omp parallel for
			for (int x = 0; x < w; x++) {
				Vec3f colorResponse;
				Ray ray = camera.rayAt (float (x) / w, 1.f - float (y) / h);
				size_t meshIndex, triangleIndex;
				float u, v, d;
				bool intersectionFound = rayTrace (ray, scene, meshIndex, triangleIndex, u, v, d);
				if (intersectionFound && d > 0.f)
					image (x,y) = shade (scene, meshIndex, triangleIndex, u, v);
			}
		}
	}


	// render function remastered for the project todo: try not to copy
    inline void render (const Scene& scene, Image& image, vector<Ray>& Rays) {
        size_t w = image.width();
        size_t h = image.height();
        const Camera& camera = scene.camera();
        std::vector<std::vector<bool>> visited; // todo: check the dimensions
        std::cout << "Before filling\n";
        for(int i=0; i<w; i++){
            visited.push_back(std::vector<bool>());
            for(int j=0; j<h; j++){
                visited[i].push_back(false);
            }
        }
        std::cout << "After filling\n";
//        std::cout << "Width=" << w << ", Height=" << h << '\n';
//        std::cout << "Before loops\n";

#pragma omp parallel for
        // Todo: do we need Ray.x and Ray.y?
        for(int i = 0; i<Rays.size(); i++) {
            if(visited[Rays.at(i).x][Rays.at(i).y])
                continue;
            visited[Rays.at(i).x][Rays.at(i).y] = true;
            float u, v, d;
            size_t meshIndex, triangleIndex;
            Ray ray = camera.rayAt (float (Rays.at(i).x) / w, 1.f - float (Rays.at(i).y) / h);
            bool intersectionFound = rayTrace (ray, scene, meshIndex, triangleIndex, u, v, d);
            if (intersectionFound && d > 0.f){

                image (Rays.at(i).x, Rays.at(i).y) = shade (scene, meshIndex, triangleIndex, u, v);

            }
        }
//        image.savePPM ("DACRT2");
//        exit(0);

        /*
        for(int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
//                std::cout << "i=" << i << ", j=" <<  j << ".\n";
//                std::cout << "Here\n";
                if (visited[j][i]) {
//                    std::cout << "entered visited\n";
                    continue;
                }
//                std::cout << "Here\n";
                visited[j][i] = true;

                float u, v, d;
                size_t meshIndex, triangleIndex;
//                std::cout << "Here\n";
                Ray ray = camera.rayAt(float(j) / w, 1.f - float(i) / h);
//                std::cout << "Here\n";
                bool intersectionFound = rayTrace(ray, scene, meshIndex, triangleIndex, u, v, d);
                if (intersectionFound && d > 0.f) {

                    image(j, i) = shade(scene, meshIndex, triangleIndex, u, v);

                // todo: not using counter
                }
            }
        }
        */
    }


    // todo: try not to copy the following
    inline void myRender (const Scene& scene, Image& image, vector<Ray> Rays, int &counter, bool (&visited)[ROW][COLUMN]) {
        size_t w = image.width();
        size_t h = image.height();
        const Camera& camera = scene.camera();

#pragma omp parallel for
        for(int i = 0; i<Rays.size(); i++) {
            if(visited[Rays.at(i).x][Rays.at(i).y])
                continue;
            visited[Rays.at(i).x][Rays.at(i).y] = true;
            float u, v, d;
            size_t meshIndex, triangleIndex;
            Ray ray = camera.rayAt (float (Rays.at(i).x) / w, 1.f - float (Rays.at(i).y) / h);
            bool intersectionFound = rayTrace (ray, scene, meshIndex, triangleIndex, u, v, d);
            if (intersectionFound && d > 0.f){

                image (Rays.at(i).x, Rays.at(i).y) = shade (scene, meshIndex, triangleIndex, u, v);

                counter++;

            }
        }
        //image.savePPM ("DACRT2");
    }
};
