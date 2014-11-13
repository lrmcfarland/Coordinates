# Coordinates library

To debug with gdb (or lldb on OS X) using the gtest filter, use the
full method name (in this example Coords::rotator::rotate) to set
the break point and --gtest_filter with the GTEST name
(XRotationTest.Positive90AboutZ) to only run that test.

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
   232 						  const Coords::angle& an_angle) {
   233 	
   234 	  // Quaternion-derived rotation matrix
-> 235 	  if (m_is_new_axis || m_current_angle != an_angle) {
   236 	
   237 	    double c(cos(an_angle.radians()));
   238 	    double s(sin(an_angle.radians()));
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