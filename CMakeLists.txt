cmake_minimum_required(VERSION 3.17)
project(OpenGL_Test_HelloTriangle)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(OpenGL_Test_HelloTriangle main.cpp additional/glad.c Shader.h Cumera.h Mesh.h Model.h)

find_package(glfw3 REQUIRED)
target_link_libraries(OpenGL_Test_HelloTriangle GL glfw assimp)
