// ================================================================
// Filename:    Cartesian_unittest.cpp
// Description: This is the gtest unittest of the Cartesian library.
//
// Author:      L.R. McFarland, lrm@starbug.com
// Created:     2012 May 13
// Language:    C++
//
// See also: http://code.google.com/p/googletest/wiki/Primer
//
// To compile manually:
// g++ -I /usr/local/gtest-1.6.0/include -I . -c Cartesian_unittest.cpp
// g++ -L /usr/local/gtest-1.6.0 -lgtest Cartesian_unittest.o -o Cartesian_unittest
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


// TODO Rotation: more arbitrary rotations, copy and assign operators
// TODO CartesianRecorder


namespace {

  // ---------------------------
  // ----- Fixed Cartesian -----
  // ---------------------------

  TEST(FixedCartesian, Accessors) {

    Coords::Cartesian a;

    a.x(1.1);
    EXPECT_EQ(1.1, a.x());
    EXPECT_EQ(1.1, a.getX());

    a.y(-2.2);
    EXPECT_EQ(-2.2, a.y());
    EXPECT_EQ(-2.2, a.getY());

    a.z(3.3);
    EXPECT_EQ(3.3, a.z());
    EXPECT_EQ(3.3, a.getZ());

  }

  TEST(FixedCartesian, OutputOperator) {
    Coords::Cartesian a(1, 2, 3);
    std::stringstream out;
    out << a;
    EXPECT_STREQ("<Cartesian><x>1</x><y>2</y><z>3</z></Cartesian>", out.str().c_str());
  }

  TEST(FixedCartesian, UnitVectors) {
    EXPECT_EQ(Coords::Cartesian::Uo, Coords::Cartesian());
    EXPECT_EQ(Coords::Cartesian::Ux, Coords::Cartesian(1));
    EXPECT_EQ(Coords::Cartesian::Uy, Coords::Cartesian(0, 1));
    EXPECT_EQ(Coords::Cartesian::Uz, Coords::Cartesian(0, 0, 1));
  }

  TEST(FixedCartesian, Equivalence) {
    EXPECT_TRUE(Coords::Cartesian(1, 2, 3) == Coords::Cartesian(1.0, 2.0, 3.0));

    EXPECT_FALSE(Coords::Cartesian(1, 2, 3) == Coords::Cartesian(1.1, 2.0, 3.0));
    EXPECT_FALSE(Coords::Cartesian(1, 2, 3) == Coords::Cartesian(1.0, 2.1, 3.0));
    EXPECT_FALSE(Coords::Cartesian(1, 2, 3) == Coords::Cartesian(1.0, 2.0, 3.1));

    EXPECT_TRUE(Coords::Cartesian(1, 2, 3) != Coords::Cartesian(1.1, 2.0, 3.0));
    EXPECT_TRUE(Coords::Cartesian(1, 2, 3) != Coords::Cartesian(1.0, 2.1, 3.0));
    EXPECT_TRUE(Coords::Cartesian(1, 2, 3) != Coords::Cartesian(1.0, 2.0, 3.1));
  }

  TEST(FixedCartesian, DefaultConstructor) {
    Coords::Cartesian a;
    EXPECT_DOUBLE_EQ(0, a.x());
    EXPECT_DOUBLE_EQ(0, a.y());
    EXPECT_DOUBLE_EQ(0, a.z());
  }

  TEST(FixedCartesian, ConstructFromString) {

    Coords::Cartesian a("1.23");
    EXPECT_DOUBLE_EQ(1.23, a.x());
    EXPECT_DOUBLE_EQ(0.0, a.y());
    EXPECT_DOUBLE_EQ(0.0, a.z());

    Coords::Cartesian b("-1.23", "1.23e-7");
    EXPECT_DOUBLE_EQ(-1.23, b.x());
    EXPECT_DOUBLE_EQ(1.23e-7, b.y());
    EXPECT_DOUBLE_EQ(0.0, b.z());

    Coords::Cartesian c("1.23", "-1.23e-7", "10");
    EXPECT_DOUBLE_EQ(1.23, c.x());
    EXPECT_DOUBLE_EQ(-1.23e-7, c.y());
    EXPECT_DOUBLE_EQ(10.0, c.z());

  }

