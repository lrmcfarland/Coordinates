#!/bin/bash
#
# This is a shell wrapper to set up the Boost Python environment.
#
#

. ../../bin/utils.sh

COORDS_ROOT=$(find_coords_root ../.. )
COORDS_LIBRARY_PATH=${COORDS_ROOT}/libCoords

# ----------------------------
# ----- set library path -----
# ----------------------------

platform=`uname`

if [[ "$platform" == 'Darwin' ]]; then

    export DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:${COORDS_LIBRARY_PATH}

elif [[ "$platform" == 'Linux' ]]; then

    export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${COORDS_LIBRARY_PATH}

else
    echo "unsupported platform"
    exit 1
fi

# ---------------------------
# ----- set python path -----
# ---------------------------

COORDS_SO=`find . -name coords.so`

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

echo '# PYTHONPATH' ${PYTHONPATH}
