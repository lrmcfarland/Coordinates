# ===============================================================
# Filename:    Makefile
# Description: Makefile to run build, test commands
# Author:      L.R. McFarland
# Created:     2014nov13
# ===============================================================

OSXFLAGS = env ARCHFLAGS="-arch x86_64"

all: build

build: setup.py coords.cpp
	${OSXFLAGS} python setup.py build

test: build test_angle.py test_Cartesian.py test_spherical.py
	./test_coords.sh -v

clean:
	rm -rf build

clobber: clean
	rm *~