  TEST(FixedCartesian, ConstructFromBadString) {
    Coords::Cartesian def; // default

    // TODO in future, expect throw std::invalid_argument from std::stod.
    Coords::Cartesian d("asdf");
    EXPECT_EQ(def, d);

    Coords::Cartesian e("");
    EXPECT_EQ(def, e);

    Coords::Cartesian f("asdf 3.1415 blah");
    EXPECT_EQ(def, f);
  }


  // conversion constructors

  TEST(FixedCartesian, ConstructFromSpherical_x_1) {
    // all x
    Coords::spherical a(2.0, Coords::angle(90));
    Coords::Cartesian b(a);
    EXPECT_DOUBLE_EQ(2.0, b.x());
    EXPECT_DOUBLE_EQ(0.0, b.y());
    EXPECT_NEAR(0.0, b.z(), Coords::epsilon*10); // TODO meh?
  }

  TEST(FixedCartesian, ConstructFromSpherical_x_2) {
    // all -x
    Coords::spherical a(2.0, Coords::angle(-90));
    Coords::Cartesian b(a);
    EXPECT_DOUBLE_EQ(-2.0, b.x());
    EXPECT_DOUBLE_EQ(0.0, b.y());
    EXPECT_NEAR(0.0, b.z(), Coords::epsilon*10); // TODO meh?
  }


  TEST(FixedCartesian, ConstructFromSpherical_y_1) {
    // all y
    Coords::spherical a(3.0, Coords::angle(90), Coords::angle(90));
    Coords::Cartesian b(a);
    EXPECT_NEAR(0.0, b.x(), Coords::epsilon*10);
    EXPECT_DOUBLE_EQ(3.0, b.y());
    EXPECT_NEAR(0.0, b.z(), Coords::epsilon*10);
  }

  TEST(FixedCartesian, ConstructFromSpherical_y_2) {
    // all -y
    Coords::spherical a(3.0, Coords::angle(90), Coords::angle(-90));
    Coords::Cartesian b(a);
    EXPECT_NEAR(0.0, b.x(), Coords::epsilon*10);
    EXPECT_DOUBLE_EQ(-3.0, b.y());
    EXPECT_NEAR(0.0, b.z(), Coords::epsilon*10);
  }


  TEST(FixedCartesian, ConstructFromSpherical_z_1) {
    // all z
    Coords::spherical a(1.0);
    Coords::Cartesian b(a);
    EXPECT_DOUBLE_EQ(0.0, b.x());
    EXPECT_DOUBLE_EQ(0.0, b.y());
    EXPECT_DOUBLE_EQ(1.0, b.z());
  }

  TEST(FixedCartesian, ConstructFromSpherical_z_2) {
    // all -z
    Coords::spherical a(1.0, Coords::angle(180));
    Coords::Cartesian b(a);
    EXPECT_NEAR(0.0, b.x(), Coords::epsilon*10);
    EXPECT_DOUBLE_EQ(0.0, b.y());
    EXPECT_DOUBLE_EQ(-1.0, b.z());
  }


  TEST(FixedCartesian, ConstructFromSpherical_xy_1) {
    // xy first quadrant
    Coords::angle phi(30);
    Coords::spherical a(1.0, Coords::angle(90), phi);
    Coords::Cartesian b(a);
    EXPECT_DOUBLE_EQ(cos(phi.radians()), b.x());
    EXPECT_DOUBLE_EQ(sin(phi.radians()), b.y());
    EXPECT_NEAR(0.0, b.z(), Coords::epsilon*10);
  }

