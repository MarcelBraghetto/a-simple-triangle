#!/bin/bash

# Include the shared scripts.
. ../shared-scripts.sh

# Ask Homebrew to fetch our required programs
fetch_brew_dependency "wget"
fetch_brew_dependency "cmake"
fetch_brew_dependency "ninja"

fetch_third_party_lib_sdl
fetch_framework_sdl2