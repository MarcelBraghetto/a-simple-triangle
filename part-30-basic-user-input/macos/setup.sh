#!/bin/bash

# Include the shared scripts.
. ../shared-scripts.sh

# Ask Homebrew to fetch our required programs
fetch_brew_dependency "wget"
fetch_brew_dependency "xcodegen"

fetch_third_party_lib_sdl
fetch_third_party_lib_glm
fetch_third_party_lib_tiny_obj_loader
fetch_framework_sdl2
fetch_third_party_lib_sdl_image
fetch_framework_sdl2_image
fetch_third_party_lib_vulkan_macos
setup_vulkan_libs_macos

# Check to see if we have an existing symlink to our shared main C++ source folder.
if [ ! -d "Source" ]; then
    echo "Linking 'Source' path to '../main/src'"
    ln -s ../main/src Source
fi

# Check to see if we have an existing symlink to our shared main assets folder.
if [ ! -d "assets" ]; then
    echo "Linking 'assets' path to '../main/assets'"
    ln -s ../main/assets assets
fi

# Invoke the xcodegen tool to create our project file.
echo "Generating Xcode project"
xcodegen generate

compile_vulkan_shaders