  TEST(FixedCartesian, ConstructFromSpherical_xy_2) {
    // xy second quadrant
    Coords::angle phi(90);
    Coords::spherical a(1.0, Coords::angle(90), phi);
    Coords::Cartesian b(a);
    EXPECT_DOUBLE_EQ(cos(phi.radians()), b.x());
    EXPECT_DOUBLE_EQ(sin(phi.radians()), b.y());
    EXPECT_NEAR(0.0, b.z(), Coords::epsilon*10);
  }

  TEST(FixedCartesian, ConstructFromSpherical_xy_3) {
    // xy third quadrant
    Coords::angle phi(190);
    Coords::spherical a(1.0, Coords::angle(90), phi);
    Coords::Cartesian b(a);
    EXPECT_DOUBLE_EQ(cos(phi.radians()), b.x());
    EXPECT_DOUBLE_EQ(sin(phi.radians()), b.y());
    EXPECT_NEAR(0.0, b.z(), Coords::epsilon*10);
  }

  TEST(FixedCartesian, ConstructFromSpherical_xy_4) {
    // xy fourth quadrant
    Coords::angle phi(-60);
    Coords::spherical a(1.0, Coords::angle(90), phi);
    Coords::Cartesian b(a);
    EXPECT_DOUBLE_EQ(cos(phi.radians()), b.x());
    EXPECT_DOUBLE_EQ(sin(phi.radians()), b.y());
    EXPECT_NEAR(0.0, b.z(), Coords::epsilon*10);
  }


  TEST(FixedCartesian, ConstructFromSpherical_yz_1) {
    // yz first quadrant
    Coords::angle theta(30);
    Coords::spherical a(1.0, theta, Coords::angle(90));
    Coords::Cartesian b(a);
    EXPECT_NEAR(0.0, b.x(), Coords::epsilon*10);
    EXPECT_DOUBLE_EQ(sin(theta.radians()), b.y());
    EXPECT_DOUBLE_EQ(cos(theta.radians()), b.z());
  }

  TEST(FixedCartesian, ConstructFromSpherical_yz_2) {
    // yz second quadrant
    Coords::angle theta(60);
    Coords::spherical a(1.0, theta, Coords::angle(-90));
    Coords::Cartesian b(a);
    EXPECT_NEAR(0.0, b.x(), Coords::epsilon*10);
    EXPECT_DOUBLE_EQ(-sin(theta.radians()), b.y());
    EXPECT_DOUBLE_EQ(cos(theta.radians()), b.z());
  }


  TEST(FixedCartesian, ConstructFromSpherical_yz_3) {
    // yz third quadrant
    Coords::angle theta(100);
    Coords::spherical a(1.0, theta, Coords::angle(-90));
    Coords::Cartesian b(a);
    EXPECT_NEAR(0.0, b.x(), Coords::epsilon*10);
    EXPECT_DOUBLE_EQ(-sin(theta.radians()), b.y());
    EXPECT_DOUBLE_EQ(cos(theta.radians()), b.z());
  }

  TEST(FixedCartesian, ConstructFromSpherical_yz_4) {
    // yz fourth quadrant
    Coords::angle theta(160);
    Coords::spherical a(1.0, theta, Coords::angle(90));
    Coords::Cartesian b(a);
    EXPECT_NEAR(0.0, b.x(), Coords::epsilon*10);
    EXPECT_DOUBLE_EQ(sin(theta.radians()), b.y());
    EXPECT_DOUBLE_EQ(cos(theta.radians()), b.z());
  }


  TEST(FixedCartesian, ConstructFromSpherical_xz_1) {
    // xz first quadrant
    Coords::angle theta(30);
    Coords::spherical a(1.0, theta);
    Coords::Cartesian b(a);
    EXPECT_DOUBLE_EQ(sin(theta.radians()), b.x());
    EXPECT_DOUBLE_EQ(0.0, b.y());
    EXPECT_DOUBLE_EQ(cos(theta.radians()), b.z());
  }

