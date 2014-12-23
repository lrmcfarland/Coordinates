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
basic arithmetic operators, +, -, * and / to make the basic physics
operations of adding distances, velocities and forces transparent.

I also created an [implementation](libCoords/spherical.h) of a
[spherical coordinate system](http://en.wikipedia.org/wiki/Spherical_coordinate_system)
with conversion constructors to convert between it and the Cartesian
coordinate system. This simplifies the basic astronomy operations
using Latitude/Longitude,
[Right ascension](http://en.wikipedia.org/wiki/Right_ascension) and
[declination](http://en.wikipedia.org/wiki/Declination).

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


```



#!/usr/bin/env python

"""Transforming ecliptic and equitorial coordinates

Equitorial Coordinate System (http://en.wikipedia.org/wiki/Equatorial_coordinate_system):
    right ascension (http://en.wikipedia.org/wiki/Right_ascension)
    declination (http://en.wikipedia.org/wiki/Declination)

Ecliptic Coordinate System (http://en.wikipedia.org/wiki/Ecliptic_coordinate_system):
    ecliptic longitude
    ecliptic latitude

to run: ./pylaunch.sh ecliptic.py

See also:
    http://en.wikipedia.org/wiki/Ecliptic
    http://en.wikipedia.org/wiki/Axial_tilt

    http://lambda.gsfc.nasa.gov/toolbox/tb_converters_ov.cfm

    http://ned.ipac.caltech.edu/cgi-bin/calc?in_csys=Equatorial&in_equinox=J2000.0&obs_epoch=2014&lon=2&lat=90&pa=0.0&out_csys=Ecliptic&out_equinox=J2000.0

"""

import math

import coords


class EclipticEquitorialTransforms(object):

    """Transforms Cartesian space vectors from equitorial and ecliptic coordinates

    ASSUMES: x-axis point to vernal equinox. Positive rotations are right hand rule,
    Y x Z = X, i.e. counter clockwise looking down X.
    """

    # x axis points to vernal equinox (the first point of Aries in this epoch)
    equinox_axis = coords.rotator(coords.Ux)

    # obliquiy of the ecliptic terms are from
    # http://en.wikipedia.org/wiki/Axial_tilt
    obterms = list()
    obterms.append(coords.angle(23, 26, 21.45))
    obterms.append(coords.angle(-1)*coords.angle(0, 0, 46.815)) # TODO no unary minus in boost wrappers
    obterms.append(coords.angle(-1)*coords.angle(0, 0, 0.0006))
    obterms.append(coords.angle(0, 0, 0.00181))
    # TODO more terms, updated

    def eps(self, a_datetime):
        """Calculates the obliquty of the ecliptic given the datetime"""
        T = (a_datetime.toJulianDate() - a_datetime.J2000)/36525.0
        the_eps = 0
        for i in xrange(len(self.obterms)):
            the_eps += self.obterms[i].value * math.pow(T, i)
        return the_eps


    def _xform(self, a_vector, a_datetime, a_direction):
        """Transforms a_vector from ecliptic coordinates into equitorial coordinates

        Args:
        a_vector: the vector to transform
        a_datetime: the time of the transformation
        a_direction: +1 to ecliptic, -1 to equitorial

        Returns a Cartesian vector in the transformed coordinates
        """
        if isinstance(a_vector, coords.spherical):
            the_rotatee = coords.Cartesian(a_vector)
        else:
            the_rotatee = a_vector

        the_rotated = self.equinox_axis.rotate(the_rotatee,
                                               coords.angle(a_direction * self.eps(a_datetime)))

        if isinstance(a_vector, coords.spherical):
            return coords.spherical(the_rotated)
        else:
            return the_rotated


    def toEcliptic(self, a_vector, a_datetime):
        """Transforms a_vector from equitorial to ecliptic coordinates

        Returns a Cartesian vector in eliptic coordinates
        """

        return self._xform(a_vector, a_datetime, 1.0)


    def toEquitorial(self, a_vector, a_datetime):
        """Transforms a_vector from ecliptic to equitorial coordinates

        Returns a Cartesian vector in equitorial coordinates
        """

        return self._xform(a_vector, a_datetime, -1.0)



if __name__ == '__main__':

    eceq_xform = EclipticEquitorialTransforms()

    a_date = coords.datetime('2000-01-01T00:00:00')

    print "For datetime:", a_date

    # vernal equinox
    print # linefeed
    print 'The First point of Aries (Ux) no-op'
    Ux = coords.spherical(coords.Ux)
    print 'Ux', Ux
    Ux_ec = eceq_xform.toEcliptic(Ux, a_date)
    print "Ux ecliptic", Ux_ec
    Ux_eq = eceq_xform.toEquitorial(Ux, a_date)
    print "Ux equitorial", Ux_eq


    # North Pole
    print # linefeed
    print 'The North pole (Uz)'
    Uz = coords.spherical(coords.Uz)
    print 'Uz', Uz
    Uz_ec = eceq_xform.toEcliptic(Uz, a_date)
    print 'Uz ec:', Uz_ec
    print 'Uz ec theta:', Uz_ec.theta
    Uz_eq = eceq_xform.toEquitorial(Uz_ec, a_date)
    print 'Uz toEquitorial(toEcliptic(Uz))', Uz_eq

    # tbd
    print '\n'

    some_point = coords.spherical(1, coords.declination(30), coords.angle(30))
    print 'some point:', some_point
    some_point_ec = eceq_xform.toEcliptic(some_point, a_date)
    print 'some point ec', some_point_ec

    print 'longitude:', some_point_ec.phi, some_point_ec.phi.value
    print 'dec:', coords.angle(90 - some_point_ec.theta.value)



```

To run

```

$ ./pylaunch.sh EcEqXforms.py
# COORDS_ROOT not set. Using ..
# coords.so: ../Coordinates/Python/Manual/build/lib.macosx-10.10-intel-2.7/coords.so
# DYLD_LIBRARY_PATH :../Coordinates/libCoords
# PYTHONPATH :../Coordinates/Python/Manual/build/lib.macosx-10.10-intel-2.7

For datetime: 2000-01-01T00:00:00

The First point of Aries (Ux) no-op
Ux <spherical><r>1</r><theta>90</theta><phi>0</phi></spherical>
Ux ecliptic <spherical><r>1</r><theta>90</theta><phi>-1.3955433756e-15</phi></spherical>
Ux equitorial <spherical><r>1</r><theta>90</theta><phi>1.3955433756e-15</phi></spherical>

The North pole (Uz)
Uz <spherical><r>1</r><theta>0</theta><phi>0</phi></spherical>
Uz ec: <spherical><r>1</r><theta>23.4392918447</theta><phi>-90</phi></spherical>
Uz ec theta: 23:26:21.4506
Uz toEquitorial(toEcliptic(Uz)) <spherical><r>1</r><theta>1.3955433756e-15</theta><phi>0</phi></spherical>


some point: <spherical><r>1</r><theta>60</theta><phi>30</phi></spherical>
some point ec <spherical><r>1</r><theta>50.8772722999</theta><phi>14.816726587</phi></spherical>
longitude: 14:49:0.215713 14.816726587
dec: 39:07:21.8197


```
