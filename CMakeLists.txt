# CMakeList.txt: plik projektu CMake najwyższego poziomu, wykonaj konfigurację globalną
# i uwzględnij podprojekty w tym miejscu.
#
cmake_minimum_required (VERSION 3.8)

include(ExternalProject)

project ("EditFoxEngine3D")

ExternalProject_Add(GLFW SOURCE_DIR ${PROJECT_SOURCE_DIR}/glfw )
ExternalProject_Get_Property(GLFW install_dir)
ExternalProject_Get_Property(GLFW BINARY_DIR)
include_directories(${install_dir}/include)


# Uwzględnij podprojekty.
add_subdirectory ("EditFoxEngine3D")
