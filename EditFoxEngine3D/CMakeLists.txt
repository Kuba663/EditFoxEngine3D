# CMakeList.txt: projekt CMake dla elementu EditFoxEngine3D, dołącz źródło i zdefiniuj
# tutaj logikę specyficzną dla projektu.
#
cmake_minimum_required (VERSION 3.8)

find_package(Vulkan REQUIRED)

# Dodaj źródło do pliku wykonywalnego tego projektu.
add_executable (EditFoxEngine3D "EditFoxEngine3D.cpp" "EditFoxEngine3D.h")

# TODO: Dodaj testy i zainstaluj elementy docelowe w razie potrzeby.
target_link_libraries(EditFoxEngine3D PRIVATE Vulkan::Vulkan)