# CMakeList.txt: projekt CMake dla elementu EditFoxEngine3D, dołącz źródło i zdefiniuj
# tutaj logikę specyficzną dla projektu.
#
cmake_minimum_required (VERSION 3.8)

find_package(Vulkan REQUIRED)

# Dodaj źródło do pliku wykonywalnego tego projektu.
add_executable (EditFoxEngine3D "EditFoxEngine3D.cpp" "EFEWindow.cxx" "EFEPipeline.cpp" "EFEDevice.cpp" "EFESwapChain.cxx" "EFEModel.hpp" "EFEModel.cxx")
add_dependencies(EditFoxEngine3D glfw Vulkan::Vulkan Eigen3::Eigen)
# TODO: Dodaj testy i zainstaluj elementy docelowe w razie potrzeby.
target_link_libraries(EditFoxEngine3D PRIVATE Vulkan::Vulkan glfw Eigen3::Eigen)

#add_custom_target(Shaders ALL DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/shaders" "${CMAKE_CURRENT_BINARY_DIR}/shaders/simple_shader.frag.spv" "${CMAKE_CURRENT_BINARY_DIR}/shaders/simple_shader.vert.spv")

#add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/shaders" COMMAND "${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/shaders" VERBATIM)

#add_custom_command(
#	OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/shaders/simple_shader.vert.spv"
#	COMMAND
#		"$ENV{VULKAN_SDK}/Bin/glslc.exe ${CMAKE_CURRENT_SOURCE_DIR}/shaders/simple_shader.vert -o ${CMAKE_CURRENT_BINARY_DIR}/shaders/simple_shader.vert.spv"
#	VERBATIM
#)

#add_custom_command(
#	OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/shaders/simple_shader.frag.spv"
#	COMMAND
#		"$ENV{VULKAN_SDK}/Bin/glslc.exe ${CMAKE_CURRENT_SOURCE_DIR}/shaders/simple_shader.frag -o ${CMAKE_CURRENT_BINARY_DIR}/shaders/simple_shader.frag.spv"
#	VERBATIM
#)