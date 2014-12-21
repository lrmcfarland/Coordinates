#!/usr/bin/env bash
#
# Shell wrapper to set up the python environment
#

# -------------------------
# ----- Coords origin -----
# -------------------------

if [ -n "$COORD_ORIGIN" ]; then
    echo "# COORD_ORIGIN is" $COORD_ORIGIN
else
    COORD_ORIGIN=..
    echo "# COORD_ORIGIN not set. Using" $COORD_ORIGIN
fi

# ----------------------
# ----- Gtest root -----
# ----------------------

if [ -n "$GTEST_DIR" ]; then
    echo "# GTEST_DIR is" $COORD_ORIGIN
else
    GTEST_DIR=/usr/local/gtest-1.7.0
    echo "# GTEST_DIR not set. Using" $GTEST_DIR
fi


# ----------------------------
# ----- set library path -----
# ----------------------------

COORD_LIBRARY_PATH=${COORD_ORIGIN}/libCoords
GTEST_LIBRARY_PATH=${GTEST_DIR}

platform=`uname`

if [[ "$platform" == 'Darwin' ]]; then
    export DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:${COORD_LIBRARY_PATH}:${GTEST_LIBRARY_PATH}
    echo '# DYLD_LIBRARY_PATH' ${DYLD_LIBRARY_PATH}
    echo  # linefeed

elif [[ "$platform" == 'Linux' ]]; then
    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${COORD_LIBRARY_PATH}:${GTEST_LIBRARY_PATH}
    echo '# LD_LIBRARY_PATH' ${LD_LIBRARY_PATH}
    echo  # linefeed

else
    echo "unsupported platform"
    exit 1
fi

# -----------------------
# ----- echo result -----
# -----------------------


# EoF
