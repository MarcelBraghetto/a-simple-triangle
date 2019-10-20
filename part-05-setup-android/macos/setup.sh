#!/bin/bash

# Include the shared scripts.
. ../shared-scripts.sh

# Ask Homebrew to fetch our required programs
fetch_brew_dependency "wget"
fetch_brew_dependency "xcodegen"

fetch_third_party_lib_sdl
fetch_framework_sdl2

# Check to see if we have an existing symlink to our shared main C++ source folder.
if [ ! -d "Source" ]; then
    echo "Linking 'Source' path to '../main/src'"
    ln -s ../main/src Source
fi

# Invoke the xcodegen tool to create our project file.
echo "Generating Xcode project"
xcodegen generate