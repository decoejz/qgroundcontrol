#!/bin/bash

echo "Build from zero?"
echo -en "Res (y|N): "
read -r BUILDING_POINT

if [ "$BUILDING_POINT" == "y" ]
then
    rm -rf build/
    cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
fi

cmake --build build --config Debug

echo
echo
echo "To run code execute:"
echo "./build/QGroundControl.app/Contents/MacOS/QGroundControl"