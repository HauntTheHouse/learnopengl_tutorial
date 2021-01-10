cmake_minimum_required(VERSION 3.17)
project(learnopengl_tutorial)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(learnopengl_tutorial main.cpp Shader.h Cumera.h Mesh.h Model.h)

find_package(glfw3 REQUIRED)
target_link_libraries(learnopengl_tutorial GL glfw GLEW assimp)
