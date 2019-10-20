cmake_minimum_required(VERSION 3.4.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -fexceptions")

set(THIRD_PARTY_DIR "../../third-party")
set(MAIN_SOURCE_DIR "../main/src")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out)

include_directories(${THIRD_PARTY_DIR}/SDL/include)
include_directories(${THIRD_PARTY_DIR}/glm)
include_directories(${THIRD_PARTY_DIR}/tiny-obj-loader)
include_directories(${THIRD_PARTY_DIR}/SDL2_image)
include_directories(${THIRD_PARTY_DIR}/vulkan-mac/macOS/include)

file(GLOB_RECURSE CPP_HEADERS ${MAIN_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE CPP_SOURCES ${MAIN_SOURCE_DIR}/*.cpp)

set(DYLIB_VULKAN ${CMAKE_CURRENT_SOURCE_DIR}/Frameworks/libvulkan.1.dylib)
set(DYLIB_MOLTEN_VK ${CMAKE_CURRENT_SOURCE_DIR}/Frameworks/libMoltenVK.dylib)

add_executable(
    a-simple-triangle-console
    ${CPP_HEADERS}
    ${CPP_SOURCES}
)

set_target_properties(
    a-simple-triangle-console
    PROPERTIES
    LINK_FLAGS
    "-F../Frameworks -framework SDL2 -framework SDL2_image -framework OpenGL"
)

target_link_libraries(
	a-simple-triangle-console
    ${DYLIB_VULKAN}
    ${DYLIB_MOLTEN_VK}
)

add_custom_command(
    TARGET a-simple-triangle-console
    POST_BUILD
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    COMMAND ./cmake-post-build.sh
)