  TEST(FixedCartesian, ConstructFromSpherical_xz_2) {
    // xz second quadrant
    Coords::angle theta(-30);
    Coords::spherical a(1.0, theta);
    Coords::Cartesian b(a);
    EXPECT_DOUBLE_EQ(sin(theta.radians()), b.x());
    EXPECT_DOUBLE_EQ(0.0, b.y());
    EXPECT_DOUBLE_EQ(cos(theta.radians()), b.z());
  }

  TEST(FixedCartesian, ConstructFromSpherical_xz_3) {
    // xz third quadrant
    Coords::angle theta(-100);
    Coords::spherical a(1.0, theta);
    Coords::Cartesian b(a);
    EXPECT_DOUBLE_EQ(sin(theta.radians()), b.x());
    EXPECT_DOUBLE_EQ(0.0, b.y());
    EXPECT_DOUBLE_EQ(cos(theta.radians()), b.z());
  }

  TEST(FixedCartesian, ConstructFromSpherical_xz_4) {
    // xz fourth quadrant
    Coords::angle theta(120);
    Coords::spherical a(1.0, theta);
    Coords::Cartesian b(a);
    EXPECT_DOUBLE_EQ(sin(theta.radians()), b.x());
    EXPECT_DOUBLE_EQ(0.0, b.y());
    EXPECT_DOUBLE_EQ(cos(theta.radians()), b.z());
  }


  TEST(FixedCartesian, ConstructFromSpherical_theta45_1) {
    Coords::spherical a(1, Coords::angle(45), Coords::angle(45));
    Coords::Cartesian b(a);
    EXPECT_DOUBLE_EQ(0.5, b.x());
    EXPECT_DOUBLE_EQ(0.5, b.y());
    EXPECT_DOUBLE_EQ(0.70710678118654757, b.z());
  }

  TEST(FixedCartesian, ConstructFromSpherical_theta45_2) {
    Coords::spherical a(10.0, Coords::angle(-45), Coords::angle(45));
    Coords::Cartesian b(a);
    EXPECT_DOUBLE_EQ(-5.0, b.x());
    EXPECT_DOUBLE_EQ(-5.0, b.y());
    EXPECT_DOUBLE_EQ(7.0710678118654757, b.z());
  }

  TEST(FixedCartesian, ConstructFromSpherical_theta45_3) {
    Coords::spherical a(10.0, Coords::angle(135), Coords::angle(45));
    Coords::Cartesian b(a);
    EXPECT_DOUBLE_EQ(5.0, b.x());
    EXPECT_DOUBLE_EQ(5.0, b.y());
    EXPECT_DOUBLE_EQ(-7.0710678118654757, b.z());
  }

  TEST(FixedCartesian, ConstructFromSpherical_theta45_4) {
    Coords::spherical a(10.0, Coords::angle(-135), Coords::angle(45));
    Coords::Cartesian b(a);
    EXPECT_DOUBLE_EQ(-5.0, b.x());
    EXPECT_DOUBLE_EQ(-5.0, b.y());
    EXPECT_DOUBLE_EQ(-7.0710678118654757, b.z());
  }

  TEST(FixedCartesian, ConstructFromSpherical_theta45_5) {
    Coords::spherical a(10.0, Coords::angle(45), Coords::angle(-45));
    Coords::Cartesian b(a);
    EXPECT_DOUBLE_EQ(5.0, b.x());
    EXPECT_DOUBLE_EQ(-5.0, b.y());
    EXPECT_DOUBLE_EQ(7.0710678118654757, b.z());
  }

  TEST(FixedCartesian, ConstructFromSpherical_theta45_6) {
    Coords::spherical a(10.0, Coords::angle(45), Coords::angle(135));
    Coords::Cartesian b(a);
    EXPECT_DOUBLE_EQ(-5.0, b.x());
    EXPECT_DOUBLE_EQ(5.0, b.y());
    EXPECT_DOUBLE_EQ(7.0710678118654757, b.z());
  }






