#pragma once

#include <random>
#include <cmath>
#include <algorithm>
#include <limits>

#include "Vec3.h"
#include "Image.h"
#include "Camera.h"
#include "Scene.h"

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
    inline void render (const Scene& scene, Image& image, vector<Ray> Rays, int &counter) {
        size_t w = image.width();
        size_t h = image.height();
        const Camera& camera = scene.camera();
        std::vector<std::vector<bool>> visited; // todo: check the dimensions

#pragma omp parallel for
        for(int i = 0; i < h; i++) {
            for (int j = 0; j < w; j++) {
                if (visited[j][i])
                    continue;
                visited[j][i] = true;

                float u, v, d;
                size_t meshIndex, triangleIndex;
                Ray ray = camera.rayAt(float(j) / w, 1.f - float(i) / h);
                bool intersectionFound = rayTrace(ray, scene, meshIndex, triangleIndex, u, v, d);
                if (intersectionFound && d > 0.f) {

                    image(j, i) = shade(scene, meshIndex, triangleIndex, u, v);

                    counter++;
                }
            }
        }
        //image.savePPM ("DACRT2");
    }
};
