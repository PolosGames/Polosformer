cmake_minimum_required(VERSION 3.24)

# Add source to this project's executable.
add_executable(Polosformer)

file(GLOB_RECURSE Polosformer_SOURCES "${CMAKE_CURRENT_LIST_DIR}/*.cpp")
file(GLOB_RECURSE Polosformer_HEADERS "${CMAKE_CURRENT_LIST_DIR}/*.h")

target_sources(
    Polosformer
        PRIVATE
            ${Polosformer_SOURCES}
            ${Polosformer_HEADERS}
)

target_link_libraries(
    Polosformer
        PRIVATE
            Runtime
            PolosThirdParty
            glm::glm_interface
            glad::glad_interface
            glfw::glfw_interface
)

target_compile_features(
    Polosformer
        PRIVATE
            cxx_std_20
)

target_precompile_headers(
    Polosformer
        REUSE_FROM
            RuntimePCH
)

install(TARGETS Polosformer DESTINATION ${CMAKE_INSTALL_PREFIX})

build_options(Polosformer true)
