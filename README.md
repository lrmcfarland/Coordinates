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


```

#!/usr/bin/env python

"""Transforming ecliptic and equitorial coordinates

Equitorial Coordinate System http://en.wikipedia.org/wiki/Equatorial_coordinate_system
    right ascension http://en.wikipedia.org/wiki/Right_ascension
    declination http://en.wikipedia.org/wiki/Declination

Ecliptic Coordinate System http://en.wikipedia.org/wiki/Ecliptic_coordinate_system
    ecliptic longitude
    ecliptic latitude

to run: ./pylaunch.sh ecliptic.py

See also:
    http://en.wikipedia.org/wiki/Ecliptic
    http://en.wikipedia.org/wiki/Axial_tilt
    http://lambda.gsfc.nasa.gov/toolbox/tb_converters_ov.cfm
"""

import math
import coords

class EclipticEquitorialTransforms(object):

    """Transforms Cartesian space vectors to/from equitorial/ecliptic coordinates

    ASSUMES: The x-axis points to vernal equinox. Positive rotations are right hand rule,
    Y x Z = X, i.e. counter clockwise looking down X.
    """

    # x axis points to vernal equinox (the first point of Aries in this epoch)
    equinox_axis = coords.rotator(coords.Ux)

    # obliquiy of the ecliptic terms are from http://en.wikipedia.org/wiki/Axial_tilt
    obe = list()
    obe.append(coords.angle(23, 26, 21.45))
    obe.append(coords.angle(-1)*coords.angle(0, 0, 46.815)) # TODO no unary minus in boost wrappers
    obe.append(coords.angle(-1)*coords.angle(0, 0, 0.0006))
    obe.append(coords.angle(0, 0, 0.00181))
    # TODO more terms, updated


    def eps(self, a_datetime):
        """Calculates the obliquty of the ecliptic given the datetime"""
        T = (a_datetime.toJulianDate() - a_datetime.J2000)/36525.0
        the_eps = 0
        for i in xrange(len(self.obe)):
            the_eps += self.obe[i].value * math.pow(T, i)
        return the_eps


    def _xform(self, a_vector, a_datetime, a_direction):
        """Transforms a vector ecliptic to/from equitorial/ecliptic coordinates.

        Args:
        a_vector: the vector to transform
        a_datetime: the time of the transformation
        a_direction: +1 to equitorial, -1 to ecliptic

        Returns a vector in the transformed coordinates
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
        return self._xform(a_vector, a_datetime, -1.0)


    def toEquitorial(self, a_vector, a_datetime):
        """Transforms a_vector from ecliptic to equitorial coordinates

        Returns a Cartesian vector in equitorial coordinates
        """
        return self._xform(a_vector, a_datetime, 1.0)


    @staticmethod
    def theta2latitude(an_angle):
        """Converts spherical coordinate theta (angle to +z axis) to latitude/declination"""
        return coords.angle(90 - an_angle.theta.value)


    @staticmethod
    def phi2longitude(an_angle):
        """Converts spherical coordinate phi (angle to +x axis of projection in xy plane) to longitude"""
        if an_angle.phi.value < 0:
            return coords.angle(360 + an_angle.phi.value)
        else:
            return coords.angle(an_angle.phi.value)


if __name__ == '__main__':

    # Actuals from http://lambda.gsfc.nasa.gov/toolbox/tb_coordconv.cfm

    eceq_xform = EclipticEquitorialTransforms()

    j2000 = coords.datetime('2000-01-01T00:00:00')

    print "For datetime:", j2000

    print '='*20
    print 'J2000 North pole'
    # Actual: Celestial J2000 +90:00:00.00 Latitude(deg)   +15:00:00.00 Longitude(deg)
    #         Ecliptic  J2000 +66:33:38.55 Latitude(deg)   +90:00:00.00 Longitude(deg)
    some_point = coords.spherical(1, coords.latitude(90), coords.angle(15))
    print 'some point:', some_point

    some_point_ec = eceq_xform.toEcliptic(some_point, j2000)
    print 'Ecliptic(some point)', some_point_ec
    print 'latitude:', EclipticEquitorialTransforms.theta2latitude(some_point_ec),
    print 'longitude:', EclipticEquitorialTransforms.phi2longitude(some_point_ec)
    # latitude: 66:33:38.5494 longitude: 90:00:00 Good

    # Actual: Ecliptic  J2000 +90:00:00.00 Latitude(deg)   +15:00:00.00 Longitude(deg)
    #         Celestial J2000 +66:33:38.55 Latitude(deg)   +270:00:00.00 Longitude(deg)
    some_point_eq = eceq_xform.toEquitorial(some_point, j2000)
    print 'Equitorial(some point)', some_point_eq
    print 'latitude:', EclipticEquitorialTransforms.theta2latitude(some_point_eq),
    print 'longitude:', EclipticEquitorialTransforms.phi2longitude(some_point_eq)
    # latitude: 66:33:38.5494 longitude: 270:00:00 Good


    print '='*20
    print 'J2000 latitude 45, longitude 15'
    # Actual: Celestial J2000 +45:00:00.00 Latitude(deg)   +15:00:00.00 Longitude(deg)
    #         Ecliptic  J2000 +35:10:00.68 Latitude(deg)   +33:19:50.99 Longitude(deg)
    some_point = coords.spherical(1, coords.latitude(45), coords.angle(15))
    print 'some point:', some_point

    some_point_ec = eceq_xform.toEcliptic(some_point, j2000)
    print 'Ecliptic(some point)', some_point_ec
    print 'latitude:', EclipticEquitorialTransforms.theta2latitude(some_point_ec),
    print 'longitude:', EclipticEquitorialTransforms.phi2longitude(some_point_ec)
    # latitude: 35:10:0.677949 longitude: 33:19:50.9946 Good

    # Actual: Ecliptic  J2000 +45:00:00.00 Latitude(deg)   +15:00:00.00 Longitude(deg)
    #         Celestial J2000 +46:10:59.17 Latitude(deg)   +350:34:35.34 Longitude(deg)
    some_point_eq = eceq_xform.toEquitorial(some_point, j2000)
    print 'Equitorial(some point)', some_point_eq
    print 'latitude:', EclipticEquitorialTransforms.theta2latitude(some_point_eq),
    print 'longitude:', EclipticEquitorialTransforms.phi2longitude(some_point_eq)
    # latitude: 46:10:59.1649 longitude: 350:34:35.3393 Good


    j2015 = coords.datetime('2015-01-01T00:00:00')

    print '='*20
    print 'J2015 latitude -60, longitude 200'
    # Actual: Celestial J2015 -60:00:00.00 Latitude(deg)   +200:00:00.00 Longitude(deg)
    #         Ecliptic  J2015 -46:35:54.38 Latitude(deg)   +226:51:30.34 Longitude(deg)
    some_point = coords.spherical(1, coords.declination(-60), coords.angle(200))
    print 'some point:', some_point

    some_point_ec = eceq_xform.toEcliptic(some_point, j2015)
    print 'Ecliptic(some point)', some_point_ec
    print 'latitude:', EclipticEquitorialTransforms.theta2latitude(some_point_ec),
    print 'longitude:', EclipticEquitorialTransforms.phi2longitude(some_point_ec)
    # latitude: -46:35:54.3761 longitude: 226:51:30.339 Good

    # Actual: Ecliptic  J2015 -60:00:00.00 Latitude(deg)   +200:00:00.00 Longitude(deg)
    #         Celestial J2015 -59:36:32.36 Latitude(deg)   +158:14:19.32 Longitude(deg)
    some_point_eq = eceq_xform.toEquitorial(some_point, j2015)
    print 'Equitorial(some point)', some_point_eq
    print 'latitude:', EclipticEquitorialTransforms.theta2latitude(some_point_eq),
    print 'longitude:', EclipticEquitorialTransforms.phi2longitude(some_point_eq)
    # latitude: -59:36:32.3636 longitude: 158:14:19.3202 Good




```

