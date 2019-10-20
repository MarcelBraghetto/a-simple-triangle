#!/bin/bash

# Include the shared scripts and utility methods that are common to all platforms.
. ../shared-scripts.sh

# We will assume that the Android SDK is in the default location that Android Studio installs it to.
ANDROID_SDK="/Users/$USER/Library/Android/sdk"
echo "Using Android SDK at: $ANDROID_SDK"

# We will assume that we'll use Java that is bundled with Android Studio.
export JAVA_HOME="/Applications/Android Studio.app/Contents/jre/jdk/Contents/Home"

# We will be using a specific version of the Android NDK.
NDK_VERSION="20.0.5594570"
ANDROID_NDK="$ANDROID_SDK/ndk/$NDK_VERSION"
echo "Using Android NDK at: $ANDROID_NDK"

# This command will automatically answer 'yes' to each of the licences that a user normally has to manually agree to.
echo "Auto accepting Android SDK licenses ..."
yes | $ANDROID_SDK/tools/bin/sdkmanager --licenses

# This is the collection of Android SDK components that our code base will need.
echo "Installing required Android SDK components ..."
$ANDROID_SDK/tools/bin/sdkmanager \
    "platform-tools" \
    "build-tools;28.0.3" \
    "tools" \
    "platforms;android-28" \
    "cmake;3.10.2.4988404" \
    "ndk;$NDK_VERSION"

fetch_brew_dependency "wget"

fetch_third_party_lib_sdl
fetch_third_party_lib_glm
fetch_third_party_lib_tiny_obj_loader

# If required, create the SDL symlink into the Android library project so it can include it in its build.
pushd sdl/jni
    if [ ! -d "SDL" ]; then
      echo "Linking SDL path to sdl/jni/SDL ..."
      ln -s ../../../../third-party/SDL SDL
    fi
popd

# Copy the Java classes from the SDL library source into the Android library project.
pushd sdl/src/main
    echo "Copying base SDL Java source to Android library project ..."
    cp -r ../../../../../third-party/SDL/android-project/app/src/main/java .
popd

# Check whether our main Android app has a root assets folder
pushd app/src/main
    if [ ! -d "assets" ]; then
      echo "Creating root 'assets' folder in app/src/main ..."
      mkdir assets
    fi
popd

# Check whether the root assets folder of our main Android app has a symlink to our shared assets
pushd app/src/main/assets
    if [ ! -d "assets" ]; then
      echo "Linking assets to ../../../../../main/assets"
      ln -s ../../../../../main/assets assets
    fi
popd
