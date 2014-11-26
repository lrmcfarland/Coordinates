# ===============================================================
# Filename:    Makefile
# Description: makefile to run build, test commands
# Author:      L.R. McFarland
# Created:     2014 Nov 21
# ===============================================================

OSXFLAGS = env ARCHFLAGS="-arch x86_64"
RM       = rm -rf

all: build

build: coords.cpp setup.py
	${OSXFLAGS} python setup.py build

test: build test_angle.py # test_Cartesian.py test_spherical.py
	./test_coords.sh -v

clean:
	-$(RM) build

clobber: clean
	-$(RM) *~
