cmake_minimum_required(VERSION 3.4.1)

# We are using C++ 17 and will make use of C++ exceptions.
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fexceptions")

set(THIRD_PARTY_DIR "../../third-party")
set(MAIN_SOURCE_DIR "../main/src")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out)

include_directories(${THIRD_PARTY_DIR}/SDL/include)
include_directories(${THIRD_PARTY_DIR}/glm)
include_directories(${THIRD_PARTY_DIR}/tiny-obj-loader)
include_directories(${THIRD_PARTY_DIR}/SDL2_image)

file(GLOB_RECURSE CPP_HEADERS ${MAIN_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE CPP_SOURCES ${MAIN_SOURCE_DIR}/*.cpp)

# The 'a-simple-triangle' executable is the output target for our Emscripten application and is why we end up with 'a-simple-triangle.js' in the output folder. Note that it will use the C++ source code from the shared 'main' folder through the collated sources.
add_executable(
    a-simple-triangle
    ${CPP_HEADERS}
    ${CPP_SOURCES}
)

# These properties specify what kind of Emscripten build to perform and are assigned to our 'a-simple-triangle' executable target.
set_target_properties(
    a-simple-triangle
    PROPERTIES
    LINK_FLAGS
    "-O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=\"['png']\" -s USE_WEBGL2=1 --preload-file ../../main/assets@/assets"
)

# When the build is complete we will run the 'cmake-post-build.sh' script.
add_custom_command(
    TARGET a-simple-triangle
    POST_BUILD
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND ./cmake-post-build.sh
)
