// ================================================================
// Filename:    angle_unittest.cpp
// Description: This is the gtest unittest of the angle library.
//
// Author:      L.R. McFarland, lrm@starbug.com
// Created:     2014 Jun 19
// Language:    C++
//
//  angle is free software: you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  angle is distributed in the hope that it will be useful, but
//  WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Coordinates.  If not, see <http://www.gnu.org/licenses/>.
// ================================================================

#include <sstream>

#include <gtest/gtest.h>

#include <angle.h>

namespace {


  // -----------------
  // ----- angle -----
  // -----------------

  // ----- static methods -----

  TEST(angle, Degree2Radians45) {
    double a = Coords::angle::deg2rad(45);
    EXPECT_DOUBLE_EQ(0.78539816339744828, a);
  }

  TEST(angle, Radian2Degrees45) {
    double a = Coords::angle::rad2deg(0.78539816339744828);
    EXPECT_DOUBLE_EQ(45, a);
  }



  // Right Ascension

  TEST(angle, Degree2RightAscension90) {
    double a = Coords::angle::deg2RA(90);
    EXPECT_DOUBLE_EQ(6.0, a);
  }

  TEST(angle, Degree2RightAscension_n90) {
    double a = Coords::angle::deg2RA(-90);
    EXPECT_DOUBLE_EQ(18.0, a);
  }

  TEST(angle, Degree2RightAscension360) {
    double a = Coords::angle::deg2RA(360);
    EXPECT_DOUBLE_EQ(0, a);
  }

  // TODO 24.333?
  TEST(angle, Degree2RightAscension365) {
    double a = Coords::angle::deg2RA(365);
    EXPECT_DOUBLE_EQ(0.33333333333333331, a);
  }



  TEST(angle, RightAscension2Degrees6) {
    double a = Coords::angle::RA2deg(6);
    EXPECT_DOUBLE_EQ(90, a);
  }

  TEST(angle, RightAscension2Degrees12) {
    double a = Coords::angle::RA2deg(12);
    EXPECT_DOUBLE_EQ(-180, a);
  }

  TEST(angle, RightAscension2Degrees18) {
    double a = Coords::angle::RA2deg(18);
    EXPECT_DOUBLE_EQ(-90, a);
  }

  TEST(angle, RightAscension2Degrees24) {
    double a = Coords::angle::RA2deg(24);
    EXPECT_DOUBLE_EQ(0.0, a);
  }





  // constructors (and implicitly radians() accessor)

  TEST(angle, CopyConstructor) {
    Coords::angle a;
    a.degrees(1);
    Coords::angle b(a);
    EXPECT_TRUE(a == b);
  }

  TEST(angle, CopyAssign) {
    Coords::angle a;
    a.degrees(1);
    Coords::angle b = a;
    EXPECT_TRUE(a == b);
  }

  TEST(angle, DefaultConstructorRadians) {
    Coords::angle a;
    EXPECT_EQ(0, a.radians());
  }

  TEST(angle, DefaultConstructorRA) {
    Coords::angle a;
    EXPECT_EQ(0, a.RA());
  }

  TEST(angle, Constructor1) {
    Coords::angle a(-45);
    EXPECT_EQ(Coords::angle::deg2rad(-45), a.radians());
  }

  TEST(angle, Constructor2) {
    Coords::angle a(-30);
    EXPECT_EQ(Coords::angle::deg2RA(-30), a.RA());
  }

  TEST(angle, ConstructorDegFromStr) {
    Coords::angle a("-45");
    EXPECT_EQ(Coords::angle::deg2rad(-45), a.radians());
  }

  TEST(angle, ConstructorDegMin) {
    Coords::angle a(0, -60);
    EXPECT_EQ(Coords::angle::deg2rad(-1), a.radians());
  }

  TEST(angle, ConstructorDegMinFromStr) {
    Coords::angle a("0", "-60");
    EXPECT_EQ(Coords::angle::deg2rad(-1), a.radians());
  }

