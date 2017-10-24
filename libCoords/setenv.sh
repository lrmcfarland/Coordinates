#!/bin/bash
#
# Shell wrapper for libCoords
#
#


. ../bin/utils.sh

COORDS_ROOT=$(find_coords_root .. )
COORDS_LIBRARY_PATH=${COORDS_ROOT}/libCoords
GTEST_LIBRARY_PATH=${COORDS_ROOT}/googletest/googlemock/gtest/

# ----------------------------
# ----- set library path -----
# ----------------------------

platform=`uname`

if [[ "$platform" == 'Darwin' ]]; then

    export DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:${COORDS_LIBRARY_PATH}:${GTEST_LIBRARY_PATH}

elif [[ "$platform" == 'Linux' ]]; then

    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${COORDS_LIBRARY_PATH}:${GTEST_LIBRARY_PATH}

else
    echo "unsupported platform"
    exit 1
fi

# -----------------------
# ----- echo result -----
# -----------------------

echo '# DYLD_LIBRARY_PATH is' ${DYLD_LIBRARY_PATH}
echo '# LD_LIBRARY_PATH is' ${LD_LIBRARY_PATH}
