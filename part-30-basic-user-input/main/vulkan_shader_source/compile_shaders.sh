#!/bin/sh

echo "Compiling Vulkan shaders ..."

# Check that we have a 'vulkan' shader asset folder
pushd ../assets/shaders
    if [ ! -d "vulkan" ]; then
        mkdir vulkan
    fi
popd

# Grab all the files in the current directory ending with 'vert' or 'frag'
# and iterate them one at a time, invoking the Vulkan shader compiler for each.
for FILE_PATH in *.vert *.frag; do
    FILE_NAME=$(basename $FILE_PATH)

    echo "Compiling Vulkan shader: ${FILE_NAME}"

    ../../../third-party/vulkan-mac/macOS/bin/glslangValidator \
        -V \
        --target-env vulkan1.0 \
        -o ../assets/shaders/vulkan/${FILE_NAME} \
        ${FILE_NAME}
done
