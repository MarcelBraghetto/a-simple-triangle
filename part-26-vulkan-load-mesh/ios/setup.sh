#!/bin/bash

# Include the shared scripts and utility methods that are common to all platforms.
. ../shared-scripts.sh

fetch_brew_dependency "wget"
fetch_brew_dependency "xcodegen"

fetch_third_party_lib_sdl
fetch_third_party_lib_glm
fetch_third_party_lib_tiny_obj_loader
fetch_third_party_lib_vulkan_macos

# This method will compile a static library from an Xcode project if it doesn't already exist in the Libs folder.
create_static_library() {
    # The following arguments need to be passed into this method:  
    STATIC_LIBRARY=$1
    PROJECT_PATH=$2
    XCODE_PROJECT=$3
    XCODE_TARGET=$4
    BUILD_FOLDER=$5

    # If there is no 'Libs' folder yet, create it.
    if [ ! -d "Libs" ]; then
        mkdir Libs
    fi

    # Navigate into the 'Libs' folder.
    pushd Libs
        # If the static library file doesn't exist, we'll make it.
        if [ ! -e $STATIC_LIBRARY ]; then
          
          # Navigate to the path containing the Xcode project.
          pushd $PROJECT_PATH
              # Build the iPhone library.
              echo "Building the iOS iPhone static library ..."
              
              xcrun xcodebuild -configuration "Release" \
                  -project $XCODE_PROJECT \
                  -target $XCODE_TARGET \
                  -sdk "iphoneos" \
                  build \
                  ONLY_ACTIVE_ARCH=NO \
                  RUN_CLANG_STATIC_ANALYZER=NO \
                  BUILD_DIR="build/$BUILD_FOLDER" \
                  SYMROOT="build/$BUILD_FOLDER" \
                  OBJROOT="build/$BUILD_FOLDER/obj" \
                  BUILD_ROOT="build/$BUILD_FOLDER" \
                  TARGET_BUILD_DIR="build/$BUILD_FOLDER/iphoneos"

              # Build the simulator library.
              echo "Building the iOS Simulator static library ..."

              xcrun xcodebuild -configuration "Release" \
                  -project $XCODE_PROJECT \
                  -target $XCODE_TARGET \
                  -sdk "iphonesimulator" \
                  -arch i386 -arch x86_64 \
                  build \
                  ONLY_ACTIVE_ARCH=NO \
                  RUN_CLANG_STATIC_ANALYZER=NO \
                  BUILD_DIR="build/$BUILD_FOLDER" \
                  SYMROOT="build/$BUILD_FOLDER" \
                  OBJROOT="build/$BUILD_FOLDER/obj" \
                  BUILD_ROOT="build/$BUILD_FOLDER" \
                  TARGET_BUILD_DIR="build/$BUILD_FOLDER/iphonesimulator"

              # Join both libraries into one 'fat' library.
              echo "Creating fat library ..."

              xcrun -sdk iphoneos lipo -create \
                  -output "build/$BUILD_FOLDER/$STATIC_LIBRARY" \
                  "build/$BUILD_FOLDER/iphoneos/$STATIC_LIBRARY" \
                  "build/$BUILD_FOLDER/iphonesimulator/$STATIC_LIBRARY"

              echo "The fat static library '$STATIC_LIBRARY' is ready."
            popd

            # Copy the result into the Libs folder.
            echo "Copying '$STATIC_LIBRARY' into Libs."
            cp "$PROJECT_PATH/build/$BUILD_FOLDER/$STATIC_LIBRARY" .
        fi
    popd
}

# Create our main SDL2 static library if necessary and put it into the Libs folder.
create_static_library \
    libSDL2.a \
    ../../../third-party/SDL/Xcode-iOS/SDL \
    SDL.xcodeproj \
    libSDL-iOS \
    SDL

# Create our SDL_image static library if necessary and put it into the Libs folder.
create_static_library \
    libSDL2_image.a \
    ../../../third-party/SDL2_image/Xcode-iOS \
    SDL_image.xcodeproj \
    libSDL_image-iOS \
    SDL_image

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

# Check to see if we have the MoltenVK framework and dynamic library
verify_frameworks_folder_exists
pushd "Frameworks"
  if [ ! -d "MoltenVK.framework" ]; then
    cp -R ../../../third-party/vulkan-mac/MoltenVK/iOS/framework/MoltenVK.framework .
  fi

  if [ ! -e "libMoltenVK.dylib" ]; then
    cp ../../../third-party/vulkan-mac/MoltenVK/iOS/dynamic/libMoltenVK.dylib .
  fi
popd

# Invoke the xcodegen tool to create our project file.
echo "Generating Xcode project"
xcodegen generate

compile_vulkan_shaders