#!/bin/bash

# Include the shared scripts and utility methods that are common to all platforms.
. ../shared-scripts.sh

fetch_brew_dependency "wget"
fetch_brew_dependency "cmake"

fetch_third_party_lib_sdl
fetch_third_party_lib_glm
fetch_third_party_lib_tiny_obj_loader

# If required, download and configure the Emscripten SDK into the third-party folder.
pushd ../../third-party
    if [ ! -d "emscripten" ]; then
        echo "Fetching Emscripten SDK ..."

        # Download the Emscripten SDK as a zip file from GitHub.
        wget https://github.com/emscripten-core/emsdk/archive/master.zip

        # Unzip the Emscripten SDK.
        unzip -q master.zip

        # Rename it to 'emscripten'.
        mv emsdk-master emscripten

        # Clean up the zip file we downloaded.
        rm master.zip

        pushd emscripten
            echo "Updating Emscripten SDK ..."
            ./emsdk update

            echo "Installing latest Emscripten SDK ..."
            ./emsdk install latest

            echo "Activating latest Emscripten SDK ..."
            ./emsdk activate latest
        popd
    fi
popd