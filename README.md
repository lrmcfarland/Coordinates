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

"""Calculates the position of the sun for the given datetime.

http://en.wikipedia.org/wiki/Position_of_the_Sun


to calculate:
    http://www.nrel.gov/docs/fy08osti/34302.pdf

to validate:

    http://www.esrl.noaa.gov/gmd/grad/solcalc/

to run:

$ ./pylaunch.sh SunPosition.py -v -- 37:24 -122:04:57 2015-03-21T12:57:00-08


Note: use -- to end options and allow for negative coordinates.

"""

import math

import coords

from Transforms import EclipticEquatorial
from Transforms import EquatorialHorizon
from Transforms import GMST
from Transforms import utils


class Error(Exception):
    pass


def SolarLongitude(a_datetime):
    """Calculate the longitude of the sun for the given date

    from http://en.wikipedia.org/wiki/Position_of_the_Sun

    Args:

    a_datetime: The time of the observation (coords.datetime).

    Returns: the sun's longitude and distance in AU
    """

    n = a_datetime.toJulianDate() - a_datetime.J2000

    # mean longitude
    L = coords.angle(280.460 + 0.9856474*n)
    L.normalize(0, 360)

    # ecliptic longitude
    g = coords.angle(357.528 + 0.9856003*n)
    g.normalize(0, 360)

    # ecliptic longitude
    ecliptic_longitude = coords.angle(L.value + 1.915*math.sin(g.radians) + 0.020*math.sin(2.0*g.radians))

    # distance to sun in AU
    R = 1.00014 - 0.01671*math.cos(g.radians) - 0.00014*math.cos(2.0*g.radians)

    return ecliptic_longitude, R


def SunPosition(a_datetime, an_observer):
    """Calculate the location of the sun relaive to an observer

    Args:

    a_datetime: The time of the observation (coords.datetime).

    an_observer: The observers location (coords.spherical).

    Returns: the position of the sun (coords.spherical).
    """

    ecliptic_longitude, R = SolarLongitude(a_datetime)

    sun_ec = coords.spherical(R, coords.angle(90), ecliptic_longitude)
    sun_eq = EclipticEquatorial.toEquatorial(sun_ec, a_datetime)
    sun_hz = EquatorialHorizon.toHorizon(sun_eq, an_observer, a_datetime)

    return sun_hz


def EquationOfTime(a_datetime):
    """Calcuate the equation of time

    from http://en.wikipedia.org/wiki/Equation_of_time

    TODO: Only valid at for noon. Rounds to nearest half day.

    Args:

    a_datetime: The time of the observation (coords.datetime).

    Returns: equation of time as a coords angle in degrees. *60 for minutes.
    """

    noon = coords.datetime()
    noon.fromJulianDate(math.floor(a_datetime.toJulianDate()))
    gast = GMST.USNO_C163.GAST(noon)

    ecliptic_longitude, R = SolarLongitude(noon)

    sun_ec = coords.spherical(R, coords.angle(90), ecliptic_longitude)
    sun_eq = EclipticEquatorial.toEquatorial(sun_ec, noon)

    eot = coords.angle()

    sun_ra = utils.get_RA(sun_eq)

    if gast.value - sun_ra.value < 12:
        eot.value = gast.value - sun_ra.value

    elif gast.value - sun_ra.value >= 12:
        eot.value = gast.value - sun_ra.value - 24

    else:
        raise Error('unsupported EoT case')

    return eot


# ================
# ===== main =====
# ================


if __name__ == '__main__':

    # -------------------------
    # ----- parse options -----
    # -------------------------

    import optparse

    defaults = {'isVerbose': False}

    usage = '%prog [options] <latitude> <longitude> <datetime>'

    parser = optparse.OptionParser(usage=usage)

    parser.add_option('-v', '--verbose',
                      action='store_true', dest='verbose',
                      default=defaults['isVerbose'],
                      help='verbose [%default]')

    options, args = parser.parse_args()

    # ----- validate -----

    if len(args) < 3:
        parser.error('missing object and/or datetime.')

    an_observer = utils.latlon2spherical(a_latitude=utils.parse_angle_arg(args[0]),
                                         a_longitude=utils.parse_angle_arg(args[1]))

    a_datetime = coords.datetime(args[2])

    # ----------------------------------
    # ----- calculate sun position -----
    # ----------------------------------

    # azimuth, altitude

    print 'A datetime: ', a_datetime
    print 'An observer:', an_observer

    ecliptic_longitude, R = SolarLongitude(a_datetime)
    print 'Ecliptic longitude:', ecliptic_longitude
    print 'Distance in AU:', R

    sun_ec = coords.spherical(R, coords.angle(90), ecliptic_longitude)
    print 'Sun in ecliptic coordinates:\n\t', sun_ec

    sun_eq = EclipticEquatorial.toEquatorial(sun_ec, a_datetime)
    print 'Sun in equatorial coordinates:\n\t', sun_eq

    sun_hz = EquatorialHorizon.toHorizon(sun_eq, an_observer, a_datetime)
    print 'Sun in horizon coordinates:\n\t', sun_hz

    print 'Azimuth (degrees):', utils.get_azimuth(sun_hz),
    print ''.join(('(', str(utils.get_azimuth(sun_hz).value), ')'))
    print 'Altitude (degrees):', utils.get_altitude(sun_hz),
    print ''.join(('(', str(utils.get_altitude(sun_hz).value), ')'))

    eot = EquationOfTime(a_datetime)

    print 'Equation of time (minutes):', eot.value * 60

```

To run

```

$ ./pylaunch.sh SunPosition.py -v -- 37:24 -122:04:57 2015-04-24T10:00:00-07
# COORDS_ROOT not set. Using ..
# coords.so: ../Coordinates/Python/Manual/build/lib.macosx-10.10-intel-2.7/coords.so
# DYLD_LIBRARY_PATH :../Coordinates/libCoords
# PYTHONPATH :../Coordinates/Python/Manual/build/lib.macosx-10.10-intel-2.7:..

A datetime:  2015-04-24T10:00:00-07
An observer: <spherical><r>1</r><theta>52.6</theta><phi>-122.0825</phi></spherical>
Ecliptic longitude: 33:37:59.2735
Distance in AU: 1.00559097002
Sun in ecliptic coordinates:
	<spherical><r>1.00559097002</r><theta>90</theta><phi>33.6331315168</phi></spherical>
Sun in equatorial coordinates:
	<spherical><r>1.00559097002</r><theta>77.2733216974</theta><phi>31.3977138404</phi></spherical>
Sun in horizon coordinates:
	<spherical><r>1</r><theta>48.2602377782</theta><phi>108.089661148</phi></spherical>
Azimuth (degrees): 108:05:22.7801 (108.089661148)
Altitude (degrees): 41:44:23.144 (41.7397622218)
Equation of time (minutes): 1.61885019052


```
