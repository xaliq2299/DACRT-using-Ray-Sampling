cmake_minimum_required(VERSION 3.9)

project(MyRayTracer LANGUAGES CXX)

find_package(OpenMP REQUIRED)

add_executable (
	MyRayTracer
	Main.cpp
	CommandLine.h
	Vec3.h
	Ray.h
	Ray.cpp
	Image.h
	Camera.h
	Mesh.h
	Scene.h
	RayTracer.h
)

set_target_properties(MyRayTracer PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(MyRayTracer PRIVATE OpenMP::OpenMP_CXX)

