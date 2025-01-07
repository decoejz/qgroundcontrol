#!/bin/bash

RED="\033[0;31m" # Red color
GREEN="\033[0;32m" # Green color
BLUE="\033[0;34m" # Blue color
NC='\033[0m' # No color

echo "Build from zero?"
echo -en "Res (y|N): "
read -r BUILDING_POINT

if [ "$BUILDING_POINT" == "y" ]
then
    echo "Which sign scheme would you like to use?"
    echo -en "1. No signature (${BLUE}default${NC})\n2. RSA\n3. ECSDA\n\nRes: "
    read -r DES_SIGN

    if [ "$DES_SIGN" == "3" ]
    then
        export SIGN_TYPE=ECSDA
    elif [ "$DES_SIGN" == "2" ]
    then
        export SIGN_TYPE=RSA
    else
        export SIGN_TYPE=no_sign
    fi

    rm -rf build/
    cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
fi

cmake --build build --config Debug

echo
echo
echo "To run code execute:"
echo "./build/QGroundControl.app/Contents/MacOS/QGroundControl"