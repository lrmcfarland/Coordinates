# Coordinates library

## To Build

The build is done using make on the command line. There are targets for
build and test. See the Makefile for details.

## To run

### On OS X

On OS X, libCoords.dylib needs to be on the DYLD_LIBRARY_PATH along
with libgtest.a described above.

setenv.sh will use COORDS_ROOT to find these or assume they are
relative to this directory.

example1.sh uses this to set up its runtime environment for the
example1 executable.

### On Linux

TODO

## To test

On the command line, use the shell wrapper to set up the environment
(setenv.sh) and run the tests

```
$ ./spherical_unittest.sh
# COORD_ORIGIN not set. Using ..
# GTEST_DIR not set. Using /usr/local/gtest-1.7.0
# DYLD_LIBRARY_PATH :../libCoords:/usr/local/gtest-1.7.0

[==========] Running 43 tests from 2 test cases.
[----------] Global test environment set-up.
[----------] 42 tests from FixedSpherical
[ RUN      ] FixedSpherical.Accessors
[       OK ] FixedSpherical.Accessors (0 ms)
[ RUN      ] FixedSpherical.Equivalence
[       OK ] FixedSpherical.Equivalence (0 ms)
...

[----------] Global test environment tear-down
[==========] 43 tests from 2 test cases ran. (1 ms total)
[  PASSED  ] 43 tests.


```

To run a single test, use the --gtest_filter option to pass the test
name through the shell wrapper

```

$ ./Cartesian_unittest.sh --gtest_filter=FixedCartesian.ConstructFromString
# COORD_ORIGIN not set. Using ..
# GTEST_DIR not set. Using /usr/local/gtest-1.7.0
# DYLD_LIBRARY_PATH :../libCoords:/usr/local/gtest-1.7.0

Note: Google Test filter = FixedCartesian.ConstructFromString
[==========] Running 1 test from 1 test case.
[----------] Global test environment set-up.
[----------] 1 test from FixedCartesian
[ RUN      ] FixedCartesian.ConstructFromString
[       OK ] FixedCartesian.ConstructFromString (0 ms)
[----------] 1 test from FixedCartesian (0 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test case ran. (0 ms total)
[  PASSED  ] 1 test.


```

### [googletest](https://code.google.com/p/googletest/)

The C++ library uses [googletest](https://code.google.com/p/googletest/) to
run the unit tests. I have downloaded and built it in /usr/local using
following the instructions in the gtest README

```
[root gtest-1.7.0]# export GTEST_DIR=/usr/local/gtest-1.7.0
[root gtest-1.7.0]# g++ -isystem ${GTEST_DIR}/include -I${GTEST_DIR} -pthread -c ${GTEST_DIR}/src/gtest-all.cc

[root gtest-1.7.0]# ar -rv libgtest.a gtest-all.o
ar: creating archive libgtest.a
a - gtest-all.o
```

libCoords/Makefile sets its GTEST_DIR to /usr/local/gtest-1.7.0 and picks
up libgtest.a from there.


### To debug

With gdb (or lldb on OS X) using the gtest filter, use the
full method name (in this example Coords::rotator::rotate) to set
the break point and --gtest_filter with the GTEST name
(XRotationTest.Positive90AboutZ) to run that test.

```
[libCoords (spherical_atan2)]$ lldb Cartesian_unittest

(lldb) target create "Cartesian_unittest"
Current executable set to 'Cartesian_unittest' (x86_64).

(lldb) b Coords::rotator::rotate
Breakpoint 1: where = libCoords.1.dylib`Coords::rotator::rotate(Coords::Cartesian const&, Coords::angle const&) + 42 at Cartesian.cpp:235, address = 0x0000000000003f7a

(lldb) run --gtest_filter=RotationTest.FirstDiagonal_1
Process 31270 launched: '~/Coordinates/libCoords/Cartesian_unittest' (x86_64)
Note: Google Test filter = RotationTest.FirstDiagonal_1
[==========] Running 1 test from 1 test case.
[----------] Global test environment set-up.
[----------] 1 test from RotationTest
[ RUN      ] RotationTest.FirstDiagonal_1
Process 31270 stopped
* thread #1: tid = 0x212ea0, 0x000000010013df7a libCoords.1.dylib`Coords::rotator::rotate(this=0x00007fff5fbff348, a_heading=0x00007fff5fbff330, an_angle=0x00007fff5fbff3e8) + 42 at Cartesian.cpp:235, queue = 'com.apple.main-thread', stop reason = breakpoint 1.1
	frame #0: 0x000000010013df7a libCoords.1.dylib`Coords::rotator::rotate(this=0x00007fff5fbff348, a_heading=0x00007fff5fbff330, an_angle=0x00007fff5fbff3e8) + 42 at Cartesian.cpp:235
   232						  const Coords::angle& an_angle) {
   233
   234	  // Quaternion-derived rotation matrix
-> 235	  if (m_is_new_axis || m_current_angle != an_angle) {
   236
   237		double c(cos(an_angle.radians()));
   238		double s(sin(an_angle.radians()));

(lldb) p a_heading
(const Coords::Cartesian) $0 = (m_x = -1, m_y = -1, m_z = 1)

(lldb) c

Process 31270 resuming
[       OK ] RotationTest.FirstDiagonal_1 (38877 ms)
[----------] 1 test from RotationTest (38877 ms total)

[----------] Global test environment tear-down
[==========] 1 test from 1 test case ran. (38877 ms total)
[  PASSED  ] 1 test.
Process 31270 exited with status = 0 (0x00000000)

(lldb) ^D

```
