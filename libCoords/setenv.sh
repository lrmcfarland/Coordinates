#!/bin/bash
#
# Shell wrapper for libCoords
#
#


. ../bin/utils.sh

COORDS_ROOT=$(find_coords_root .. )
COORDS_LIBRARY_PATH=${COORDS_ROOT}/libCoords

# ----------------------------
# ----- set library path -----
# ----------------------------

platform=`uname`

if [[ "$platform" == 'Darwin' ]]; then

    # assumes built in /usr/local like in the readme
    GTEST_DIR=$( find_gtest /usr/local gtest-all.o)
    echo '# GTEST_DIR found. Using:' ${GTEST_DIR}
    export DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:${COORDS_LIBRARY_PATH}:${GTEST_DIR}

elif [[ "$platform" == 'Linux' ]]; then

    # assumes installed in /usr/lib64 by yum
    GTEST_DIR=$( find_gtest /usr/ libgtest.so)
    echo '# GTEST_DIR found. Using:' ${GTEST_DIR}
    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${COORDS_LIBRARY_PATH}:${GTEST_DIR}

else
    echo "unsupported platform"
    exit 1
fi

# -----------------------
# ----- echo result -----
# -----------------------

echo '# DYLD_LIBRARY_PATH is' ${DYLD_LIBRARY_PATH}
echo '# LD_LIBRARY_PATH is' ${LD_LIBRARY_PATH}
