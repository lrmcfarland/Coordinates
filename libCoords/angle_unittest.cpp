// ================================================================
// Filename:    angle_unittest.cpp
// Description: This is the gtest unittest of the angle library.
//
// Author:      L.R. McFarland, lrm@starbug.com
// Created:     2014 Jun 19
// Language:    C++
//
//  Angle is free software: you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Angle is distributed in the hope that it will be useful, but
//  WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Orbits.  If not, see <http://www.gnu.org/licenses/>.
// ================================================================

#include <sstream>

#include <gtest/gtest.h>

#include <angle.h>

namespace {


  // -----------------
  // ----- Angle -----
  // -----------------

  // ----- static methods -----

  TEST(Angle, Degree2Radians) {
    double a = Coords::Angle::deg2rad(45);
    EXPECT_DOUBLE_EQ(0.78539816339744828, a);
  }

  TEST(Angle, Radian2Degrees) {
    double a = Coords::Angle::rad2deg(0.78539816339744828);
    EXPECT_DOUBLE_EQ(45, a);
  }

  // constructors (and implicitly radians() accessor)

  TEST(Angle, CopyConstructor) {
    Coords::Angle a;
    a.value(1);
    Coords::Angle b(a);
    EXPECT_TRUE(a == b);
  }

  TEST(Angle, CopyAssign) {
    Coords::Angle a;
    a.value(1);
    Coords::Angle b = a;
    EXPECT_TRUE(a == b);
  }

  TEST(Angle, DefaultConstructor) {
    Coords::Angle a;
    EXPECT_EQ(0, a.radians());
  }


  TEST(Angle, ConstructorDeg) {
    Coords::Angle a(-45);
    EXPECT_EQ(Coords::Angle::deg2rad(-45), a.radians());
  }

  TEST(Angle, ConstructorDegFromStr) {
    Coords::Angle a("-45");
    EXPECT_EQ(Coords::Angle::deg2rad(-45), a.radians());
  }

  TEST(Angle, ConstructorDegMin) {
    Coords::Angle a(0, -60);
    EXPECT_EQ(Coords::Angle::deg2rad(-1), a.radians());
  }

  TEST(Angle, ConstructorDegMinFromStr) {
    Coords::Angle a("0", "-60");
    EXPECT_EQ(Coords::Angle::deg2rad(-1), a.radians());
  }

  TEST(Angle, ConstructorDegMinSec) {
    Coords::Angle a(0, 0, -1);
    EXPECT_DOUBLE_EQ(Coords::Angle::deg2rad(-1/3600.0), a.radians());
  }

  TEST(Angle, ConstructorDegMinSecFromStr) {
    Coords::Angle a("0", "0", "-6.1");
    EXPECT_DOUBLE_EQ(Coords::Angle::deg2rad(-6.1/3600.0), a.radians());
  }

  // TODO correct behavior?
  TEST(Angle, MixedSignX) {
    Coords::Angle a(-1, 2);
    Coords::Angle b(1, -2);
    EXPECT_FALSE(a.value() == b.value());
  }

  TEST(Angle, MixedSign1) {
    Coords::Angle a(-1, 2);
    Coords::Angle b(-1, -2);
    EXPECT_DOUBLE_EQ(a.value(), b.value());
  }

  TEST(Angle, MixedSign2) {
    Coords::Angle a(-1, 2, 3);
    Coords::Angle b(-1, -2, -3);
    EXPECT_DOUBLE_EQ(a.value(), b.value());
  }

  TEST(Angle, MixedSign3) {
    Coords::Angle a(1, 2, 3);
    Coords::Angle b(1, -2, -3);
    EXPECT_DOUBLE_EQ(a.value(), b.value());
  }

  // accessors

  TEST(Angle, Accessors) {
    Coords::Angle a(-45);
    EXPECT_EQ(-45, a.value());
    EXPECT_EQ(-45, a.getValue());
    EXPECT_EQ(Coords::Angle::deg2rad(-45), a.radians());
    EXPECT_EQ(Coords::Angle::deg2rad(-45), a.getRadians());
  }

  // ----- booleans -----

  TEST(Angle, Equivalence1) {
    Coords::Angle a;
    Coords::Angle b;
    a.value(1);
    b.value(1);
    EXPECT_TRUE(a == b);
  }