  TEST(FixedCartesian, Magnitude) {
    Coords::Cartesian a(1, 2, 3);
    EXPECT_DOUBLE_EQ(3.7416573867739413, a.magnitude());
  }

  TEST(FixedCartesian, Normalized) {
    Coords::Cartesian a(1, 2, 3);
    Coords::Cartesian a_norm(a.normalized());
    EXPECT_DOUBLE_EQ(1.0, a_norm.magnitude());
    EXPECT_DOUBLE_EQ(0.2672612419124244, a_norm.x());
    EXPECT_DOUBLE_EQ(0.53452248382484879, a_norm.y());
    EXPECT_DOUBLE_EQ(0.80178372573727319, a_norm.z());
  }

  TEST(FixedCartesian, DotProduct) {
    Coords::Cartesian dx2(-1, -2, -3);
    EXPECT_EQ(-1.0, Coords::dot(dx2, Coords::Cartesian::Ux));
    EXPECT_EQ(-2.0, Coords::dot(dx2, Coords::Cartesian::Uy));
    EXPECT_EQ(-3.0, Coords::dot(dx2, Coords::Cartesian::Uz));
  }

  TEST(FixedCartesian, XYCrossProduct) {
    Coords::Cartesian z = Coords::cross(Coords::Cartesian::Ux, Coords::Cartesian::Uy);
    EXPECT_EQ(Coords::Cartesian::Uz, z);
  }

  TEST(FixedCartesian, YZCrossProduct) {
    Coords::Cartesian x = Coords::cross(Coords::Cartesian::Uy, Coords::Cartesian::Uz);
    EXPECT_EQ(Coords::Cartesian::Ux, x);
  }

  TEST(FixedCartesian, ZXCrossProduct) {
    Coords::Cartesian y = Coords::cross(Coords::Cartesian::Uz, Coords::Cartesian::Ux);
    EXPECT_EQ(Coords::Cartesian::Uy, y);
  }

