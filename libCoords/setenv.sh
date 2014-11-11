#!/usr/bin/env bash
#
# Shell wrapper to set up OS X python environment
#
# TODO: add linux support
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

export DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:${COORD_LIBRARY_PATH}:${GTEST_LIBRARY_PATH}

# -----------------------
# ----- echo result -----
# -----------------------

echo '# DYLD_LIBRARY_PATH' ${DYLD_LIBRARY_PATH}
echo  # linefeed

# EoF
