// ================================================================
// Filename:    spherical_unittest.cpp
// Description: This is the gtest unittest of the spherical library.
//
// Author:      L.R. McFarland, lrm@starbug.com
// Created:     2012 May 13
// Language:    C++
//
// See also: http://code.google.com/p/googletest/wiki/Primer
//
// To compile manually:
// g++ -I /usr/local/gtest-1.6.0/include -I . -c spherical_unittest.cpp
// g++ -L /usr/local/gtest-1.6.0 -lgtest spherical_unittest.o -o spherical_unittest
//
//  Orbits is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Orbits is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Orbits.  If not, see <http://www.gnu.org/licenses/>.
// ================================================================


#include <chrono>
#include <random>
#include <sstream>

#include <gtest/gtest.h>

#include <angle.h>
#include <Cartesian.h>
#include <spherical.h>


namespace {

  // -----------------------
  // ----- Fixed Space -----
  // -----------------------

  TEST(FixedSpherical, Accessors) {

    Coords::spherical a;

    a.r(1.1);
    EXPECT_EQ(1.1, a.r());
    EXPECT_EQ(1.1, a.getR());

    a.theta(Coords::angle(-2.2));
    EXPECT_EQ(-2.2, a.theta().value());
    EXPECT_EQ(-2.2, a.getTheta().value());

    a.phi(Coords::angle(3.3));
    EXPECT_EQ(3.3, a.phi().value());
    EXPECT_EQ(3.3, a.getPhi().value());

  }

  TEST(FixedSpace, OutputOperator) {
    Coords::spherical a(1, Coords::angle(2), Coords::angle(3));
    std::stringstream out;
    out << a;
    EXPECT_STREQ("<spherical><r>1</r><theta>2</theta><phi>3</phi></spherical>", out.str().c_str());
  }

  TEST(FixedSpherical, Equivalence) {
    EXPECT_TRUE(Coords::spherical(1, Coords::angle(2), Coords::angle(3)) ==
		Coords::spherical(1.0, Coords::angle(2.0), Coords::angle(3.0)));

    EXPECT_FALSE(Coords::spherical(1, Coords::angle(2), Coords::angle(3)) ==
		 Coords::spherical(1.1, Coords::angle(2.0), Coords::angle(3.0)));
    EXPECT_FALSE(Coords::spherical(1, Coords::angle(2), Coords::angle(3)) ==
		 Coords::spherical(1.0, Coords::angle(2.1), Coords::angle(3.0)));
    EXPECT_FALSE(Coords::spherical(1, Coords::angle(2), Coords::angle(3)) ==
		 Coords::spherical(1.0, Coords::angle(2.0), Coords::angle(3.1)));

    EXPECT_TRUE(Coords::spherical(1, Coords::angle(2), Coords::angle(3)) !=
		Coords::spherical(1.1, Coords::angle(2.0), Coords::angle(3.0)));
    EXPECT_TRUE(Coords::spherical(1, Coords::angle(2), Coords::angle(3)) !=
		Coords::spherical(1.0, Coords::angle(2.1), Coords::angle(3.0)));
    EXPECT_TRUE(Coords::spherical(1, Coords::angle(2), Coords::angle(3)) !=
		Coords::spherical(1.0, Coords::angle(2.0), Coords::angle(3.1)));
  }

  TEST(FixedSpherical, DefaultConstructor) {
    Coords::spherical a;
    EXPECT_DOUBLE_EQ(0, a.r());
    EXPECT_EQ(Coords::angle(0), a.theta());
    EXPECT_EQ(Coords::angle(0), a.phi());
  }

  TEST(FixedSpherical, ConstructFromString) {

    Coords::spherical a("1.23");
    EXPECT_DOUBLE_EQ(1.23, a.r());
    EXPECT_DOUBLE_EQ(0, a.theta().value());
    EXPECT_DOUBLE_EQ(0.0, a.phi().value());

    Coords::spherical b("-1.23", "1.23e-7");
    EXPECT_DOUBLE_EQ(-1.23, b.r());
    EXPECT_DOUBLE_EQ(1.23e-7, b.theta().value());
    EXPECT_DOUBLE_EQ(0.0, b.phi().value());

    Coords::spherical c("1.23", "-1.23e-7", "10");
    EXPECT_DOUBLE_EQ(1.23, c.r());
    EXPECT_DOUBLE_EQ(-1.23e-7, c.theta().value());
    EXPECT_DOUBLE_EQ(10.0, c.phi().value());

  }

