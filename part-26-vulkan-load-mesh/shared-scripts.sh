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

# If required, download the SDL2 image library source into the third-party folder.
fetch_third_party_lib_sdl_image() {
    verify_third_party_folder_exists

    pushd ../../third-party
        if [ ! -d "SDL2_image" ]; then
            echo "Fetching SDL2_image source library (2.0.4) ..."
            wget https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.4.zip
            unzip -q SDL2_image-2.0.4.zip
            mv SDL2_image-2.0.4 SDL2_image
            rm SDL2_image-2.0.4.zip
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
            echo "Fetching Tiny OBJ Loader from: https://github.com/syoyo/tinyobjloader/archive/v1.4.1.zip"
            wget https://github.com/syoyo/tinyobjloader/archive/v1.4.1.zip
            unzip -q v1.4.1.zip
            rm v1.4.1.zip
            mv tinyobjloader-1.4.1 tiny-obj-loader
        fi
    popd
}

fetch_third_party_lib_vulkan_macos() {
    verify_third_party_folder_exists

    pushd ../../third-party
        if [ ! -d "vulkan-mac" ]; then
            echo "Fetching Vulkan SDK (Mac) from: https://sdk.lunarg.com/sdk/download/1.1.92.1/mac/vulkansdk-macos-1.1.92.1.tar.gz?Human=true"
            wget --no-cookies https://sdk.lunarg.com/sdk/download/1.1.92.1/mac/vulkansdk-macos-1.1.92.1.tar.gz?Human=true -O vulkan-mac.tar.gz
            echo "Unzipping Vulkan SDK (Mac) into 'third-party/vulkan-mac' ..."
            tar -xf vulkan-mac.tar.gz
            rm vulkan-mac.tar.gz
            mv vulkansdk-macos-1.1.92.1 vulkan-mac
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

# If required, download the SDL2 Image MacOS Framework into the Frameworks folder.
fetch_framework_sdl2_image() {
    verify_frameworks_folder_exists

    pushd Frameworks
        if [ ! -d "SDL2_image.framework" ]; then
            echo "Fetching SDL2_image framework from: https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.4.dmg"
            wget https://www.libsdl.org/projects/SDL_image/release/SDL2_image-2.0.4.dmg
            
            echo "Attaching DMG file ..."
            hdiutil attach SDL2_image-2.0.4.dmg
            
            echo "Copying SDL2_image.framework from DMG file ..."
            cp -R /Volumes/SDL2_image/SDL2_image.framework .
            
            echo "Detaching DMG file ..."
            hdiutil detach /Volumes/SDL2_image
            rm SDL2_image-2.0.4.dmg
            
            # We need to code sign a couple of binaries to avoid Xcode errors
            pushd SDL2_image.framework/Versions/A/Frameworks/webp.framework
                echo "Code signing SDL2_image.framework / Frameworks / webp.framework ..."
                codesign -f -s - webp
            popd

            pushd SDL2_image.framework
                echo "Code signing SDL2_image.framework ..."
                codesign -f -s - SDL2_image
            popd
        fi
    popd
}

verify_build_folder_exists() {
    echo "Checking for build folder ..."
    if [ ! -d "build" ]; then
        mkdir build
    fi
}

setup_vulkan_libs_macos() {
	verify_frameworks_folder_exists

	pushd "Frameworks"
		if [ ! -e "libvulkan.1.dylib" ]; then
			cp ../../../third-party/vulkan-mac/macOS/lib/libvulkan.1.1.92.dylib libvulkan.1.dylib
		fi

		if [ ! -e "libMoltenVK.dylib" ]; then
			cp ../../../third-party/vulkan-mac/macOS/lib/libMoltenVK.dylib libMoltenVK.dylib
		fi
	popd
}

compile_vulkan_shaders() {
    pushd ../main/vulkan_shader_source
        ./compile_shaders.sh
    popd
}