  TEST(Angle, Equivalence2) {
    Coords::Angle a;
    Coords::Angle b;
    a.value(1);
    b.value(-1);
    EXPECT_FALSE(a == b);
  }

  TEST(Angle, NotEquivalence1) {
    Coords::Angle a(1);
    Coords::Angle b(1);
    EXPECT_FALSE(a != b);
  }

  TEST(Angle, NotEquivalence2) {
    Coords::Angle a(1);
    Coords::Angle b(-1);
    EXPECT_TRUE(a != b);
  }

  TEST(Angle, LessThan1) {
    Coords::Angle a(10);
    Coords::Angle b(20);
    EXPECT_TRUE(a < b);
  }

  TEST(Angle, LessThan2) {
    Coords::Angle a(10);
    Coords::Angle b(20);
    EXPECT_FALSE(b < a);
  }

  TEST(Angle, LessThanOrEqualTo1) {
    Coords::Angle a(25.1);
    Coords::Angle b(25.1);
    EXPECT_TRUE(a <= b);
  }

  TEST(Angle, LessThanOrEqualTo2) {
    Coords::Angle a(10.5);
    Coords::Angle b(10.6);
    EXPECT_FALSE(b <= a);
  }

  TEST(Angle, GreaterThan1) {
    Coords::Angle a(20);
    Coords::Angle b(10);
    EXPECT_TRUE(a > b);
  }

  TEST(Angle, GreaterThan2) {
    Coords::Angle a(20);
    Coords::Angle b(10);
    EXPECT_FALSE(b > a);
  }

  TEST(Angle, GreaterThanOrEqualTo1) {
    Coords::Angle a(25.1);
    Coords::Angle b(25.1);
    EXPECT_TRUE(a >= b);
  }

  TEST(Angle, GreaterThanOrEqualTo2) {
    Coords::Angle a(10.6);
    Coords::Angle b(10.5);
    EXPECT_FALSE(b >= a);
  }

  // operators

  // add
  TEST(Angle, InplaceAddAngle) {
    Coords::Angle a(45);
    Coords::Angle b(45);
    a += b;
    EXPECT_DOUBLE_EQ(90, a.value());
  }

  TEST(Angle, InplaceAddAngleNeg) {
    Coords::Angle a(45);
    Coords::Angle b(-45);
    a += b;
    EXPECT_DOUBLE_EQ(0, a.value());
  }

  TEST(Angle, AnglePlusAnglePos) {
    Coords::Angle a(44.5);
    Coords::Angle b(44.5);
    Coords::Angle c;
    c = a + b;
    EXPECT_DOUBLE_EQ(89, c.value());
  }

  TEST(Angle, AnglePlusAngleNeg) {
    Coords::Angle a(45);
    Coords::Angle b(-45);
    Coords::Angle c;
    c = a + b;
    EXPECT_DOUBLE_EQ(0, c.value());
  }

  // subtract
  TEST(Angle, InplaceSubtractAnglePos) {
    Coords::Angle a(45);
    Coords::Angle b(40);
    a -= b;
    EXPECT_DOUBLE_EQ(5, a.value());
  }

  TEST(Angle, InplaceSubtractAngleNeg) {
    Coords::Angle a(45);
    Coords::Angle b(-45);
    a -= b;
    EXPECT_DOUBLE_EQ(90, a.value());
  }

  TEST(Angle, AngleMinusAnglePos) {
    Coords::Angle a(45);
    Coords::Angle b(45);
    Coords::Angle c;
    c = a - b;
    EXPECT_DOUBLE_EQ(0, c.value());
  }

  TEST(Angle, AngleMinusAngleNeg) {
    Coords::Angle a(45);
    Coords::Angle b(-45);
    Coords::Angle c;
    c = a - b;
    EXPECT_DOUBLE_EQ(90, c.value());
  }

  // unitary minus
  TEST(Angle, UnitaryMinus) {
    Coords::Angle a;
    Coords::Angle b(-45);
    a = -b;
    EXPECT_DOUBLE_EQ(45, a.value());
  }

  // multiply

  TEST(Angle, InplaceMultiply) {
    Coords::Angle a(45);
    Coords::Angle b(2);
    a *= b;
    EXPECT_DOUBLE_EQ(90, a.value());
  }

  TEST(Angle, MultiplyAngleByAngle) {
    Coords::Angle a(45);
    Coords::Angle b(2);
    Coords::Angle c;
    c = a * b;
    EXPECT_DOUBLE_EQ(90, c.value());
  }