  TEST(angle, ConstructorDegMinSec) {
    Coords::angle a(0, 0, -1);
    EXPECT_DOUBLE_EQ(Coords::angle::deg2rad(-1/3600.0), a.radians());
  }

  TEST(angle, ConstructorDegMinSecFromStr) {
    Coords::angle a("0", "0", "-6.1");
    EXPECT_DOUBLE_EQ(Coords::angle::deg2rad(-6.1/3600.0), a.radians());
  }

  // TODO correct behavior?
  TEST(angle, MixedSignX) {
    Coords::angle a(-1, 2);
    Coords::angle b(1, -2);
    EXPECT_FALSE(a.degrees() == b.degrees());
  }

  TEST(angle, MixedSign1) {
    Coords::angle a(-1, 2);
    Coords::angle b(-1, -2);
    EXPECT_DOUBLE_EQ(a.degrees(), b.degrees());
  }

  TEST(angle, MixedSign2) {
    Coords::angle a(-1, 2, 3);
    Coords::angle b(-1, -2, -3);
    EXPECT_DOUBLE_EQ(a.degrees(), b.degrees());
  }

  TEST(angle, MixedSign3) {
    Coords::angle a(1, 2, 3);
    Coords::angle b(1, -2, -3);
    EXPECT_DOUBLE_EQ(a.degrees(), b.degrees());
  }

  // accessors

  TEST(angle, Accessors) {
    Coords::angle a(-45);
    EXPECT_EQ(-45, a.degrees());
    EXPECT_EQ(-45, a.getDegrees());
    EXPECT_EQ(Coords::angle::deg2rad(-45), a.radians());
    EXPECT_EQ(Coords::angle::deg2rad(-45), a.getRadians());
    EXPECT_EQ(Coords::angle::deg2RA(-45),  a.RA());
    EXPECT_EQ(Coords::angle::deg2RA(-45),  a.getRA());
  }

  // ----- booleans -----

  TEST(angle, Equivalence1) {
    Coords::angle a;
    Coords::angle b;
    a.degrees(1);
    b.degrees(1);
    EXPECT_TRUE(a == b);
  }

  TEST(angle, Equivalence2) {
    Coords::angle a;
    Coords::angle b;
    a.degrees(1);
    b.degrees(-1);
    EXPECT_FALSE(a == b);
  }

  TEST(angle, NotEquivalence1) {
    Coords::angle a(1);
    Coords::angle b(1);
    EXPECT_FALSE(a != b);
  }

  TEST(angle, NotEquivalence2) {
    Coords::angle a(1);
    Coords::angle b(-1);
    EXPECT_TRUE(a != b);
  }

  TEST(angle, LessThan1) {
    Coords::angle a(10);
    Coords::angle b(20);
    EXPECT_TRUE(a < b);
  }

  TEST(angle, LessThan2) {
    Coords::angle a(10);
    Coords::angle b(20);
    EXPECT_FALSE(b < a);
  }

  TEST(angle, LessThanOrEqualTo1) {
    Coords::angle a(25.1);
    Coords::angle b(25.1);
    EXPECT_TRUE(a <= b);
  }

  TEST(angle, LessThanOrEqualTo2) {
    Coords::angle a(10.5);
    Coords::angle b(10.6);
    EXPECT_FALSE(b <= a);
  }

  TEST(angle, GreaterThan1) {
    Coords::angle a(20);
    Coords::angle b(10);
    EXPECT_TRUE(a > b);
  }

  TEST(angle, GreaterThan2) {
    Coords::angle a(20);
    Coords::angle b(10);
    EXPECT_FALSE(b > a);
  }

  TEST(angle, GreaterThanOrEqualTo1) {
    Coords::angle a(25.1);
    Coords::angle b(25.1);
    EXPECT_TRUE(a >= b);
  }

  TEST(angle, GreaterThanOrEqualTo2) {
    Coords::angle a(10.6);
    Coords::angle b(10.5);
    EXPECT_FALSE(b >= a);
  }

  // operators

