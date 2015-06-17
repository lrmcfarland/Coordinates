# Coordinates

## Features

This is my repo of Coordinate classes for use in physics and astronomy
applications. The classes are implemented in C++ and wrapped in
Python. I make full use of operator overloading to focus on the physics.
This makes it possible to write a vector equation like:

```

F = m*a;

```

I started with a [C++ implementation](libCoords/Cartesian.h) of the
[Cartesian coordinate system](http://en.wikipedia.org/wiki/Cartesian_coordinate_system).
The class
[overloads](http://en.wikipedia.org/wiki/Operator_overloading) the
basic arithmetic operators (+, -, * and /) to make working
with distances, velocities and forces transparent.

I also created an [implementation](libCoords/spherical.h) of a
[spherical coordinate system](http://en.wikipedia.org/wiki/Spherical_coordinate_system)
with conversion constructors to convert between it and the Cartesian
coordinate system. This simplifies the basic astronomy operations
using Latitude/Longitude,
[right ascension](http://en.wikipedia.org/wiki/Right_ascension)/[declination](http://en.wikipedia.org/wiki/Declination).

All objects are in the name space "Coords", e.g. Coords::Cartesian.
At first this seems little weird in English, but then
[Descartes](http://en.wikipedia.org/wiki/René_Descartes) was French.


## To Install

While libCoords and the "manual" python extension can be built with
out additional packages, a complete build of Coordinates uses gtest
and Boost.

Coordinates uses [gtest](https://code.google.com/p/googletest/) to
validate the C++ library libCoords. Details on this can be found
in [libCoords](libCoords/README.md).

To build the Boost wrappers you will, of course, need to install
[Boost](http://www.boost.org) with python. Details on this can be found
in [Python/Boost](Python/Boost/README.md).


### OS X

I built this on my iMac using the g++ compiler that comes with
[Xcode](https://developer.apple.com/xcode/).

### Linux

On ubuntu I needed to upgrade to the g++-4.9 to get the regex library
for datetime. Details on this can be found
[libCoords/README.md](libCoords/README.md)

TODO: I am still working on how to make the Boost and Manual Python setup.py
use this instead of the default g++ compiler.


## To Build

The top level [build.sh](build.sh) script will build all libraries.
libCoords must be bulit first. Python/Boost and Python/Manual are
built next.

```
$ pwd
.../Coordinates/
$ ./build.sh clean
$ ./build.sh test
```

Each directory has its own Makefile with 'build', 'test', and 'clean'
targets. build.sh simply runs the Makefile in each of the sub directories.

```
$ pwd
.../Coordinates/libCoords
$ make clean
$ make test
```


## To Run

### C++

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


### Python


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
