# Python Boost wrappers

## [starbug.com coordinate Python wrappers](https://github.com/lrmcfarland/Coordinates)

This module contains the source to generate Python wrappers for the
C++ classes in [libCoords](https://github.com/lrmcfarland/Coordinates/tree/adds-pypi-v2/libCoords) using Boost.  This
library is assumed to have been built first. coords.cpp contains the
Boost macros that are used to generate the wrappers. setup.py builds
them.

There are some differences from the [manual](https://github.com/lrmcfarland/Coordinates/tree/master/Python/Manual) version,
like not having a coords.Error exception but using RuntimeError
instead. The static unit vectors have also moved from coords.Ux
to coords.Cartesian.Ux. Similarly for Uy, Uz, and Uo.

# To Build

The build is done using make on the command line. There are targets
for clean and test. See the Makefile for details.

This Makefile will not build libCoords. That must be done before this.

setup.py expects to find libCoords.dylib in libCoords and Boost
to be in /usr/local. See setup.py for details.

## Build Tools

### [Boost](http://www.boost.org)

The Boost Python library must be installed before this build will
work.


#### On OS X with brew

The easiest way I found to install and maintain boost on OS X is to
use [homebrew](http://brew.sh). Open the boost-python bottle. The
python wrappers are not in the "regular" boost bottle.


```
$ brew install boost-python  --with-python3
```

#### On CentOS with yum

```
sudo yum install boost-devel
```

#### On Ubuntu with apt-get


```
sudo apt-get install libboost-all-dev
```


#### from source

To built and install from the
[boost](http://www.boost.org/users/history/version_1_56_0.html)
source, unpack the tar-ball in /usr/local

```
cd /usr/local/boost_1_56_0

./bootstrap.sh
./b2
sudo ./b2 install
```

The boost files are now in /usr/local/include/boost and
/usr/local/lib/libboost_*.  python/Boost/setup.py sets BOOST_ROOT to
point there. On OS X brew doctor will notice and complain about this.


# The run time environment with setenv.sh

On the local host, relative to this directory, the runtime environment
expects libCoords to be built.

On OS X, this needs to be on the DYLD_LIBRARY_PATH.
On Linux, this needs to be on the LD_LIBRARY_PATH.

The PYTHONPATH must contain the boost coords.so.

setenv.sh will detect the platform using uname and set the appropriate
library path and use COORDS_ORIGIN environment variable to find
coords.so. If COORDS_ORIGIN is not set, it will assume it is relative
to this directory.

## An Interactive Session

You can use pylaunch.sh to set up an interactive session.

Here I use the spherical coordinates with the radius of the earth, the
latitude (which needs to be converted to the theta angle from the
positive z axis), and longitude of two locations to calculate the
distance between them.

```
$ ./pylaunch.sh
# COORDS_ROOT not set. Using ../..
# coords.so: ../../python/Boost/build/lib.macosx-10.9-intel-2.7/coords.so
# DYLD_LIBRARY_PATH :../../libCoords
# PYTHONPATH :../../python/Boost/build/lib.macosx-10.9-intel-2.7

Python 2.7.5 (default, Mar  9 2014, 22:15:05)
[GCC 4.2.1 Compatible Apple LLVM 5.0 (clang-500.0.68)] on darwin
Type "help", "copyright", "credits" or "license" for more information.
>>> import coords
>>> keplers = coords.spherical(6371, coords.angle(90) - coords.angle(37, 27, 13), coords.angle(-122, 10, 55))
>>> booksinc = coords.spherical(6371, coords.angle(90) - coords.angle(37, 23, 32.4852), coords.angle(-122, 4, 46.2252))
>>> print keplers - booksinc
<spherical><r>11.3235</r><theta>61.4649</theta><phi>123.282</phi></spherical>

```


# To test a component wiht pylaunch.sh

Use pylaunch.sh to call setenv.sh. Select one of the associated python
unittest scripts, e.g. test_Cartesian.py, along with the usual command
line options like -v

```
$ ./pylaunch.sh test_angle.py -v

# COORDS_ROOT not set. Using ../..
# coords.so: ../../Python/Boost/build/lib.macosx-10.9-intel-2.7/coords.so
# DYLD_LIBRARY_PATH :../../libCoords
# PYTHONPATH :../../Python/Boost/build/lib.macosx-10.9-intel-2.7

test_accessors_1 (__main__.TestAngle)
Test value accessors 1 ... ok
test_accessors_1b (__main__.TestAngle)
Test value accessor exception ... skipped 'TODO Boost.Python.ArgumentError'
test_accessors_2 (__main__.TestAngle)
Test radians accessor negative ... ok
test_accessors_2a (__main__.TestAngle)
Test radians accessor exception ... skipped 'TODO Boost.Python.ArgumentError'
test_angle1_eq_angle1 (__main__.TestAngle)
Test richcompare operator==() ... ok
...
Test latitude += latitude ... ok
test_inplace_add_2 (__main__.TestLatitude)
Test latitude += angle ... ok
test_inplace_add_over_limit (__main__.TestLatitude)
Test latitude += latitude ... ok

----------------------------------------------------------------------
Ran 49 tests in 0.003s

OK (skipped=5)
```

# To run a single unit test

From the command line, pass in the UnitTest test harness and test name,
e.g. TestCartesian, test_default_constructor to the test script

```
$ ./pylaunch.sh test_Cartesian.py -v TestCartesian.test_default_constructor

# COORDS_ROOT not set. Using ../..
# coords.so: ../../Python/Boost/build/lib.macosx-10.9-intel-2.7/coords.so
# DYLD_LIBRARY_PATH :../../libCoords
# PYTHONPATH :../../Python/Boost/build/lib.macosx-10.9-intel-2.7

test_default_constructor (__main__.TestCartesian)
Test default constructor ... ok

----------------------------------------------------------------------
Ran 1 test in 0.000s

OK
```


# To test all components

```
$ ./test_coords.sh -v
```


# PyPI dist

The Makefile as a dist target to build the python distribution files.

```
$ pwd
.../Coordinates/Python/Boost

$ make dist
env ARCHFLAGS="-arch x86_64" python setup.py build

/Users/lrm/.pyenv/versions/3.7.0/lib/python3.7/distutils/dist.py:274: UserWarning: Unknown distribution option: 'long_description_content_type'
  warnings.warn(msg)
```

TODO: It complains about long_description_content_type, but it doesn't render correctly with out it.

Make clean will remove them

```
$ make clean
rm -f -r build dist coords.egg-info
```


# PyPI

I use the [packaging python projects
instructions](https://packaging.python.org/tutorials/packaging-projects/)
to push builds to a PyPI distribution.

You will need to increment the version number in setup.py to push a new package.

Use twine to upload the distribution


## To Test PyPI

```
$ twine upload --repository-url https://test.pypi.org/legacy/ dist/*
Enter your username: lrmcfarland
Enter your password:
Uploading distributions to https://pypi.org/
Uploading coords-1.0.1-cp37-cp37m-macosx_10_13_x86_64.whl
100%|███████████████████████████████████████████████████████████████████████████████████████████████████████████████████████| 129k/129k [00:02<00:00, 58.4kB/s]
Uploading coords-1.0.1.tar.gz
100%|█████████████████████████████████████████████████████████████████████████████████████████████████████████████████████| 13.8k/13.8k [00:01<00:00, 7.83kB/s]
```

To install the test distribution

```

$ pip install -i https://test.pypi.org/simple/ starbug.coords
Looking in indexes: https://test.pypi.org/simple/
Collecting coords
  Downloading https://test-files.pythonhosted.org/packages/e3/6f/0476d2792045eb0185e039e52b29991fc32bb9c4ceabc178053d11c276e0/coords-1.0-cp37-cp37m-macosx_10_13_x86_64.whl (123kB)
    100% |████████████████████████████████| 133kB 1.4MB/s
Installing collected packages: coords
Successfully installed coords-1.0

```

## To PyPI

```
$ twine upload dist/*
```

To install the test distribution

```
$ pip install -i https://pypi.org/project/ starbug.coords
```


# To use

```
(test-coords) $ python
Python 3.7.0 (default, Jul  1 2018, 12:43:10)
[Clang 9.1.0 (clang-902.0.39.2)] on darwin
Type "help", "copyright", "credits" or "license" for more information.

>>> import starbug.coords as coords

>>> keplers = coords.spherical(6371, coords.angle(90) - coords.angle(37, 27, 13), coords.angle(-122, 10, 55))

>>> booksinc = coords.spherical(6371, coords.angle(90) - coords.angle(37, 23, 32.4852), coords.angle(-122, 4, 46.2252))

>>> str(keplers - booksinc)
'<spherical><r>11.3235</r><theta>61.4649</theta><phi>123.282</phi></spherical>'

```