  // add
  TEST(angle, InplaceAddAngle) {
    Coords::angle a(45);
    Coords::angle b(45);
    a += b;
    EXPECT_DOUBLE_EQ(90, a.degrees());
  }

  TEST(angle, InplaceAddAngleNeg) {
    Coords::angle a(45);
    Coords::angle b(-45);
    a += b;
    EXPECT_DOUBLE_EQ(0, a.degrees());
  }

  TEST(angle, InplaceAddDouble) {
    Coords::angle a(45);
    double b(45);
    a += b;
    EXPECT_DOUBLE_EQ(90, a.degrees());
  }


  TEST(angle, AnglePlusAnglePos) {
    Coords::angle a(44.5);
    Coords::angle b(44.5);
    Coords::angle c;
    c = a + b;
    EXPECT_DOUBLE_EQ(89, c.degrees());
  }

  TEST(angle, AnglePlusAngleNeg) {
    Coords::angle a(45);
    Coords::angle b(-45);
    Coords::angle c;
    c = a + b;
    EXPECT_DOUBLE_EQ(0, c.degrees());
  }

  TEST(angle, AnglePlusDouble) {
    Coords::angle a(30);
    double b(60);
    Coords::angle c;
    c = a + b;
    EXPECT_DOUBLE_EQ(90, c.degrees());
  }

  TEST(angle, DoublePlusAngle) {
    Coords::angle a(29);
    double b(61);
    Coords::angle c;
    c = b + a;
    EXPECT_DOUBLE_EQ(90, c.degrees());
  }


  // subtract
  TEST(angle, InplaceSubtractAnglePos) {
    Coords::angle a(45);
    Coords::angle b(40);
    a -= b;
    EXPECT_DOUBLE_EQ(5, a.degrees());
  }

  TEST(angle, InplaceSubtractAngleNeg) {
    Coords::angle a(45);
    Coords::angle b(-45);
    a -= b;
    EXPECT_DOUBLE_EQ(90, a.degrees());
  }

  TEST(angle, InplaceSubtractDouble) {
    Coords::angle a(40);
    double b(-45);
    a -= b;
    EXPECT_DOUBLE_EQ(85, a.degrees());
  }


  TEST(angle, AngleMinusAnglePos) {
    Coords::angle a(45);
    Coords::angle b(45);
    Coords::angle c;
    c = a - b;
    EXPECT_DOUBLE_EQ(0, c.degrees());
  }

  TEST(angle, AngleMinusAngleNeg) {
    Coords::angle a(45);
    Coords::angle b(-45);
    Coords::angle c;
    c = a - b;
    EXPECT_DOUBLE_EQ(90, c.degrees());
  }


  TEST(angle, AngleMinusDouble) {
    Coords::angle a(45);
    double b(40);
    Coords::angle c;
    c = a - b;
    EXPECT_DOUBLE_EQ(5, c.degrees());
  }

  TEST(angle, DoubleMinusAngle) {
    Coords::angle a(45);
    double b(40);
    Coords::angle c;
    c = b - a;
    EXPECT_DOUBLE_EQ(-5, c.degrees());
  }


  // unary minus
  TEST(angle, UnaryMinus) {
    Coords::angle a;
    Coords::angle b(-45);
    a = -b;
    EXPECT_DOUBLE_EQ(45, a.degrees());
  }

  // multiply

  TEST(angle, InplaceMultiplyByAngle) {
    Coords::angle a(45);
    Coords::angle b(2);
    a *= b;
    EXPECT_DOUBLE_EQ(90, a.degrees());
  }

  TEST(angle, InplaceMultiplyByDouble) {
    Coords::angle a(45);
    double b(2);
    a *= b;
    EXPECT_DOUBLE_EQ(90, a.degrees());
  }

  TEST(angle, MultiplyAngleByAngle) {
    Coords::angle a(45);
    Coords::angle b(2);
    Coords::angle c;
    c = a * b;
    EXPECT_DOUBLE_EQ(90, c.degrees());
  }

