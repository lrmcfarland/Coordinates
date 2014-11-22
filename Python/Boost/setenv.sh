#!/usr/bin/env bash
#
# Shell wrapper to set up OS X python environment
#
# TODO: add linux support
#

# -----------------------
# ----- Coords root -----
# -----------------------

if [ -n "$COORDS_ROOT" ]; then
    echo "# COORDS_ROOT is" $COORDS_ROOT
else
    COORDS_ROOT=../..
    echo "# COORDS_ROOT not set. Using" $COORDS_ROOT
fi

# ----------------------------
# ----- set library path -----
# ----------------------------

CARTESIAN_LIBRARY_PATH=${COORDS_ROOT}/libCoords
export DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:${CARTESIAN_LIBRARY_PATH}

# ---------------------------
# ----- set python path -----
# ---------------------------

COORDS_PYTHON_ROOT=Python/Boost # TODO remove duplication with Manual

COORDS_SO=`find ${COORDS_ROOT}/${COORDS_PYTHON_ROOT} -name coords.so`

if [ -n "$COORDS_SO" ]; then
    echo "# coords.so:" $COORDS_SO
    export PYTHONPATH=${PYTHONPATH}:$(dirname ${COORDS_SO})
else
    echo "coords.so not found"
    exit 1
fi

# -----------------------
# ----- echo result -----
# -----------------------

echo '# DYLD_LIBRARY_PATH' ${DYLD_LIBRARY_PATH}
echo '# PYTHONPATH' ${PYTHONPATH}
echo  # linefeed

# EoF
