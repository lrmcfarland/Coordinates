// ================================================================
// Filename:    cartesian_unittest.cpp
// Description: This is the gtest unittest of the cartesian library.
//
// Author:      L.R. McFarland, lrm@starbug.com
// Created:     2012 May 13
// Language:    C++
//
// See also: http://code.google.com/p/googletest/wiki/Primer
//
// To compile manually:
// g++ -I /usr/local/gtest-1.6.0/include -I . -c cartesian_unittest.cpp
// g++ -L /usr/local/gtest-1.6.0 -lgtest cartesian_unittest.o -o cartesian_unittest
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
#include <cartesian.h>
#include <spherical.h>


// TODO Rotation: more arbitrary rotations, copy and assign operators
// TODO CartesianRecorder


namespace {

  // ---------------------------
  // ----- Fixed Cartesian -----
  // ---------------------------

  TEST(FixedCartesian, Accessors) {

    Coords::cartesian a;

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
    Coords::cartesian a(1, 2, 3);
    std::stringstream out;
    out << a;
    EXPECT_STREQ("<cartesian><x>1</x><y>2</y><z>3</z></cartesian>", out.str().c_str());
  }

  TEST(FixedCartesian, UnitVectors) {
    EXPECT_EQ(Coords::cartesian::Uo, Coords::cartesian());
    EXPECT_EQ(Coords::cartesian::Ux, Coords::cartesian(1));
    EXPECT_EQ(Coords::cartesian::Uy, Coords::cartesian(0, 1));
    EXPECT_EQ(Coords::cartesian::Uz, Coords::cartesian(0, 0, 1));
  }

  TEST(FixedCartesian, Equivalence) {
    EXPECT_TRUE(Coords::cartesian(1, 2, 3) == Coords::cartesian(1.0, 2.0, 3.0));

    EXPECT_FALSE(Coords::cartesian(1, 2, 3) == Coords::cartesian(1.1, 2.0, 3.0));
    EXPECT_FALSE(Coords::cartesian(1, 2, 3) == Coords::cartesian(1.0, 2.1, 3.0));
    EXPECT_FALSE(Coords::cartesian(1, 2, 3) == Coords::cartesian(1.0, 2.0, 3.1));

    EXPECT_TRUE(Coords::cartesian(1, 2, 3) != Coords::cartesian(1.1, 2.0, 3.0));
    EXPECT_TRUE(Coords::cartesian(1, 2, 3) != Coords::cartesian(1.0, 2.1, 3.0));
    EXPECT_TRUE(Coords::cartesian(1, 2, 3) != Coords::cartesian(1.0, 2.0, 3.1));
  }

  TEST(FixedCartesian, DefaultConstructor) {
    Coords::cartesian a;
    EXPECT_DOUBLE_EQ(0, a.x());
    EXPECT_DOUBLE_EQ(0, a.y());
    EXPECT_DOUBLE_EQ(0, a.z());
  }

  TEST(FixedCartesian, ConstructFromString) {

    Coords::cartesian a("1.23");
    EXPECT_DOUBLE_EQ(1.23, a.x());
    EXPECT_DOUBLE_EQ(0.0, a.y());
    EXPECT_DOUBLE_EQ(0.0, a.z());

    Coords::cartesian b("-1.23", "1.23e-7");
    EXPECT_DOUBLE_EQ(-1.23, b.x());
    EXPECT_DOUBLE_EQ(1.23e-7, b.y());
    EXPECT_DOUBLE_EQ(0.0, b.z());

    Coords::cartesian c("1.23", "-1.23e-7", "10");
    EXPECT_DOUBLE_EQ(1.23, c.x());
    EXPECT_DOUBLE_EQ(-1.23e-7, c.y());
    EXPECT_DOUBLE_EQ(10.0, c.z());

  }

  TEST(FixedCartesian, ConstructFromBadString) {
    Coords::cartesian def; // default

    // TODO in future, expect throw std::invalid_argument from std::stod.
    Coords::cartesian d("asdf");
    EXPECT_EQ(def, d);

    Coords::cartesian e("");
    EXPECT_EQ(def, e);

    Coords::cartesian f("asdf 3.1415 blah");
    EXPECT_EQ(def, f);
  }




  TEST(FixedCartesian, ConstructFromSpherical_z) {
    // all z
    Coords::spherical a(1.0);
    Coords::cartesian b(a);
    EXPECT_DOUBLE_EQ(0.0, b.x());
    EXPECT_DOUBLE_EQ(0.0, b.y());
    EXPECT_DOUBLE_EQ(1.0, b.z());
  }

  TEST(FixedCartesian, ConstructFromSpherical_x) {
    // all x
    Coords::spherical a(2.0, Coords::Angle(90));
    Coords::cartesian b(a);
    EXPECT_DOUBLE_EQ(2.0, b.x());
    EXPECT_DOUBLE_EQ(0.0, b.y());
    EXPECT_NEAR(0.0, b.z(), Coords::epsilon*10); // TODO meh?
  }