  TEST(angle, MultiplyAngleByDouble) {
    Coords::angle a(33);
    double b(2.5);
    Coords::angle c;
    c = a * b;
    EXPECT_DOUBLE_EQ(82.5, c.degrees());
  }

  TEST(angle, MultiplyAngleByInt) {
    Coords::angle a(33);
    int b(2);
    Coords::angle c;
    c = a * b;
    EXPECT_DOUBLE_EQ(66, c.degrees());
  }

  TEST(angle, MultiplyIntByAngle) {
    Coords::angle a(20.5);
    int b(2);
    Coords::angle c;
    c = b * a;
    EXPECT_DOUBLE_EQ(41, c.degrees());
  }


  TEST(angle, MultiplyDoubleByAngle) {
    Coords::angle a(33);
    double b(1.5);
    Coords::angle c;
    c = b * a;
    EXPECT_DOUBLE_EQ(49.5, c.degrees());
  }




  // divide

  TEST(angle, InplaceDivideByAngle) {
    Coords::angle a(90);
    Coords::angle b(2);
    a /= b;
    EXPECT_DOUBLE_EQ(45, a.degrees());
  }

  TEST(angle, InplaceDivideByDouble) {
    Coords::angle a(90);
    double b(2);
    a /= b;
    EXPECT_DOUBLE_EQ(45, a.degrees());
  }

  TEST(angle, InplaceDivideByZeroError) {
    Coords::angle a(45);
    Coords::angle b(0);
    EXPECT_THROW(a /= b, Coords::DivideByZeroError);
  }

  TEST(angle, InplaceDivideByZeroErrorMsg) {
    try {
      Coords::angle a(15);
      Coords::angle b;
      a /= b;
    } catch (Coords::DivideByZeroError& err) {
      EXPECT_STREQ(err.what(), "division by zero is undefined");
    }
  }

  TEST(angle, DivideAngleByAngle) {
    Coords::angle a(90);
    Coords::angle b(2);
    Coords::angle c;
    c = a / b;
    EXPECT_DOUBLE_EQ(45, c.degrees());
  }

  TEST(angle, DivideAngleByDouble) {
    Coords::angle a(90);
    double b(2);
    Coords::angle c;
    c = a / b;
    EXPECT_DOUBLE_EQ(45, c.degrees());
  }

  TEST(angle, DivideDoubleByAngle) {
    Coords::angle a(0.5);
    double b(33);
    Coords::angle c;
    c = b / a;
    EXPECT_DOUBLE_EQ(66, c.degrees());
  }

  // operator<<

  TEST(angle, operatorStdOut1) {
    Coords::angle a(44, 32, 15.4);
    std::stringstream out;
    out << a;
    EXPECT_STREQ("44:32:15.4", out.str().c_str());
  }

  TEST(angle, operatorStdOut2) {
    Coords::angle a(44, 32, 5.35);
    std::stringstream out;
    out << a;
    EXPECT_STREQ("44:32:05.4", out.str().c_str());
  }


  // 360 and beyond

  TEST(angle, Stdout360) {
    Coords::angle a(360);
    std::stringstream out;
    out << a;
    EXPECT_STREQ("360:00:00.0", out.str().c_str());
  }

  TEST(angle, Stdout361) {
    Coords::angle a(361);
    std::stringstream out;
    out << a;
    EXPECT_STREQ("361:00:00.0", out.str().c_str());
  }

  TEST(angle, RoundingIssuesBeyond360) {
    // had rounding issues when storing degrees in radians.
    Coords::angle a(45+360);
    std::stringstream out;
    out << a;
    EXPECT_STREQ("405:00:00.0", out.str().c_str());
  }

  // ---------------------
  // ----- normalize -----
  // ---------------------

  TEST(angle, Normalize_45) {
    Coords::angle a(45 + 360);
    a.normalize();
    EXPECT_NEAR(45, a.degrees(), 1e-15);
  }

  TEST(angle, Normalize_45_2) {
    Coords::angle a(45 - 360);
    a.normalize();
    EXPECT_NEAR(45, a.degrees(), 1e-15);
  }

