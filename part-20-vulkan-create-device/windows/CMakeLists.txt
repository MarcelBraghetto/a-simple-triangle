cmake_minimum_required(VERSION 3.4.1)

project(a-simple-triangle)

set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../third-party")
set(MAIN_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../main/src")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out)

set(LIB_SDL2 ${THIRD_PARTY_DIR}/sdl-windows/lib/x64/SDL2.lib)
set(LIB_SDL2_MAIN ${THIRD_PARTY_DIR}/sdl-windows/lib/x64/SDL2main.lib)
set(LIB_GLEW ${THIRD_PARTY_DIR}/glew/lib/Release/x64/glew32s.lib)
set(LIB_SDL2_IMAGE ${THIRD_PARTY_DIR}/sdl-image-windows/lib/x64/SDL2_image.lib)
set(LIB_VULKAN ${THIRD_PARTY_DIR}/vulkan-windows/Lib/vulkan-1.lib)

include_directories(${THIRD_PARTY_DIR}/sdl-windows/include)
include_directories(${THIRD_PARTY_DIR}/glew/include)
include_directories(${THIRD_PARTY_DIR}/glm)
include_directories(${THIRD_PARTY_DIR}/tiny-obj-loader)
include_directories(${THIRD_PARTY_DIR}/sdl-image-windows/include)
include_directories(${THIRD_PARTY_DIR}/vulkan-windows/Include)

file(GLOB_RECURSE CPP_HEADERS ${MAIN_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE CPP_SOURCES ${MAIN_SOURCE_DIR}/*.cpp)

# If we are generating a release build, then request that we produce a Windows desktop
# application executable by specifying WIN32 as an executable option. This avoids the
# console window from appearing when we launch the .exe file. However for debug builds,
# the console window makes it really easy to view logging output so we'll allow it by
# NOT specifying the WIN32 executable option.
if(CMAKE_BUILD_TYPE MATCHES "Release")
	set(EXECUTABLE_TYPE WIN32)
endif()

add_executable(
    a-simple-triangle ${EXECUTABLE_TYPE}
    ${CPP_HEADERS}
    ${CPP_SOURCES}
)

set_property(TARGET a-simple-triangle PROPERTY CXX_STANDARD 17)
set_property(TARGET a-simple-triangle PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(
	a-simple-triangle
	opengl32
	${LIB_GLEW}
	${LIB_SDL2}
    ${LIB_SDL2_MAIN}
    ${LIB_SDL2_IMAGE}
	${LIB_VULKAN}
)

add_custom_command(
    TARGET a-simple-triangle
    POST_BUILD
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND PowerShell -File cmake-post-build.ps1
)