  TEST(FixedCartesian, ConstructFromSpherical_y_1) {
    // all y
    Coords::spherical a(3.0, Coords::Angle(90), Coords::Angle(90));
    Coords::cartesian b(a);
    EXPECT_NEAR(0.0, b.x(), Coords::epsilon*10); // TODO meh?
    EXPECT_DOUBLE_EQ(3.0, b.y());
    EXPECT_NEAR(0.0, b.z(), Coords::epsilon*10); // TODO meh?
  }

  TEST(FixedCartesian, ConstructFromSpherical_y_2) {
    Coords::spherical a(10.0, Coords::Angle(-90), Coords::Angle(45));
    Coords::cartesian b(a);
    EXPECT_DOUBLE_EQ(-7.0710678118654755, b.x());
    EXPECT_DOUBLE_EQ(-7.0710678118654755, b.y());
    EXPECT_NEAR(0.0, b.z(), Coords::epsilon*10); // TODO meh?
  }


  TEST(FixedCartesian, ConstructFromSpherical_theta45_1) {
    Coords::spherical a(1, Coords::Angle(45), Coords::Angle(45));
    Coords::cartesian b(a);
    EXPECT_DOUBLE_EQ(0.5, b.x());
    EXPECT_DOUBLE_EQ(0.5, b.y());
    EXPECT_DOUBLE_EQ(0.70710678118654757, b.z());
  }

  TEST(FixedCartesian, ConstructFromSpherical_theta45_2) {
    Coords::spherical a(10.0, Coords::Angle(-45), Coords::Angle(45));
    Coords::cartesian b(a);
    EXPECT_DOUBLE_EQ(-5.0, b.x());
    EXPECT_DOUBLE_EQ(-5.0, b.y());
    EXPECT_DOUBLE_EQ(7.0710678118654757, b.z());
  }

  TEST(FixedCartesian, ConstructFromSpherical_theta45_3) {
    Coords::spherical a(10.0, Coords::Angle(135), Coords::Angle(45));
    Coords::cartesian b(a);
    EXPECT_DOUBLE_EQ(5.0, b.x());
    EXPECT_DOUBLE_EQ(5.0, b.y());
    EXPECT_DOUBLE_EQ(-7.0710678118654757, b.z());
  }

  TEST(FixedCartesian, ConstructFromSpherical_theta45_4) {
    Coords::spherical a(10.0, Coords::Angle(-135), Coords::Angle(45));
    Coords::cartesian b(a);
    EXPECT_DOUBLE_EQ(-5.0, b.x());
    EXPECT_DOUBLE_EQ(-5.0, b.y());
    EXPECT_DOUBLE_EQ(-7.0710678118654757, b.z());
  }

  TEST(FixedCartesian, ConstructFromSpherical_theta45_5) {
    Coords::spherical a(10.0, Coords::Angle(45), Coords::Angle(-45));
    Coords::cartesian b(a);
    EXPECT_DOUBLE_EQ(5.0, b.x());
    EXPECT_DOUBLE_EQ(-5.0, b.y());
    EXPECT_DOUBLE_EQ(7.0710678118654757, b.z());
  }

  TEST(FixedCartesian, ConstructFromSpherical_theta45_6) {
    Coords::spherical a(10.0, Coords::Angle(45), Coords::Angle(135));
    Coords::cartesian b(a);
    EXPECT_DOUBLE_EQ(-5.0, b.x());
    EXPECT_DOUBLE_EQ(5.0, b.y());
    EXPECT_DOUBLE_EQ(7.0710678118654757, b.z());
  }






  TEST(FixedCartesian, Magnitude) {
    Coords::cartesian a(1, 2, 3);
    EXPECT_DOUBLE_EQ(3.7416573867739413, a.magnitude());
  }

  TEST(FixedCartesian, Normalized) {
    Coords::cartesian a(1, 2, 3);
    Coords::cartesian a_norm(a.normalized());
    EXPECT_DOUBLE_EQ(1.0, a_norm.magnitude());
    EXPECT_DOUBLE_EQ(0.2672612419124244, a_norm.x());
    EXPECT_DOUBLE_EQ(0.53452248382484879, a_norm.y());
    EXPECT_DOUBLE_EQ(0.80178372573727319, a_norm.z());
  }

  TEST(FixedCartesian, DotProduct) {
    Coords::cartesian dx2(-1, -2, -3);
    EXPECT_EQ(-1.0, Coords::dot(dx2, Coords::cartesian::Ux));
    EXPECT_EQ(-2.0, Coords::dot(dx2, Coords::cartesian::Uy));
    EXPECT_EQ(-3.0, Coords::dot(dx2, Coords::cartesian::Uz));
  }