  TEST(angle, Normalize_45n) {
    Coords::angle a(-45);
    a.normalize();
    EXPECT_NEAR(315, a.degrees(), 1e-15);
  }

  TEST(angle, Normalize_45n_2) {
    Coords::angle a(-45);
    a.normalize(-180, 180);
    EXPECT_NEAR(-45, a.degrees(), 1e-15);
  }

  TEST(angle, Normalize_45n_360) {
    Coords::angle a(-45 + 360);
    a.normalize(-180, 180);
    EXPECT_NEAR(-45, a.degrees(), 1e-15);
  }

  TEST(angle, Normalize_720) {
    Coords::angle a(45 + 360*2);
    a.normalize();
    EXPECT_NEAR(45, a.degrees(), 1e-15);
  }

  TEST(angle, Normalize_1080) {
    Coords::angle a(45 + 360*3);
    a.normalize();
    EXPECT_NEAR(45, a.degrees(), 1e-15);
  }

  // ----------------------
  // ----- complement -----
  // ----------------------

  TEST(angle, Complement_45) {
    Coords::angle a(45);
    Coords::angle c = a.complement();
    EXPECT_NEAR(45, c.degrees(), 1e-15);
  }

  TEST(angle, Complement_30) {
    Coords::angle a(30);
    Coords::angle c = a.complement();
    EXPECT_NEAR(60, c.degrees(), 1e-15);
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

  TEST(angle, output_dms) {
    Coords::angle a(12, 34, 56.78);

    std::stringstream out;
    Coords::degrees2DMSString(a.degrees(), out);

    EXPECT_STREQ("12* 34' 56.78\"", out.str().c_str());
  }

  TEST(angle, output_hms) {
    Coords::angle a(2, 4, 6);

    std::stringstream out;
    Coords::degrees2HMSString(a.degrees(), out);

    EXPECT_STREQ("02:04:06.0", out.str().c_str());
  }


  // Latitude

  TEST(Latitude, DefaultConstructor) {
    Coords::Latitude a;
    EXPECT_EQ(0, a.radians());
  }

  TEST(Latitude, ConstructorDeg) {
    Coords::Latitude a(90);
    EXPECT_EQ(Coords::Latitude::deg2rad(90), a.radians());
  }

  TEST(Latitude, ConstructorDegFromStr) {
    Coords::Latitude a("-90");
    EXPECT_EQ(Coords::Latitude::deg2rad(-90), a.radians());
  }

  TEST(Latitude, MaximumExceeded) {
    try {
      Coords::Latitude a(90.0001);
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "maximum exceeded");
    }

  }

  TEST(Latitude, MinimumExceeded) {
    try {
      Coords::Latitude a(-90.0001);
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "minimum exceeded");
    }

  }

  TEST(Latitude, Addition) {
    Coords::Latitude a(45);
    Coords::Latitude b(-40);
    Coords::angle c;

    c = a + b;

    EXPECT_EQ(5.0, c.degrees());

  }

  // Declination

  TEST(Declination, DefaultConstructor) {
    Coords::Declination a;
    EXPECT_EQ(0, a.radians());
  }

  TEST(Declination, ConstructorDeg) {
    Coords::Declination a(90);
    EXPECT_EQ(Coords::Declination::deg2rad(90), a.radians());
  }

  TEST(Declination, ConstructorDegFromStr) {
    Coords::Declination a("-90");
    EXPECT_EQ(Coords::Declination::deg2rad(-90), a.radians());
  }

  TEST(Declination, MaximumExceeded) {
    try {
      Coords::Declination a(90.0001);
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "maximum exceeded");
    }

  }

  TEST(Declination, MinimumExceeded) {
    try {
      Coords::Declination a(-90.0001);
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "minimum exceeded");
    }

  }

  TEST(Declination, Addition) {
    Coords::Declination a(45);
    Coords::Declination b(-40);
    Coords::angle c;

    c = a + b;

    EXPECT_EQ(5.0, c.degrees());

  }


} // end anonymous namespace



// ==================
// ===== main() =====
// ==================

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