  TEST(FixedSpherical, ConstructFromBadString) {

    Coords::spherical def; // default

    // TODO in future, expect throw std::invalid_argument from std::stod.
    Coords::spherical d("asdf");

    EXPECT_EQ(def, d);

    Coords::spherical e("");
    EXPECT_EQ(def, e);

    Coords::spherical f("asdf 3.1415 blah");
    EXPECT_EQ(def, f);

  }



  TEST(FixedSpherical, ConstructFromCartesian_x) {
    // all x
    Coords::Cartesian a(1);
    Coords::spherical b(a);

    EXPECT_DOUBLE_EQ(1.0, b.r());
    EXPECT_EQ(Coords::angle(90.0), b.theta());
    EXPECT_EQ(Coords::angle(0.0), b.phi());
  }

  TEST(FixedSpherical, ConstructFromCartesian_y) {
    // all y
    Coords::Cartesian a(0, 1);
    Coords::spherical b(a);

    EXPECT_DOUBLE_EQ(1.0, b.r());

    EXPECT_DOUBLE_EQ(90.0, b.theta().value());
    EXPECT_EQ(Coords::angle(90.0), b.theta());

    EXPECT_DOUBLE_EQ(0.0, b.phi().value());
    EXPECT_EQ(Coords::angle(0.0), b.phi());
  }

  TEST(FixedSpherical, ConstructFromCartesian_z) {
    // all z
    Coords::Cartesian a(0, 0, 1);
    Coords::spherical b(a);

    EXPECT_DOUBLE_EQ(1.0, b.r());
    EXPECT_EQ(Coords::angle(0.0), b.theta());
    EXPECT_EQ(Coords::angle(0.0), b.phi());
  }




  TEST(FixedSpherical, Add_inplace_trivial_1) {
    Coords::spherical a(1, Coords::angle(30), Coords::angle(60));
    Coords::spherical b(1, Coords::angle(30), Coords::angle(60));

    a += b;
    EXPECT_DOUBLE_EQ(2.0, a.r());
    EXPECT_DOUBLE_EQ(Coords::angle(30.0).value(), a.theta().value());
    EXPECT_DOUBLE_EQ(Coords::angle(60.0).value(), a.phi().value());
  }


  TEST(FixedSpherical, Add_trivial_1) {
    Coords::spherical a(1, Coords::angle(30), Coords::angle(60));
    Coords::spherical b(1, Coords::angle(30), Coords::angle(60));
    Coords::spherical c;
    c = a + b;
    EXPECT_DOUBLE_EQ(2.0, c.r());
    EXPECT_DOUBLE_EQ(Coords::angle(30.0).value(), c.theta().value());
    EXPECT_DOUBLE_EQ(Coords::angle(60.0).value(), c.phi().value());
  }

  TEST(FixedSpherical, Add_1) {
    Coords::spherical a(1, Coords::angle(45), Coords::angle(45));
    Coords::spherical b(1, Coords::angle(45), Coords::angle(-45));
    Coords::spherical c;
    c = a + b;

    // a and b are both sin(45) high along z axis
    // +0.5 along x axis (1.0 total)
    // and +/- 0.5 along the y axis (0.0 total)

    double c_z = 2.0*sin(Coords::angle::deg2rad(45));

    // r is hypotenuse, z total/asin(z total/x total):
    EXPECT_DOUBLE_EQ(c_z/sin(atan(c_z/1.0)), c.r());

    // theta, the angle to the z-axis, is:
    EXPECT_DOUBLE_EQ(Coords::angle(90).value() - Coords::angle::rad2deg(atan(c_z/1.0)),
		     c.theta().value());

    EXPECT_DOUBLE_EQ(Coords::angle(0.0).value(), c.phi().value());

    Coords::Cartesian cart_c(c);
    EXPECT_DOUBLE_EQ(1.0, cart_c.x()); // 0.5 + 0.5 projection in xy plane
    EXPECT_DOUBLE_EQ(0.0, cart_c.y()); // 0.5 - 0.5 projection in xy plane
    EXPECT_NEAR(c_z, cart_c.z(), Coords::epsilon*10);

  }

  // TODO more non-trivial add tests


  TEST(FixedSpherical, Subtract_inplace_trivial_1) {
    Coords::spherical a(1, Coords::angle(30), Coords::angle(60));
    Coords::spherical b(1, Coords::angle(30), Coords::angle(60));
    a -= b;
    EXPECT_DOUBLE_EQ(0.0, a.r());
    EXPECT_DOUBLE_EQ(Coords::angle(0.0).value(), a.theta().value());
    EXPECT_DOUBLE_EQ(Coords::angle(0.0).value(), a.phi().value());
  }