  TEST(FixedCartesian, XYCrossProduct) {
    Coords::cartesian z = Coords::cross(Coords::cartesian::Ux, Coords::cartesian::Uy);
    EXPECT_EQ(Coords::cartesian::Uz, z);
  }

  TEST(FixedCartesian, YZCrossProduct) {
    Coords::cartesian x = Coords::cross(Coords::cartesian::Uy, Coords::cartesian::Uz);
    EXPECT_EQ(Coords::cartesian::Ux, x);
  }

  TEST(FixedCartesian, ZXCrossProduct) {
    Coords::cartesian y = Coords::cross(Coords::cartesian::Uz, Coords::cartesian::Ux);
    EXPECT_EQ(Coords::cartesian::Uy, y);
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
      Coords::cartesian a; a /= 0;
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

    Coords::cartesian p1;
    Coords::cartesian p2;
    double c; // random double

  };

  TEST_F(RandomCartesian, CopyConstructor) {
    Coords::cartesian a(p1);
    EXPECT_EQ(p1, a);
  }

  TEST_F(RandomCartesian, CopyAssign) {
    Coords::cartesian a;
    a = p1;
    EXPECT_EQ(p1, a);
  }

  TEST_F(RandomCartesian, Zero) {
    Coords::cartesian a(p1);
    a.zero();
    EXPECT_EQ(Coords::cartesian::Uo, a);
  }


  TEST_F(RandomCartesian, AddInplace) {
    Coords::cartesian result(p1.x() + p2.x(),
			     p1.y() + p2.y(),
			     p1.z() + p2.z());
    Coords::cartesian a(p1);
    a += p2;
    EXPECT_EQ(result, a);
  }

  TEST_F(RandomCartesian, CartesianPlusCartesian) {
    Coords::cartesian result(p1.x() + p2.x(),
			     p1.y() + p2.y(),
			     p1.z() + p2.z());
    Coords::cartesian a;
    a = p1 + p2;
    EXPECT_EQ(result, a);
  }

  TEST_F(RandomCartesian, SubtractInplace) {
    Coords::cartesian result(p1.x() - p2.x(),
			     p1.y() - p2.y(),
			     p1.z() - p2.z());
    Coords::cartesian a(p1);
    a -= p2;
    EXPECT_EQ(result, a);
  }

  TEST_F(RandomCartesian, UnitaryMinus) {
    Coords::cartesian result(-p1.x(),
			     -p1.y(),
			     -p1.z());
    Coords::cartesian a(p1);
    a = -p1;
    EXPECT_EQ(result, a);
  }

  TEST_F(RandomCartesian, CartesianMinusCartesian) {
    Coords::cartesian result(p1.x() - p2.x(),
			     p1.y() - p2.y(),
			     p1.z() - p2.z());
    Coords::cartesian a;
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
    Coords::cartesian result(p1.x() * c,
			     p1.y() * c,
			     p1.z() * c);
    Coords::cartesian a;
    a = p1 * c;
    EXPECT_EQ(result, a);
  }

  TEST_F(RandomCartesian, DoubleTimesCartesian) {
    // scale commutes
    Coords::cartesian result(p1.x() * c,
			     p1.y() * c,
			     p1.z() * c);

    Coords::cartesian a;
    a = c * p1;
    EXPECT_EQ(result, a);
  }

  TEST_F(RandomCartesian, CartesianTimesDoubleInplace) {
    Coords::cartesian result(p1.x() * c,
			     p1.y() * c,
			     p1.z() * c);
    Coords::cartesian a(p1);
    a *= c;
    EXPECT_EQ(result, a);
  }

  TEST_F(RandomCartesian, CartesianDivideDouble) {
    // scale
    Coords::cartesian result(p1.x() / c,
			     p1.y() / c,
			     p1.z() / c);
    Coords::cartesian a;
    a = p1 / c;
    EXPECT_EQ(result, a);
  }

  TEST_F(RandomCartesian, CartesianDivideDoubleInplace) {
    Coords::cartesian result(p1.x() / c,
			     p1.y() / c,
			     p1.z() / c);
    Coords::cartesian a(p1);
    a /= c;
    EXPECT_EQ(result, a);
  }

  TEST_F(RandomCartesian, DivideByZeroException) {
    Coords::cartesian a(p1);
    EXPECT_THROW(a/0, Coords::DivideByZeroError);
    EXPECT_THROW(a/=0, Coords::DivideByZeroError);
    EXPECT_THROW(1.0/Coords::cartesian::Uo, Coords::DivideByZeroError);
  }

  TEST_F(RandomCartesian, Magnitude) {
    double result(sqrt(p1.x()*p1.x() + p1.y()*p1.y() + p1.z()*p1.z()));
    Coords::cartesian a(p1);
    EXPECT_EQ(result, a.magnitude());
  }

