# Coordinates

This repo holds my Coordinates library for implementing physics and astronomy applications.
It is implemented in C++ and has Python wrappers generated with
[Boost.python](https://wiki.python.org/moin/boost.python/GettingStarted),
[SWIG](http://www.swig.org/)
and
[extensions using just
Python](https://docs.python.org/3/extending/extending.html).
The Python extension makes this simple to incorporate into a python
web framework like [flask](http://flask.pocoo.org/) (See
[AAI](https://aai.starbug.com/)).
All objects are in the name space "Coords", e.g. Coords::Cartesian.


# To Install

Clone the repo from here. This has googletest as a sub-module to
build the unit tests, but it is not necessary to build just the coords
library.
The [build.sh](build.sh) script will run

```
    git submodule update --init --recursive

```


## cmake

googletest will also need cmake installed, e.g.

### On OS X

```
    brew install cmake
```

## Boost

Boost is needed to build the Python Boost wrappers.

### On OS X

```
    brew install boost --with-python
    brew install boost-python
    brew install boost-python3
```

## swig

### On OS X

```
    brew install swig
```


# To build

The top level [build.sh](build.sh) script will build all libraries.
libCoords must be built first.
Python/Boost and Python/Manual are built next.
It will pass targets through to the Makefiles, like all (a.k.a. the
default), clean and test.

```
    $ pwd
    ~/Coordinates/
    $ ./build.sh clean
    $ ./build.sh test
```

Each directory has its own Makefile with 'build', 'test', and 'clean'
targets. build.sh simply runs the Makefile in each of the sub directories.

```
    $ pwd
    ~/Coordinates/libCoords
    $ make clean
    $ make test
```

Each part can be built by going into each directory in turn and
running the make command, but libCoords needs to be the first.
Details on this can be found in [libCoords](libCoords/README.md).
libCoords and the "manual" python extension can be built with
out additional packages.
A complete build of Coordinates uses gtest and Boost.

To build the Boost wrappers you will, of course, need to install
[Boost](http://www.boost.org) with python. Details on this can be found
in [Python/Boost](Python/Boost/README.md).


## OS X

I built this on my iMac using the [LLVM](http://llvm.org) compiler
that comes with [Xcode](https://developer.apple.com/xcode/).
Since this has an implementation of std::regex, I used that first.
I only used Boost for the Boost python wrappers.
Later, I added Boost regex to support Linux with GCC 4.8 compilers.

After installing Xcode from Apple, I used [homebrew](http://brew.sh)
to install Boost.

## Linux

The current (2015) CentOS and other Linux releases are using GCC 4.8
which has the prototype std::regex that throws regex_error on
my std::regex tests.
I experimented with upgrading to GCC 4.9, but had linking problems later on.
To work around this I use the Boost regex libraries.

[Dockerfile.centos](Dockerfile.centos) is an example that will build the library
in a CentOS container.

# To Run

## C++

[example1.cpp](libCoords/example1.cpp) demonstrates the C++ interface.
Here I use the spherical coordinates with the radius of the earth (in
kilometers), the latitude (which needs to be converted to the theta
angle from the positive z axis), and longitude of two locations to
calculate the distance between them.

```

// ============================================================
// Filename:    example1.cpp
//
// Description: Example of using libCoords to calculate the distance
//              between two places on earth given the latitude and
//              longitude.
//
// Authors:     L.R. McFarland
// Created:     2014nov11
// ============================================================

#include <iostream>

#include <angle.h>
#include <Cartesian.h>
#include <spherical.h>

int main () {

  const double Re(6371); // radius of earth in km

  Coords::spherical keplers(Re, Coords::Latitude(37, 27, 13), Coords::angle(-122, 10, 55));
  Coords::spherical booksinc(Re, Coords::Latitude(37, 23, 32.4852), Coords::angle(-122, 4, 46.2252));

  Coords::Cartesian delta(keplers - booksinc);

  std::cout << "keplers " << keplers << " - " << std::endl
	    << "books inc " << booksinc << " = " << std::endl
	    << "distance " << delta.magnitude() << " km" << std::endl;

  return 0;
}


```

```

[libCoords (master)]$ make example1
g++ -g -W -Wall -fPIC -I.   -c -o example1.o example1.cpp
g++ example1.o -o example1 -L. -lCoords

[libCoords]$ ./example1
keplers <spherical><r>6371</r><theta>52.5464</theta><phi>-122.182</phi></spherical> -
books inc <spherical><r>6371</r><theta>52.6076</theta><phi>-122.08</phi></spherical> =
distance 11.3235 km

```


## Python


see [SunPosition.py](https://github.com/lrmcfarland/Astronomy/blob/master/Bodies/SunPosition.py)

To run

```

$ ./pylaunch.sh SunPosition.py -v -- 37:24 -122:04:57 2015-06-20T09:00:00-07
# COORDS_ROOT not set. Using ..
# coords.so: ../Coordinates/Python/Manual/build/lib.macosx-10.10-intel-2.7/coords.so
# DYLD_LIBRARY_PATH :../Coordinates/libCoords
# PYTHONPATH :../Coordinates/Python/Manual/build/lib.macosx-10.10-intel-2.7:..

A datetime:  2015-06-20T09:00:00-07 (2457193.58333)
An observer: <spherical><r>1</r><theta>52.6</theta><phi>-122.0825</phi></spherical>
Ecliptic longitude: 88:27:51.4396
Distance in AU: 1.01614270588
Obliquity of the ecliptic: 23:26:14.2101
Sun in ecliptic coordinates: <spherical><r>1.01614270588</r><theta>90</theta><phi>88.4642887862</phi></spherical>
Sun in equatorial coordinates: <spherical><r>1.01614270588</r><theta>66.5716406896</theta><phi>88.3262683622</phi></spherical>
Sun in horizon coordinates: <spherical><r>1</r><theta>54.6345463459</theta><phi>85.9229363769</phi></spherical>
Solar Declination: 23:25:42.0935 (23.4283593104)
Equation of time (minutes): -1.28859183158
Azimuth (degrees): 85:55:22.571 (85.9229363769)
Altitude (degrees): 35:21:55.6332 (35.3654536541)
Rising : 2015-06-20T05:47:49.1767-07
Transit: 2015-06-20T13:10:4.5581-07
Setting: 2015-06-20T20:32:19.9395-07

```