  TEST(FixedSpherical, Subtract_inplace_trivial_2) {
    Coords::spherical a(10, Coords::angle(30), Coords::angle(-60));
    Coords::spherical b(1, Coords::angle(30), Coords::angle(-60));
    a -= b;
    EXPECT_DOUBLE_EQ(9.0, a.r());
    EXPECT_DOUBLE_EQ(Coords::angle(30.0).value(), a.theta().value());
    EXPECT_DOUBLE_EQ(Coords::angle(-60.0).value(), a.phi().value());
  }


  TEST(FixedSpherical, Subtract_trivial_1) {
    Coords::spherical a(10, Coords::angle(30), Coords::angle(60));
    Coords::spherical b(1, Coords::angle(30), Coords::angle(60));
    Coords::spherical c;
    c = a - b;
    EXPECT_DOUBLE_EQ(9.0, c.r());
    EXPECT_DOUBLE_EQ(Coords::angle(30.0).value(), c.theta().value());
    EXPECT_DOUBLE_EQ(Coords::angle(60.0).value(), c.phi().value());
  }


  TEST(FixedSpherical, Subtract_1) {
    Coords::spherical a(1, Coords::angle(45), Coords::angle(45));
    Coords::spherical b(1, Coords::angle(45), Coords::angle(-45));
    Coords::spherical c;
    c = a - b;
    EXPECT_DOUBLE_EQ(1.0, c.r());
    EXPECT_DOUBLE_EQ(Coords::angle(90.0).value(), c.theta().value());
    EXPECT_DOUBLE_EQ(Coords::angle(0.0).value(), c.phi().value());

    Coords::Cartesian cart_c(c);
    EXPECT_DOUBLE_EQ(1.0, cart_c.x());
    EXPECT_DOUBLE_EQ(0.0, cart_c.y());
    EXPECT_NEAR(0.0, cart_c.z(), Coords::epsilon);

  }


  // TODO more non-trivial subtract tests


  TEST(FixedSpherical, Multiply_inplace_1) {
    Coords::spherical a(1, Coords::angle(30), Coords::angle(60));
    a *= 10;
    EXPECT_DOUBLE_EQ(10.0, a.r());
    EXPECT_EQ(Coords::angle(30.0), a.theta());
    EXPECT_EQ(Coords::angle(60.0), a.phi());
  }


  TEST(FixedSpherical, Multiply_by_double_1) {
    Coords::spherical a(1, Coords::angle(30), Coords::angle(60));
    Coords::spherical b;
    b = a * 10;
    EXPECT_DOUBLE_EQ(10.0, b.r());
    EXPECT_EQ(Coords::angle(30.0), b.theta());
    EXPECT_EQ(Coords::angle(60.0), b.phi());
  }

  TEST(FixedSpherical, Multiply_by_double_2) {
    Coords::spherical a(1, Coords::angle(30), Coords::angle(-60));
    Coords::spherical b;
    b = 10 * a; // commutes
    EXPECT_DOUBLE_EQ(10.0, b.r());
    EXPECT_EQ(Coords::angle(30.0), b.theta());
    EXPECT_EQ(Coords::angle(-60.0), b.phi());
  }


  TEST(FixedSpherical, Divide_inplace_1) {
    Coords::spherical a(1, Coords::angle(30), Coords::angle(60));
    a /= 10;
    EXPECT_DOUBLE_EQ(0.1, a.r());
    EXPECT_EQ(Coords::angle(30.0), a.theta());
    EXPECT_EQ(Coords::angle(60.0), a.phi());
  }

  TEST(FixedSpherical, Divide_by_double_1) {
    Coords::spherical a(1, Coords::angle(30), Coords::angle(-60));
    Coords::spherical b;
    b = a/10; // commutes
    EXPECT_DOUBLE_EQ(0.1, b.r());
    EXPECT_EQ(Coords::angle(30.0), b.theta());
    EXPECT_EQ(Coords::angle(-60.0), b.phi());
  }

  TEST(FixedSpherical, Divide_by_double_2) {
    Coords::spherical a(10.0, Coords::angle(-30), Coords::angle(-60));
    Coords::spherical b;
    b = 1/a; // commutes
    EXPECT_DOUBLE_EQ(0.1, b.r());
    EXPECT_EQ(Coords::angle(-30.0), b.theta());
    EXPECT_EQ(Coords::angle(-60.0), b.phi());
  }

  TEST(FixedSpherical, InplaceDivideByZeroException) {
    try {
      Coords::spherical a; a /= 0;
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "division by zero is undefined");
    }
  }




} // end anonymous namespace


// ==================
// ===== main() =====
// ==================

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