  TEST_F(RandomCartesian, Magnitude2) {
    double result(p1.x()*p1.x() + p1.y()*p1.y() + p1.z()*p1.z());
    Coords::cartesian a(p1);
    EXPECT_EQ(result, a.magnitude2());
  }

  TEST_F(RandomCartesian, Normalized) {
    double mag(sqrt(p1.x()*p1.x() + p1.y()*p1.y() + p1.z()*p1.z()));
    Coords::cartesian result(p1.x()/mag,
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
    Coords::cartesian result(p1.y() * p2.z() - p1.z()*p2.y(),
			     p1.z() * p2.x() - p1.x()*p2.z(),
			     p1.x() * p2.y() - p1.y()*p2.x());
    Coords::cartesian a(Coords::cross(p1, p2));
    EXPECT_EQ(result, a);
  }

  // ----------------------------
  // ----- X Rotation tests -----
  // ----------------------------


  TEST(XRotationTest, Positive90AboutZ) {

    double angle(Coords::Angle::deg2rad(90));
    Coords::rotator about_z(Coords::cartesian::Uz);

    Coords::cartesian s(about_z.rotate(Coords::cartesian::Ux, angle));

    EXPECT_NEAR(Coords::cartesian::Uy.x(), s.x(), Coords::epsilon);
    EXPECT_DOUBLE_EQ(Coords::cartesian::Uy.y(), s.y());
    EXPECT_DOUBLE_EQ(Coords::cartesian::Uy.z(), s.z());

  }

  TEST(XRotationTest, Negative90AboutY) {

    double angle(Coords::Angle::deg2rad(-90));
    Coords::rotator about_y(Coords::cartesian::Uy);

    Coords::cartesian s(about_y.rotate(Coords::cartesian::Ux, angle));

    EXPECT_NEAR(Coords::cartesian::Uz.x(), s.x(), Coords::epsilon);
    EXPECT_DOUBLE_EQ(Coords::cartesian::Uz.y(), s.y());
    EXPECT_DOUBLE_EQ(Coords::cartesian::Uz.z(), s.z());

  }

  // ----------------------------
  // ----- Y Rotation tests -----
  // ----------------------------

  TEST(YRotationTest, Positive90AboutX) {

    double angle(Coords::Angle::deg2rad(90));
    Coords::rotator about_x(Coords::cartesian::Ux);

    Coords::cartesian s(about_x.rotate(Coords::cartesian::Uy, angle));

    EXPECT_DOUBLE_EQ(Coords::cartesian::Uz.x(), s.x());
    EXPECT_NEAR(Coords::cartesian::Uz.y(), s.y(), Coords::epsilon);
    EXPECT_DOUBLE_EQ(Coords::cartesian::Uz.z(), s.z());

  }

  TEST(YRotationTest, Negative90AboutZ) {

    double angle(Coords::Angle::deg2rad(-90));
    Coords::rotator about_z(Coords::cartesian::Uz);

    Coords::cartesian s(about_z.rotate(Coords::cartesian::Uy, angle));

    EXPECT_DOUBLE_EQ(Coords::cartesian::Ux.x(), s.x());
    EXPECT_NEAR(Coords::cartesian::Ux.y(), s.y(), Coords::epsilon);
    EXPECT_DOUBLE_EQ(Coords::cartesian::Ux.z(), s.z());

  }

  // ----------------------------
  // ----- Z Rotation tests -----
  // ----------------------------

  TEST(ZRotationTest, Positive90AboutY) {

    double angle(Coords::Angle::deg2rad(90));
    Coords::rotator about_y(Coords::cartesian::Uy);

    Coords::cartesian s(about_y.rotate(Coords::cartesian::Uz, angle));

    EXPECT_DOUBLE_EQ(Coords::cartesian::Ux.x(), s.x());
    EXPECT_DOUBLE_EQ(Coords::cartesian::Ux.y(), s.y());
    EXPECT_NEAR(Coords::cartesian::Ux.z(), s.z(), Coords::epsilon);

  }

  TEST(ZRotationTest, Negative90AboutX) {

    double angle(Coords::Angle::deg2rad(-90));
    Coords::rotator about_x(Coords::cartesian::Ux);

    Coords::cartesian s(about_x.rotate(Coords::cartesian::Uz, angle));

    EXPECT_DOUBLE_EQ(Coords::cartesian::Uy.x(), s.x());
    EXPECT_DOUBLE_EQ(Coords::cartesian::Uy.y(), s.y());
    EXPECT_NEAR(Coords::cartesian::Uy.z(), s.z(), Coords::epsilon);

  }

} // end anonymous namespace


// ==================
// ===== main() =====
// ==================

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
