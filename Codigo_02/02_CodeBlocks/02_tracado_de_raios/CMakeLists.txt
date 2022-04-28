cmake_minimum_required(VERSION 3.2)

project(RAYTRACING)

set(OpenGL_GL_PREFERENCE "GLVND")

# glfw
find_package(glfw3 REQUIRED)
include_directories(${GLFW_INCLUDE_DIRS})
link_libraries(${GLFW_LIBRARY_DIRS})

# opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# glew
find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})

set(RAYTRACING_INCLUDE_DIRS ${RAYTRACING_SOURCE_DIR}/include)
set(RAYTRACING_LIBRARIES m)

include_directories(${RAYTRACING_INCLUDE_DIRS} /usr/include)

# main target
aux_source_directory(${RAYTRACING_SOURCE_DIR}/src RAYTRACING_SOURCES)

add_executable(raytracer ${RAYTRACING_SOURCES})
target_link_libraries(raytracer ${RAYTRACING_LIBRARIES})
target_link_libraries(raytracer ${OPENGL_LIBRARIES} glfw ${GLEW_LIBRARIES})
