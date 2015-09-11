# ===============================================================
# Filename:    Makefile
# Description: makefile to run build, test commands
# Author:      L.R. McFarland
# Created:     2014 Nov 21
# ===============================================================

# Detect operating system flavor.
UNAME   := $(shell uname)

# VERBOSE="-v"

# ----------------
# ----- OS X -----
# ----------------

# .dylib is only for Darwin
ifeq ($(UNAME), Darwin)

OSFLAGS = env ARCHFLAGS="-arch x86_64"

endif

# -----------------
# ----- Linux -----
# -----------------

ifeq ($(UNAME), Linux)

CC = /usr/bin/g++
CFLAGS = "-g -W -Wall -fPIC -I. -std=c++11"
LDFLAGS = -L../../libCoords

OSFLAGS = CC=${CC} CFLAGS=${CFLAGS} LDFLAGS=${LDFLAGS}

endif


# -----------------
# ----- build -----
# -----------------

RM = rm -f

all: build

build: coords.cpp setup.py
	${OSFLAGS} python setup.py build

# this fails on with a datetime seg fault. datetime compile warnings about closure?

test: build test_angle test_Cartesian test_datetime test_spherical

test_angle: test_angle.py
	. ./setenv.sh; python ./test_angle.py $(VERBOSE)

test_Cartesian: test_Cartesian.py
	. ./setenv.sh; python ./test_Cartesian.py $(VERBOSE)

test_datetime: test_datetime.py
	. ./setenv.sh; python ./test_datetime.py $(VERBOSE)

test_spherical: test_spherical.py
	. ./setenv.sh; python ./test_spherical.py $(VERBOSE)


clean:
	-$(RM) -r build

clobber: clean
	-$(RM) *~
