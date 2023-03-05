cmake_minimum_required(VERSION 3.24)

# Add source to this project's executable.
add_executable(Polosformer)

set(
    Polosformer_SOURCES
        "entry_point.cpp"
 "game.cpp")

set(
    Polosformer_HEADERS
        "entry_point.h"
 "game.h")

target_sources(
    Polosformer
        PRIVATE
            ${Polosformer_SOURCES}
            ${Polosformer_HEADERS}
)

target_include_directories(
    Polosformer
        PRIVATE
            "Source"
)

target_link_libraries(
    Polosformer
        PUBLIC
            Runtime
)

target_compile_features(
    Polosformer
        PRIVATE
            cxx_std_20
)

target_precompile_headers(
    Polosformer
        PRIVATE
            "pch.h"
)

install(TARGETS Polosformer DESTINATION ${CMAKE_INSTALL_PREFIX})