  // divide

  TEST(Angle, InplaceDivide) {
    Coords::Angle a(90);
    Coords::Angle b(2);
    a /= b;
    EXPECT_DOUBLE_EQ(45, a.value());
  }

  TEST(Angle, InplaceDivideByZeroError) {
    Coords::Angle a(45);
    Coords::Angle b(0);
    EXPECT_THROW(a /= b, Coords::DivideByZeroError);
  }

  TEST(Angle, InplaceDivideByZeroErrorMsg) {
    try {
      Coords::Angle a(15);
      Coords::Angle b;
      a /= b;
    } catch (Coords::DivideByZeroError& err) {
      EXPECT_STREQ(err.what(), "division by zero is undefined");
    }
  }

  TEST(Angle, DivideAngleByAngle) {
    Coords::Angle a(90);
    Coords::Angle b(2);
    Coords::Angle c;
    c = a / b;
    EXPECT_DOUBLE_EQ(45, c.value());
  }


  // operator<<

  TEST(Angle, operatorStdOut) {
    Coords::Angle a(44, 32, 15.4);
    std::stringstream out;
    out << a;
    EXPECT_STREQ("44* 32' 15.4\"", out.str().c_str());
  }


  // 360 and beyond

  TEST(Angle, Stdout360) {
    Coords::Angle a(360);
    std::stringstream out;
    out << a;
    EXPECT_STREQ("360* 0' 0\"", out.str().c_str());
  }

  TEST(Angle, Stdout361) {
    Coords::Angle a(361);
    std::stringstream out;
    out << a;
    EXPECT_STREQ("361* 0' 0\"", out.str().c_str());
  }

  TEST(Angle, RoundingIssuesBeyond360) {
    // had rounding issues when storing value in radians.
    Coords::Angle a(45+360);
    std::stringstream out;
    out << a;
    EXPECT_STREQ("405* 0' 0\"", out.str().c_str());
  }

  // TODO fix this Angle(45 * 10).normalize() != 180
  TEST(Angle, Normalize) {
    Coords::Angle a(45 + 360);
    a.normalize();
    EXPECT_NEAR(45, a.value(), 1e-15);
  }


  // -----------------
  // ----- Utils -----
  // -----------------

  // utils degrees2seconds sign syntax errors
  TEST(Utils, Degree2SecondsError1) {
    EXPECT_TRUE(Coords::degrees2seconds(45, 60, 0) == Coords::degrees2seconds(45, -60, 0));
  }

  TEST(Utils, Degree2SecondsError2) {
    EXPECT_TRUE(Coords::degrees2seconds(45, 59, 60) == Coords::degrees2seconds(45, 59, -60));
  }

  TEST(Utils, Degree2SecondsError3) {
    EXPECT_TRUE(Coords::degrees2seconds(0, 59, 60) == Coords::degrees2seconds(0, 59, -60));
  }

  TEST(Utils, Degree2SecondsError4) {
    EXPECT_TRUE(Coords::degrees2seconds(0, -59, 60) == Coords::degrees2seconds(0, -59, -60));
  }

  TEST(Utils, Degree2SecondsError5) {
    EXPECT_TRUE(Coords::degrees2seconds(-45, 59, 60) == Coords::degrees2seconds(-45, -59, 60));
  }

  TEST(Utils, Degree2SecondsError6) {
    EXPECT_TRUE(Coords::degrees2seconds(-45, 59, 60) == Coords::degrees2seconds(-45, 59, -60));
  }

  TEST(Utils, Degree2SecondsError7) {
    EXPECT_TRUE(Coords::degrees2seconds(-45, 59, 60) == Coords::degrees2seconds(-45, -59, -60));
  }



  // opeartor<<()

  TEST(Angle, output_dms) {
    Coords::Angle a(12, 34, 56);

    std::stringstream out;
    Coords::value2DMSString(a.value(), out);

    EXPECT_STREQ("12* 34' 56\"", out.str().c_str());
  }

  TEST(Angle, output_hms) {
    Coords::Angle a(2, 4, 6);

    std::stringstream out;
    Coords::value2HMSString(a.value(), out);

    EXPECT_STREQ("02:04:06", out.str().c_str());
  }


  // TODO


} // end anonymous namespace



// ==================
// ===== main() =====
// ==================

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
