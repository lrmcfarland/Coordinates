# ===============================================================
# Filename:    Makefile
# Description: Makefile to run build, test commands
# Author:      L.R. McFarland
# Created:     2014 Nov 13
# ===============================================================

OSXFLAGS = env ARCHFLAGS="-arch x86_64"
RM       = rm -rf

all: build

build: setup.py coords.cpp
	${OSXFLAGS} python setup.py build

test: build test_angle.py test_Cartesian.py test_spherical.py
	./test_coords.sh -v

clean:
	-$(RM) build

clobber: clean
	-$(RM) *~
