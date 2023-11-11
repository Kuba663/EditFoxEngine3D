# CMakeList.txt: plik projektu CMake najwyższego poziomu, wykonaj konfigurację globalną
# i uwzględnij podprojekty w tym miejscu.
#
cmake_minimum_required (VERSION 3.8)

include(ExternalProject)

project ("EditFoxEngine3D")

add_subdirectory("glfw")
add_subdirectory("VulkanMemoryAllocator")

# Uwzględnij podprojekty.
add_subdirectory ("EditFoxEngine3D")
