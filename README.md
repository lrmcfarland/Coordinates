# Coordinates

## Features

This repo contains C++ classes with Python wrappers of
[Cartesian](http://en.wikipedia.org/wiki/Cartesian_coordinate_system)
and
[spherical](http://en.wikipedia.org/wiki/Spherical_coordinate_system)
coordinate systems for use in physics applications. The objects are
[overloaded](http://en.wikipedia.org/wiki/Operator_overloading) to
support the basic arithmetic operators of + and -. Multiplying or
dividing by a double will scale the vector. Constructors for copy
and copy assign are provided along with conversion constructors
to transform Cartesian into Spherical coordinates and vice versa.

## To Build

Each directory has its own Makefile with 'build', 'test', and 'clean'
targets, e.g.

```
$ pwd
/Users/.../Coordinates/libCoords
$ make clean
$ make test
```

libCoords must be built first. There is a build.sh script that runs
the Makefiles in all the directories in the necessary order and takes
the same make targets as above.

```
$ pwd
/Users/.../Coords/
$ ./build.sh clean
$ ./build.sh test
```

Each directory has a suite of unit tests. libCoords uses
[gtest](https://code.google.com/p/googletest/). The Python
modules use the native Python unit test.

To build the Boost wrappers you will, of course, need to install
[Boost](http://www.boost.org).


### OSX

I built this on my iMac using the g++ compiler that comes with
[Xcode](https://developer.apple.com/xcode/), but there is nothing special
that should cause a problem for other compilers.

### Linux

TODO

## To Run

### C++

Here I use conversion constructors to convert between spherical and
Cartesian coordinates.

```

// ============================================================
// Filename:    example1.cpp
//
// Description: Example of using libCoords
//
// Authors:     L.R. McFarland
// Created:     2014nov11
// ============================================================

#include <iostream>

#include <angle.h>
#include <Cartesian.h>
#include <spherical.h>

int main () {

    Coords::spherical a(1, Coords::angle(45), Coords::angle(45));
    Coords::spherical b(1, Coords::angle(45), Coords::angle(-45));
    Coords::spherical c;
    c = a + b;

    // a and b are both sin(45) high along z axis
    // +0.5 along x axis (1.0 total)
    // and +/- 0.5 along the y axis (0.0 total)

    Coords::Cartesian cart_c(c);

    std::cout << "a (" << a << ") + " << std::endl
	      << "b (" << b << ") = " << std::endl
	      << "c (" << cart_c << ")" << std::endl;

  return 0;
}

```

```

[libCoords (master)]$ make example1
g++ -g -W -Wall -fPIC -I.   -c -o example1.o example1.cpp
g++ example1.o -o example1 -L. -lCoords

[libCoords (master)]$ ./example1.sh
# COORD_ORIGIN not set. Using ..
# GTEST_DIR not set. Using /usr/local/gtest-1.7.0
# DYLD_LIBRARY_PATH :../libCoords:/usr/local/gtest-1.7.0

a (<spherical><r>1</r><theta>45</theta><phi>45</phi></spherical>) +
b (<spherical><r>1</r><theta>45</theta><phi>-45</phi></spherical>) =
c (<Cartesian><x>1</x><y>0</y><z>1.41421</z></Cartesian>)

```


### Python

Here I use the spherical coordinates with the radius of the earth (in kilometers), the
latitude (which needs to be converted to the theta angle from the
positive z axis), and longitude of two locations to calculate the
distance between them.

```
>>> import coords
>>> keplers = coords.spherical(6371, coords.angle(90) - coords.angle(37, 27, 13), coords.angle(-122, 10, 55))
>>> booksinc = coords.spherical(6371, coords.angle(90) - coords.angle(37, 23, 32.4852), coords.angle(-122, 4, 46.2252))
>>> print keplers - booksinc
<spherical><r>11.3235</r><theta>61.4649</theta><phi>123.282</phi></spherical>
```