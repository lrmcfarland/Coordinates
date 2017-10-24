#!/bin/bash
#
# This contains utilities for the setenv.sh wrappers
#
#

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