  TEST(FixedCartesian, CartesianException) {
    try {
      throw Coords::Error("custom error message");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(),"custom error message");
    }
  }

  TEST(FixedCartesian, InplaceDivideByZeroException) {
    try {
      Coords::Cartesian a; a /= 0;
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "division by zero is undefined");
    }
  }

  // ----------------------------
  // ----- Random Cartesian -----
  // ----------------------------

  class RandomCartesian : public ::testing::Test {
    // Creates new random x, y, z values each test.
  protected:

    virtual void SetUp() {

      seed = std::chrono::system_clock::now().time_since_epoch().count();
      lo = -1e3;
      hi = 1e3;

      std::default_random_engine generator(seed);
      std::uniform_real_distribution<double> distribution(lo, hi);

      p1.x(distribution(generator));
      p1.y(distribution(generator));
      p1.z(distribution(generator));

      p2.x(distribution(generator));
      p2.y(distribution(generator));
      p2.z(distribution(generator));

      c = distribution(generator);
    }

    virtual void TearDown() {}

    // members

    unsigned int seed;
    double lo;
    double hi;

    Coords::Cartesian p1;
    Coords::Cartesian p2;
    double c; // random double

  };

  TEST_F(RandomCartesian, CopyConstructor) {
    Coords::Cartesian a(p1);
    EXPECT_EQ(p1, a);
  }

  TEST_F(RandomCartesian, CopyAssign) {
    Coords::Cartesian a;
    a = p1;
    EXPECT_EQ(p1, a);
  }

  TEST_F(RandomCartesian, Zero) {
    Coords::Cartesian a(p1);
    a.zero();
    EXPECT_EQ(Coords::Cartesian::Uo, a);
  }


  TEST_F(RandomCartesian, AddInplace) {
    Coords::Cartesian result(p1.x() + p2.x(),
			     p1.y() + p2.y(),
			     p1.z() + p2.z());
    Coords::Cartesian a(p1);
    a += p2;
    EXPECT_EQ(result, a);
  }

  TEST_F(RandomCartesian, CartesianPlusCartesian) {
    Coords::Cartesian result(p1.x() + p2.x(),
			     p1.y() + p2.y(),
			     p1.z() + p2.z());
    Coords::Cartesian a;
    a = p1 + p2;
    EXPECT_EQ(result, a);
  }

  TEST_F(RandomCartesian, SubtractInplace) {
    Coords::Cartesian result(p1.x() - p2.x(),
			     p1.y() - p2.y(),
			     p1.z() - p2.z());
    Coords::Cartesian a(p1);
    a -= p2;
    EXPECT_EQ(result, a);
  }

  TEST_F(RandomCartesian, UnitaryMinus) {
    Coords::Cartesian result(-p1.x(),
			     -p1.y(),
			     -p1.z());
    Coords::Cartesian a(p1);
    a = -p1;
    EXPECT_EQ(result, a);
  }

  TEST_F(RandomCartesian, CartesianMinusCartesian) {
    Coords::Cartesian result(p1.x() - p2.x(),
			     p1.y() - p2.y(),
			     p1.z() - p2.z());
    Coords::Cartesian a;
    a = p1 - p2;
    EXPECT_EQ(result, a);
  }

  TEST_F(RandomCartesian, CartesianTimesCartesian) {
    // dot product
    double result(p1.x() * p2.x() +
		  p1.y() * p2.y() +
		  p1.z() * p2.z());
    double a;
    a = p1 * p2;
    EXPECT_EQ(result, a);
  }

  TEST_F(RandomCartesian, CartesianTimesDouble) {
    // scale
    Coords::Cartesian result(p1.x() * c,
			     p1.y() * c,
			     p1.z() * c);
    Coords::Cartesian a;
    a = p1 * c;
    EXPECT_EQ(result, a);
  }

  TEST_F(RandomCartesian, DoubleTimesCartesian) {
    // scale commutes
    Coords::Cartesian result(p1.x() * c,
			     p1.y() * c,
			     p1.z() * c);

    Coords::Cartesian a;
    a = c * p1;
    EXPECT_EQ(result, a);
  }

  TEST_F(RandomCartesian, CartesianTimesDoubleInplace) {
    Coords::Cartesian result(p1.x() * c,
			     p1.y() * c,
			     p1.z() * c);
    Coords::Cartesian a(p1);
    a *= c;
    EXPECT_EQ(result, a);
  }

  TEST_F(RandomCartesian, CartesianDivideDouble) {
    // scale
    Coords::Cartesian result(p1.x() / c,
			     p1.y() / c,
			     p1.z() / c);
    Coords::Cartesian a;
    a = p1 / c;
    EXPECT_EQ(result, a);
  }

  TEST_F(RandomCartesian, CartesianDivideDoubleInplace) {
    Coords::Cartesian result(p1.x() / c,
			     p1.y() / c,
			     p1.z() / c);
    Coords::Cartesian a(p1);
    a /= c;
    EXPECT_EQ(result, a);
  }

  TEST_F(RandomCartesian, DivideByZeroException) {
    Coords::Cartesian a(p1);
    EXPECT_THROW(a/0, Coords::DivideByZeroError);
    EXPECT_THROW(a/=0, Coords::DivideByZeroError);
    EXPECT_THROW(1.0/Coords::Cartesian::Uo, Coords::DivideByZeroError);
  }

  TEST_F(RandomCartesian, Magnitude) {
    double result(sqrt(p1.x()*p1.x() + p1.y()*p1.y() + p1.z()*p1.z()));
    Coords::Cartesian a(p1);
    EXPECT_EQ(result, a.magnitude());
  }

  TEST_F(RandomCartesian, Magnitude2) {
    double result(p1.x()*p1.x() + p1.y()*p1.y() + p1.z()*p1.z());
    Coords::Cartesian a(p1);
    EXPECT_EQ(result, a.magnitude2());
  }

  TEST_F(RandomCartesian, Normalized) {
    double mag(sqrt(p1.x()*p1.x() + p1.y()*p1.y() + p1.z()*p1.z()));
    Coords::Cartesian result(p1.x()/mag,
			     p1.y()/mag,
			     p1.z()/mag);
    EXPECT_EQ(result, p1.normalized());
  }

  TEST_F(RandomCartesian, DotProduct) {
    double result(p1.x() * p2.x() +
		  p1.y() * p2.y() +
		  p1.z() * p2.z());
    double a(Coords::dot(p1, p2));
    EXPECT_EQ(result, a);
  }

  TEST_F(RandomCartesian, CrossProduct) {
    Coords::Cartesian result(p1.y() * p2.z() - p1.z()*p2.y(),
			     p1.z() * p2.x() - p1.x()*p2.z(),
			     p1.x() * p2.y() - p1.y()*p2.x());
    Coords::Cartesian a(Coords::cross(p1, p2));
    EXPECT_EQ(result, a);
  }

  // ----------------------------
  // ----- X Rotation tests -----
  // ----------------------------


  TEST(XRotationTest, UxAboutUztoUy) {
    // Ux counter clockwise about Uz to Uy

    Coords::angle an_angle(90);
    Coords::rotator about_z(Coords::Cartesian::Uz);

    Coords::Cartesian s(about_z.rotate(Coords::Cartesian::Ux, an_angle));

    EXPECT_NEAR(Coords::Cartesian::Uy.x(), s.x(), Coords::epsilon);
    EXPECT_DOUBLE_EQ(Coords::Cartesian::Uy.y(), s.y());
    EXPECT_DOUBLE_EQ(Coords::Cartesian::Uy.z(), s.z());

  }

  TEST(XRotationTest, UxAboutUytoUz) {
    // Ux clockwise about Uy to Uz

    Coords::angle an_angle(-90);
    Coords::rotator about_y(Coords::Cartesian::Uy);

    Coords::Cartesian s(about_y.rotate(Coords::Cartesian::Ux, an_angle));

    EXPECT_NEAR(Coords::Cartesian::Uz.x(), s.x(), Coords::epsilon);
    EXPECT_DOUBLE_EQ(Coords::Cartesian::Uz.y(), s.y());
    EXPECT_DOUBLE_EQ(Coords::Cartesian::Uz.z(), s.z());

  }

  // ----------------------------
  // ----- Y Rotation tests -----
  // ----------------------------

  TEST(YRotationTest, UyAboutUxtoUz) {
    // Uy counter clockwise about Ux to Uz

    Coords::angle an_angle(90);
    Coords::rotator about_x(Coords::Cartesian::Ux);

    Coords::Cartesian s(about_x.rotate(Coords::Cartesian::Uy, an_angle));

    EXPECT_DOUBLE_EQ(Coords::Cartesian::Uz.x(), s.x());
    EXPECT_NEAR(Coords::Cartesian::Uz.y(), s.y(), Coords::epsilon);
    EXPECT_DOUBLE_EQ(Coords::Cartesian::Uz.z(), s.z());

  }

  TEST(YRotationTest, UyAboutUztoUx) {
    // Uy clockwise about Uz to Ux

    Coords::angle an_angle(-90);
    Coords::rotator about_z(Coords::Cartesian::Uz);

    Coords::Cartesian s(about_z.rotate(Coords::Cartesian::Uy, an_angle));

    EXPECT_DOUBLE_EQ(Coords::Cartesian::Ux.x(), s.x());
    EXPECT_NEAR(Coords::Cartesian::Ux.y(), s.y(), Coords::epsilon);
    EXPECT_DOUBLE_EQ(Coords::Cartesian::Ux.z(), s.z());

  }

  // ----------------------------
  // ----- Z Rotation tests -----
  // ----------------------------

  TEST(ZRotationTest, UzAboutUytoUx) {
    // Uz counter clockwise about Uy to Ux

    Coords::angle an_angle(90);
    Coords::rotator about_y(Coords::Cartesian::Uy);

    Coords::Cartesian s(about_y.rotate(Coords::Cartesian::Uz, an_angle));

    EXPECT_DOUBLE_EQ(Coords::Cartesian::Ux.x(), s.x());
    EXPECT_DOUBLE_EQ(Coords::Cartesian::Ux.y(), s.y());
    EXPECT_NEAR(Coords::Cartesian::Ux.z(), s.z(), Coords::epsilon);

  }

  TEST(ZRotationTest, UzAboutUxtoUy) {
    // Uz clockwise about Ux to Uy

    Coords::angle an_angle(-90);
    Coords::rotator about_x(Coords::Cartesian::Ux);

    Coords::Cartesian s(about_x.rotate(Coords::Cartesian::Uz, an_angle));

    EXPECT_DOUBLE_EQ(Coords::Cartesian::Uy.x(), s.x());
    EXPECT_DOUBLE_EQ(Coords::Cartesian::Uy.y(), s.y());
    EXPECT_NEAR(Coords::Cartesian::Uy.z(), s.z(), Coords::epsilon);

  }




  // ---------------------------------
  // ----- non-trivial rotations -----
  // ---------------------------------

  TEST(RotationTest, FirstDiagonal_0) {

    // spin about same axis

    Coords::angle an_angle(90);
    Coords::Cartesian first_diagonal(1, 1, 1);

    Coords::rotator about_diagonal(first_diagonal);

    Coords::Cartesian opposite_diagonal(-1, -1, -1);
    Coords::Cartesian s(about_diagonal.rotate(opposite_diagonal, an_angle));

    EXPECT_DOUBLE_EQ(-1.0, s.x());
    EXPECT_DOUBLE_EQ(-1.0, s.y());
    EXPECT_DOUBLE_EQ(-1.0, s.z());

  }

  TEST(RotationTest, FirstDiagonal_1) {

    // full circle

    Coords::angle an_angle(360);
    Coords::Cartesian first_diagonal(1, 1, 1);

    Coords::rotator about_diagonal(first_diagonal);

    Coords::Cartesian opposite_diagonal(-1, -1, 1);
    Coords::Cartesian s(about_diagonal.rotate(opposite_diagonal, an_angle));

    EXPECT_DOUBLE_EQ(-1.0, s.x());
    EXPECT_DOUBLE_EQ(-1.0, s.y());
    EXPECT_DOUBLE_EQ(1.0, s.z());

  }

  TEST(DISABLED_RotationTest, FirstDiagonal_2) {

    // half circle

    Coords::angle an_angle(180);
    Coords::Cartesian first_diagonal(1, 1, 1);

    Coords::spherical sph_fd(first_diagonal); // TODO rm
    std::cout << "first diagonal" << sph_fd << std::endl; // TODO rm

    Coords::rotator about_diagonal(first_diagonal);

    Coords::Cartesian opposite_diagonal(-1, -1, 1);

    Coords::spherical sph_od(opposite_diagonal); // TODO rm
    std::cout << "opposite diagonal" << sph_od << std::endl; // TODO rm


    Coords::Cartesian s(about_diagonal.rotate(opposite_diagonal, an_angle));
    std::cout << s << std::endl; // TODO rm

    Coords::spherical sph_rod(s); // TODO rm
    std::cout << "rotated diagonal" << sph_rod << std::endl; // TODO rm

    EXPECT_DOUBLE_EQ(sph_fd.r(), sph_rod.r());
    EXPECT_NEAR(sph_fd.phi().value(), sph_rod.phi().value(), Coords::epsilon*1000); // accumulated rounding error!?!


    EXPECT_DOUBLE_EQ(1.0, s.x());
    EXPECT_DOUBLE_EQ(1.0, s.y());
    EXPECT_DOUBLE_EQ(-1.0, s.z());

  }






} // end anonymous namespace


// ==================
// ===== main() =====
// ==================

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