To run

```

$ ./pylaunch.sh EcEqXforms.py
# COORDS_ROOT not set. Using ..
# coords.so: ../Coordinates/Python/Manual/build/lib.macosx-10.10-intel-2.7/coords.so
# DYLD_LIBRARY_PATH :../Coordinates/libCoords
# PYTHONPATH :../Coordinates/Python/Manual/build/lib.macosx-10.10-intel-2.7

For datetime: 2000-01-01T00:00:00

====================
J2000 North pole
some point: <spherical><r>1</r><theta>0</theta><phi>15</phi></spherical>
Ecliptic(some point) <spherical><r>1</r><theta>23.4392918447</theta><phi>90</phi></spherical>
latitude: 66:33:38.5494 longitude: 90:00:00
Equitorial(some point) <spherical><r>1</r><theta>23.4392918447</theta><phi>-90</phi></spherical>
latitude: 66:33:38.5494 longitude: 270:00:00

====================
J2000 latitude 45, longitude 15
some point: <spherical><r>1</r><theta>45</theta><phi>15</phi></spherical>
Ecliptic(some point) <spherical><r>1</r><theta>54.8331450141</theta><phi>33.3308318461</phi></spherical>
latitude: 35:10:0.677949 longitude: 33:19:50.9946
Equitorial(some point) <spherical><r>1</r><theta>43.8168986259</theta><phi>-9.42351685438</phi></spherical>
latitude: 46:10:59.1649 longitude: 350:34:35.3393

====================
J2015 latitude -60, longitude 200
some point: <spherical><r>1</r><theta>150</theta><phi>200</phi></spherical>
Ecliptic(some point) <spherical><r>1</r><theta>136.598437793</theta><phi>-133.141572493</phi></spherical>
latitude: -46:35:54.3761 longitude: 226:51:30.339
Equitorial(some point) <spherical><r>1</r><theta>149.608989897</theta><phi>158.238700067</phi></spherical>
latitude: -59:36:32.3636 longitude: 158:14:19.3202


```
