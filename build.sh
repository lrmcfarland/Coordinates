#!/usr/bin/env bash

# Takes make target from command line and walks source directories in
# order, i.e. libs before pys.

libs="./libCoords"
pys="./Python/Manual ./Python/Boost" # TODO ./python/SWIG"

# ---------------------
# ----- Utilities -----
# ---------------------

make_me() {
    # takes make target and list of directories, changes working
    # directory to each directory list element and makes the target.

    home_dir=`pwd`

    make_target="${1}"
    shift
    source_dirs=("$@")

    for source_dir in ${source_dirs[@]}; do
	echo "======================================================================"
	echo $source_dir
	cd $source_dir
	make $make_target
	cd $home_dir
    done

}

# -----------------------------------------
# -----process command line arguments -----
# -----------------------------------------

if [ $# -ne 1 ]
then
    TARGET="all" # default target
else
    TARGET=$1
fi

make_me $TARGET $libs
make_me $TARGET $pys

echo 'Done'
