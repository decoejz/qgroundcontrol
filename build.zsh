#!/bin/bash

RED="\033[0;31m" # Red color
GREEN="\033[0;32m" # Green color
BLUE="\033[0;34m" # Blue color
NC='\033[0m' # No color

echo "Which sign scheme would you like to use?"
echo -en "1. No signature (${BLUE}default${NC})\n2. RSA\n\nRes: "
read -r DES_SIGN

if [ "$DES_SIGN" == "2" ]
then
    export SIGN_TYPE=RSA
else
    export SIGN_TYPE=no_sign
fi

rm -rf build/

# Build project
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build --config Debug

# echo "To run code execute:"
# echo
# echo "./build/px4_sitl_default/bin/px4"

