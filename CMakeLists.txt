# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("Polosformer")

add_subdirectory("Polos")
add_subdirectory("Source")

install(DIRECTORY resources DESTINATION ${CMAKE_INSTALL_PREFIX})