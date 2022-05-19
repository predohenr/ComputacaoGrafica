cmake_minimum_required (VERSION 3.18)
project (OPENGL_VIEWER)

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

add_executable (glview ${OPENGL_VIEWER_SOURCE_DIR}/Sources/main.cpp) 
target_link_libraries(glview ${OPENGL_LIBRARIES} glfw ${GLEW_LIBRARIES})
