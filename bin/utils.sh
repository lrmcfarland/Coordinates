#!/bin/bash
#
# This contains utilities for the setenv.sh wrappers
#
#

# -----------------
# ----- gtest -----
# -----------------

find_gtest ()  {
    # find the gtest library if it is not already set
    # args: find root, gtest lib
    # assume shortest path is the best when there is more than one?

    if [ -z "$GTEST_DIR" ]; then

	for p in `find $1 -name $2 | sort -r`; do
	    GTEST_DIR=$( dirname ${p} )
	done

	if [ -z "$GTEST_DIR" ]; then
	    echo "GTEST not found"
	    exit 1
	fi
    fi

    echo $GTEST_DIR # bash return a string
}

find_coords_root () {
    # find Coordinates source root if it is not already set
    # args: find root

    if [ -z "$COORDS_ROOT" ]; then

	COORDS_ROOT=$( dirname `find $1 -name libCoords` )

	if [ -z "$COORDS_ROOT" ]; then
	    echo "COORDS_ROOT not found."
	    exit 1
	fi
    fi

    echo $COORDS_ROOT
}

