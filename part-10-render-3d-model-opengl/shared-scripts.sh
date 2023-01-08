#!/bin/bash

# Alias the 'pushd' command and have it send its output to the abyss ...
pushd() {
	command pushd "$@" > /dev/null
}

# Alias the 'popd' command and have it send its output to the abyss ...
popd() {
	command popd "$@" > /dev/null
}

# Given the name of a Homebrew formula, check if its installed and if not, install it.
fetch_brew_dependency() {
    FORMULA_NAME=$1

    echo "Fetching Brew dependency: '$FORMULA_NAME'."

    if brew ls --versions $FORMULA_NAME > /dev/null; then
        echo "Dependency '$FORMULA_NAME' is already installed, continuing ..."
    else
        echo "Dependency '$FORMULA_NAME' is not installed, installing via Homebrew ..."
        brew install $FORMULA_NAME
    fi
}

# If nothing has create the third-party folder yet, then we'll create it.
verify_third_party_folder_exists() {
	pushd ../../
		if [ ! -d "third-party" ]; then
			mkdir third-party
		fi
	popd
}

# If required, download the SDL library source into the third-party folder.
fetch_third_party_lib_sdl() {
	verify_third_party_folder_exists

	pushd ../../third-party
		if [ ! -d "SDL" ]; then
			echo "Fetching SDL (SDL2: 2.0.9) ..."
			wget https://www.libsdl.org/release/SDL2-2.0.9.zip
			unzip -q SDL2-2.0.9.zip
			mv SDL2-2.0.9 SDL
			rm SDL2-2.0.9.zip
		else
			echo "SDL library already exists in third party folder."
		fi
	popd
}

# If required, download the GLM library into the third-party folder.
fetch_third_party_lib_glm() {
    verify_third_party_folder_exists

    pushd ../../third-party
        if [ ! -d "glm" ]; then
            echo "Fetching GLM from: https://github.com/g-truc/glm/releases/download/0.9.9.3/glm-0.9.9.3.zip"
            wget https://github.com/g-truc/glm/releases/download/0.9.9.3/glm-0.9.9.3.zip
            unzip -q glm-0.9.9.3.zip
            rm glm-0.9.9.3.zip
        fi
    popd
}

# If required, download the Tiny OBJ library into the third-party folder.
fetch_third_party_lib_tiny_obj_loader() {
    verify_third_party_folder_exists

    pushd ../../third-party
        if [ ! -d "tiny-obj-loader" ]; then
            echo "Fetching Tiny OBJ Loader from: https://github.com/tinyobjloader/tinyobjloader/archive/refs/tags/v2.0-rc1.zip"
            wget https://github.com/tinyobjloader/tinyobjloader/archive/refs/tags/v2.0-rc1.zip
            unzip -q v2.0-rc1.zip
            rm v2.0-rc1.zip
            mv tinyobjloader-2.0-rc1 tiny-obj-loader
        fi
    popd
}

# If nothing has created the Frameworks folder yet, then we'll create it.
verify_frameworks_folder_exists() {
    # Check if the Frameworks folder exists ...
    if [ ! -d "Frameworks" ]; then
        # ... and if not, create it.
        mkdir Frameworks
    fi
}

# If required, download the SDL2 MacOS Framework into the Frameworks folder.
fetch_framework_sdl2() {
    # Make sure there is a Frameworks folder in the current directory.
    verify_frameworks_folder_exists

    # Navigate into the Frameworks folder
    pushd Frameworks
        # Check that there isn't already an SDL2.framework folder
        if [ ! -d "SDL2.framework" ]; then
            # Download the .dmg file from the SDL2 download site.
            wget https://www.libsdl.org/release/SDL2-2.0.9.dmg
			
            echo "Mounting DMG file ..."
            hdiutil attach SDL2-2.0.9.dmg
			
            echo "Copying SDL2.framework from DMG file into the current folder..."
            cp -R /Volumes/SDL2/SDL2.framework .
			
            echo "Unmounting DMG file ..."
            hdiutil detach /Volumes/SDL2

            echo "Deleting DMG file ..."
            rm SDL2-2.0.9.dmg
			
            # Navigate into the SDL2.framework folder.
            pushd SDL2.framework
                echo "Code signing SDL2.framework ..."
                codesign -f -s - SDL2
            popd
        else
            echo "SDL2.framework already exists ..."
        fi
    popd
}

verify_build_folder_exists() {
    echo "Checking for build folder ..."
    if [ ! -d "build" ]; then
        mkdir build
    fi
}