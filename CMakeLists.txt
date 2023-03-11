# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("Polosformer")

if(${CMAKE_BUILD_TYPE} MATCHES "Release" OR ${CMAKE_BUILD_TYPE} MATCHES "MinSizeRel")
    add_compile_definitions(PL_RELEASE)
else()
    add_compile_definitions(PL_DEBUG)
endif()


add_subdirectory("Polos")
add_subdirectory("Source")

install(DIRECTORY resources DESTINATION ${CMAKE_INSTALL_PREFIX})