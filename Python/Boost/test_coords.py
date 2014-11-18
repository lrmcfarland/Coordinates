"""Unit tests for coords objects.

It uses the random number generator to select test targets, i.e. the
test is different each time it is run. It is designed to stay in range
except when testing the range limits, but it may exceeded them in
which case the test should be investigated.
"""

import copy
import math
import random
import time
import unittest

import coords


class TestAngle(unittest.TestCase):

    def setUp(self):

        """Set up test parameters."""

        self.places = 7 # precision

        self.lower_range = -720
        self.upper_range = 720

        self.lower_minute_range = -60
        self.upper_minute_range = 60

        self.rd1 = random.uniform(self.lower_range, self.upper_range)
        self.rm1 = random.uniform(self.lower_minute_range, self.upper_minute_range)
        self.rs1 = random.uniform(self.lower_minute_range, self.upper_minute_range)

        self.rd2 = random.uniform(self.lower_range, self.upper_range)
        self.rm2 = random.uniform(self.lower_minute_range, self.upper_minute_range)
        self.rs2 = random.uniform(self.lower_minute_range, self.upper_minute_range)

    # static methods

    # Note these are different from python/Manual in that they are
    # instance methods not module methods.

    def test_deg2rad_1(self):
        """Test deg2rad"""
        self.assertAlmostEqual(math.pi/2.0, coords.angle.deg2rad(90), self.places)

    def test_deg2rad_2(self):
        """Test deg2rad negative angle"""
        self.assertAlmostEqual(-math.pi, coords.angle.deg2rad(-180), self.places)


    def test_rad2deg_1(self):
        """Test rad2deg 1"""
        self.assertAlmostEqual(270, coords.angle.rad2deg(3.0*math.pi/2.0), self.places)

    def test_rad2deg_2(self):
        """Test rad2deg negative angle"""
        self.assertAlmostEqual(-180, coords.angle.rad2deg(-math.pi), self.places)

    # booleans

    @unittest.skip('TODO boost wrap operator==()')
    def test_angle1_lt_angle1(self):
        """Test richcompare operator<()"""
        a = coords.angle(10)
        b = coords.angle(20)
        self.assertTrue(a < b)

    @unittest.skip('TODO boost wrap operator==()')
    def test_angle1_lt_angle2(self):
        """Test richcompare operator<()"""
        a = coords.angle(10)
        b = coords.angle(20)
        self.assertFalse(b < a)

    @unittest.skip('TODO boost wrap operator==()')
    def test_angle1_le_angle1(self):
        """Test richcompare operator<=()"""
        a = coords.angle(10)
        b = coords.angle(10)
        self.assertTrue(a <= b)

    @unittest.skip('TODO boost wrap operator==()')
    def test_angle1_le_angle2(self):
        """Test richcompare operator<=()"""
        a = coords.angle(20)
        b = coords.angle(20.6)
        self.assertFalse(b <= a)

    @unittest.skip('TODO boost wrap operator==()')
    def test_angle1_eq_angle1(self):
        """Test richcompare operator==()"""
        an_angle = coords.angle(1)
        another_angle = coords.angle(1)
        self.assertTrue(an_angle == another_angle)

    @unittest.skip('TODO boost wrap operator==()')
    def test_angle1_eq_angle2(self):
        """Test richcompare operator==()"""
        an_angle = coords.angle(1)
        another_angle = coords.angle(-1)
        self.assertFalse(an_angle == another_angle)

    @unittest.skip('TODO boost wrap operator!=()')
    def test_angle1_ne_angle1(self):
        """Test richcompare operator!=()"""
        an_angle = coords.angle(1)
        another_angle = coords.angle(1)
        self.assertFalse(an_angle != another_angle)

    @unittest.skip('TODO boost wrap operator!=()')
    def test_angle1_ne_angle2(self):
        """Test richcompare operator==()"""
        an_angle = coords.angle(1)
        another_angle = coords.angle(-1)
        self.assertTrue(an_angle != another_angle)

    @unittest.skip('TODO boost wrap operator!=()')
    def test_angle1_gt_angle1(self):
        """Test richcompare operato>()"""
        a = coords.angle(30)
        b = coords.angle(20)
        self.assertTrue(a > b)

    @unittest.skip('TODO boost wrap operator!=()')
    def test_angle1_gt_angle2(self):
        """Test richcompare operator>()"""
        a = coords.angle(30)
        b = coords.angle(20)
        self.assertFalse(b > a)

    @unittest.skip('TODO boost wrap operator!=()')
    def test_angle1_ge_angle1(self):
        """Test richcompare operator>=()"""
        a = coords.angle(10)
        b = coords.angle(10)
        self.assertTrue(a >= b)

    @unittest.skip('TODO boost wrap operator!=()')
    def test_angle1_ge_angle2(self):
        """Test richcompare operator>=()"""
        a = coords.angle(20.9)
        b = coords.angle(20.6)
        self.assertFalse(b >= a)

    # constructors


    def test_copy_constructor(self):
        """Test copy constructor"""
        an_angle = coords.angle(self.rd1)
        another_angle = coords.angle(an_angle)
        self.assertAlmostEqual(self.rd1, another_angle.value, self.places)

    def test_copy_assign(self):
        """Test copy assign
        This is a Python reference copy not a deep copy."""
        an_angle = coords.angle(self.rd1)
        another_angle = coords.angle()
        another_angle = an_angle
        self.assertAlmostEqual(self.rd1, another_angle.value, self.places)

    @unittest.skip('TODO Not available in boost, or manual')
    def test_deep_copy(self):
        """Test deep copy"""
        an_angle = coords.angle(self.rd1)
        another_angle = copy.deepcopy(an_angle)
        self.assertAlmostEqual(self.rd1, another_angle.value, self.places)

    def test_default_constructor(self):
        """Test default constructor"""
        an_angle = coords.angle()
        self.assertEqual(0, an_angle.value)


    def test_construct_degrees(self):
        """Test construct_degrees"""
        an_angle = coords.angle(self.rd1)
        self.assertAlmostEqual(self.rd1, an_angle.value, self.places)


    def test_construct_degrees_minutes(self):
        """Test construct degrees minutes"""
        an_angle = coords.angle(-45.0, 30.0)
        self.assertEqual(-45.5, an_angle.value)


    def test_construct_degrees_minutes_seconds(self):
        """Test construct degrees minutes seconds"""
        an_angle = coords.angle(-44, 59.0, 60)
        self.assertEqual(-45.0, an_angle.value)


    def test_mixed_sign_constructor_1(self):
        """Test mixed sign constructor 1"""
        # divide 2 to keep in range
        a = coords.angle(-self.rd1/2, self.rm1, self.rs1)
        b = coords.angle(-self.rd1/2, -self.rm1, self.rs1)
        self.assertAlmostEqual(a.value, b.value, self.places)


    def test_mixed_sign_constructor_2(self):
        """Test mixed sign constructor 2"""
        a = coords.angle(-self.rd1/2, self.rm1, self.rs1)
        b = coords.angle(-self.rd1/2, self.rm1, -self.rs1)
        self.assertAlmostEqual(a.value, b.value, self.places)

    def test_mixed_sign_constructor_3(self):
        """Test mixed sign constructor 3"""
        a = coords.angle(-self.rd1/2, self.rm1, self.rs1)
        b = coords.angle(-self.rd1/2, -self.rm1, -self.rs1)
        self.assertAlmostEqual(a.value, b.value, self.places)

    # accessors

    def test_accessors_1(self):
        """Test value accessors 1"""
        an_angle = coords.angle()
        an_angle.value = 90
        self.assertEqual(90, an_angle.value)
        self.assertEqual(math.pi/2.0, an_angle.radians)

    def test_accessors_2(self):
        """Test radians accessor negative"""
        an_angle = coords.angle()
        an_angle.radians = -math.pi/2.0
        self.assertEqual(-90, an_angle.value)
        self.assertEqual(-math.pi/2.0, an_angle.radians)

    # operators

    # add

    def test_inplace_add(self):
        """Test angle += angle"""
        a1 = coords.angle(self.rd1)
        a2 = coords.angle(self.rd2)
        a1 += a2
        self.assertAlmostEqual(self.rd1 + self.rd2, a1.value, self.places)

    def test_angle_plus_angle(self):
        """Test angle + angle"""
        a1 = coords.angle(self.rd1)
        a2 = coords.angle(self.rd2)
        a3 = coords.angle()
        a3 = a1 + a2
        self.assertAlmostEqual(self.rd1 + self.rd2, a3.value, self.places)


    # subtract

    def test_inplace_subtract(self):
        """Test angle -= angle"""
        a1 = coords.angle(self.rd1)
        a2 = coords.angle(self.rd2)
        a1 -= a2
        self.assertAlmostEqual(self.rd1 - self.rd2, a1.value, self.places)

    def test_angle_minus_angle(self):
        """Test angle - angle"""
        a1 = coords.angle(self.rd1)
        a2 = coords.angle(self.rd2)
        a3 = coords.angle()
        a3 = a1 - a2
        self.assertAlmostEqual(self.rd1 - self.rd2, a3.value, self.places)


    @unittest.skip('TODO boost unitary minus?')
    def test_unitary_minus(self):
        """Test angle = -angle"""
        a1 = coords.angle(self.rd1)
        a2 = -a1
        self.assertAlmostEqual(a1.value, -a2.value, self.places)

    # multiply

    def test_inplace_multiply(self):
        """Test angle *= angle"""
        a1 = coords.angle(self.rd1)
        a2 = coords.angle(self.rd2)
        a1 *= a2
        self.assertAlmostEqual(self.rd1 * self.rd2, a1.value, self.places)


    def test_angle_times_angle(self):
        """Test angle * angle"""
        a1 = coords.angle(self.rd1)
        a2 = coords.angle(self.rd2)
        a3 = a1 * a2
        self.assertAlmostEqual(self.rd1 * self.rd2, a3.value, self.places)


    # divide

    def test_angle_inplace_divide_angle(self):
        """Test angle /= angle"""
        a1 = coords.angle(self.rd1)
        a2 = coords.angle(self.rd2)
        a1 /= a2
        self.assertAlmostEqual(self.rd1 / self.rd2, a1.value, self.places)


    def test_angle_divide_angle(self):
        """Test angle / angle"""
        a1 = coords.angle(self.rd1)
        a2 = coords.angle(self.rd2)
        a3 = a1 / a2
        self.assertAlmostEqual(self.rd1 / self.rd2, a3.value, self.places)


    def test_angle_divide_zero(self):
        """Test angle / 0"""
        a1 = coords.angle(self.rd1)
        a2 = coords.angle(0)
        self.assertRaises(RuntimeError, lambda a, b: a / b, a1, a2)


   # strings

    def test_str(self):
        """Test str"""
        an_angle = coords.angle()
        a_str = '''0* 0' 0"'''
        self.assertEqual(a_str, str(an_angle))


    @unittest.skip('Not available in boost')
    def test_repr(self):
        """Test repr"""
        an_angle = coords.angle()
        a_repr = '''0* 0' 0"'''
        self.assertEqual(a_repr, repr(an_angle))



# end angles test

# ---------------------
# ----- Cartesian -----
# ---------------------

class TestCartesian(unittest.TestCase):

    def setUp(self):

        """Set up test parameters."""

        self.places = 7 # precision

        # random test points

        self.lower_range = -1.0e3
        self.upper_range =  1.0e3

        self.p1 = coords.Cartesian(random.uniform(self.lower_range, self.upper_range),
                                   random.uniform(self.lower_range, self.upper_range),
                                   random.uniform(self.lower_range, self.upper_range))

        self.p2 = coords.Cartesian(random.uniform(self.lower_range, self.upper_range),
                                   random.uniform(self.lower_range, self.upper_range),
                                   random.uniform(self.lower_range, self.upper_range))


    def assertSpacesAreEqual(self, lhs_space, rhs_space):
        """Space assert helper method."""
        # TODO wrap operator==()
        self.assertAlmostEqual(lhs_space.x, rhs_space.x, places=self.places)
        self.assertAlmostEqual(lhs_space.y, rhs_space.y, places=self.places)
        self.assertAlmostEqual(lhs_space.z, rhs_space.z, places=self.places)

    # -----------------------------
    # ----- test constructors -----
    # -----------------------------

    def test_default_constructor(self):
        """Test default constructor"""
        a = coords.Cartesian()
        self.assertEqual(0, a.x)
        self.assertEqual(0, a.y)
        self.assertEqual(0, a.z)
        self.assertSpacesAreEqual(coords.Cartesian.Uo, a)


    def test_x_constructor(self):
        """Test x constructor"""
        a = coords.Cartesian(self.p1.x)
        self.assertEqual(self.p1.x, a.x)
        self.assertEqual(0, a.y)
        self.assertEqual(0, a.z)


    def test_xy_constructor(self):
        """Test xy constructor"""
        a = coords.Cartesian(self.p1.x, self.p1.y)
        self.assertEqual(self.p1.x, a.x)
        self.assertEqual(self.p1.y, a.y)
        self.assertEqual(0, a.z)


    def test_xyz_constructor(self):
        """Test xyz constructor"""
        a = coords.Cartesian(self.p1.x, self.p1.y, self.p1.z)
        self.assertSpacesAreEqual(self.p1, a)


    @unittest.skip('Not available in boost')
    def test_xyz_constructor2(self):
        """Test xyz constructor by named args"""
        a = coords.Cartesian(z=self.p1.z, x=self.p1.x, y=self.p1.y)
        self.assertSpacesAreEqual(self.p1, a)


    def test_xyz_assignments(self):
        """Test space xyz assignment operators"""
        a = coords.Cartesian()
        a.x = self.p1.x
        a.y = self.p1.y
        a.z = self.p1.z
        self.assertSpacesAreEqual(self.p1, a)


    def test_copy_constructor_1(self):
        """Test copy constructor"""
        a = coords.Cartesian(self.p1)
        self.assertSpacesAreEqual(self.p1, a)


    def test_copy_assign_1(self):
        """Test copy assignment operator.
        This is a Python reference copy not a deep copy."""
        a = self.p1
        self.assertSpacesAreEqual(self.p1, a)


    def test_copy_assign_2(self):
        """Test copy assignment operator is shallow"""
        a = coords.Cartesian(1,2,3)
        b = a
        b.x = 4.0
        self.assertSpacesAreEqual(a, b)
        self.assertEqual(4, b.x)

    @unittest.skip('Not available in boost')
    def test_copy_assign_3(self):
        """Test copy assignment operator can use deep copy"""
        import copy
        a = coords.Cartesian(1,2,3)
        b = copy.deepcopy(a)
        b.x = 4.0
        self.assertEqual(1, a.x)
        self.assertEqual(4, b.x)


    def test_string_constructor_exception(self):
        """Test string constructor exception"""

        self.assertRaises(TypeError, lambda a: coords.Cartesian(a), 'some_string')
        self.assertRaises(TypeError, lambda a: coords.Cartesian(1, a), 'some_string')
        self.assertRaises(TypeError, lambda a: coords.Cartesian(1, -1, a), 'some_string')


    def test_string_assignment_exception(self):
        """Test string assignment exception"""

        self.assertRaises(TypeError, self.p1.x, 'some_string')
        self.assertRaises(TypeError, self.p1.y, 'some_string')
        self.assertRaises(TypeError, self.p1.z, 'some_string')


    def test_construct_from_spherical(self):
        """Test spherical conversion constructor"""
        a = coords.spherical(1, coords.angle(45), coords.angle(45))
        b = coords.Cartesian(a)
        self.assertAlmostEqual(0.5, b.x)
        self.assertAlmostEqual(0.5, b.y)
        self.assertAlmostEqual(math.sin(coords.angle.deg2rad(45)), b.z)

    # --------------------------------
    # ----- test unitary methods -----
    # --------------------------------

    def test_str(self):
        """Test str"""
        # precision is controlled by boost and not always passing the random number test
        a_point = coords.Cartesian(1.23, -4.56, 7.89)
        a_str = '<Cartesian><x>1.23</x><y>-4.56</y><z>7.89</z></Cartesian>'
        self.assertEqual(a_str, str(a_point))


    @unittest.skip('Not available in boost')
    def test_repr(self):
        """Test repr"""

        # %s precision is controlled by space.cpp and designed to match this test
        a_repr = '(%(x)s, %(y)s, %(z)s)' % {'x':self.p1.x,
                                            'y':self.p1.y,
                                            'z':self.p1.z}
        self.assertEqual(a_repr, repr(self.p1))


    def test_magnitude(self):
        """Test space magnitude"""
        root_sum_square = math.sqrt(self.p1.x*self.p1.x + self.p1.y*self.p1.y + self.p1.z*self.p1.z)
        a = coords.Cartesian.magnitude(self.p1)
        self.assertAlmostEqual(root_sum_square, a, self.places)


    def test_normalized(self):
        """Test space normalized"""
        root_sum_square = math.sqrt(self.p1.x*self.p1.x + self.p1.y*self.p1.y + self.p1.z*self.p1.z)
        normalized = coords.Cartesian(self.p1.x/root_sum_square,
                                      self.p1.y/root_sum_square,
                                      self.p1.z/root_sum_square)

        a = coords.Cartesian.normalized(self.p1)
        self.assertSpacesAreEqual(normalized, a)

    # ----------------------------
    # ----- test richcompare -----
    # ----------------------------

    @unittest.skip('TODO boost wrap operator==()')
    def test_space_eq_space(self):
        """Test space == space"""
        result = coords.Cartesian(self.p1.x + self.p2.x,
                                  self.p1.y + self.p2.y,
                                  self.p1.z + self.p2.z)
        a = self.p1 + self.p2
        self.assertTrue(result == a)


    @unittest.skip('TODO boost wrap operator==()')
    def test_space_ne_space(self):
        """Test space != space"""
        a = coords.Cartesian(1, 2, 3)
        b = coords.Cartesian(-1, 2, 3)
        self.assertTrue(a != b) # True because comparing memory addresses not value.


    @unittest.skip('TODO boost wrap operator==()')
    def test_space_eq_space1(self):
        """Test space == space"""
        a = coords.Cartesian(1, 2, 3)
        b = coords.Cartesian(1, 2, 3)
        self.assertTrue(a == b)


    @unittest.skip('TODO boost wrap operator==()')
    def test_space_eq_space2(self):
        """Test space == space"""
        a = coords.Cartesian(1, 2, 3)
        b = coords.Cartesian(-1, 2, 3)
        self.assertFalse(a == b)


    @unittest.skip('TODO boost wrap operator!=()')
    def test_space_ne_space1(self):
        """Test space != space"""
        a = coords.Cartesian(1, 2, 3)
        b = coords.Cartesian(-1, 2, 3)
        self.assertTrue(a != b)


    @unittest.skip('TODO boost wrap operator!=()')
    def test_space_ne_space2(self):
        """Test space != space"""
        a = coords.Cartesian(1, 2, 3)
        b = coords.Cartesian(1, 2, 3)
        self.assertFalse(a != b)


    @unittest.skip('TODO boost wrap richcompare operators')
    def test_space_noop_richcompare_space(self):
        """Test space >, >=, <, <= space"""
        a = coords.Cartesian(1, 2, 3)
        b = coords.Cartesian(4, 5, 5)
        self.assertRaises(TypeError, lambda a, b: a > b)
        self.assertRaises(TypeError, lambda a, b: a >= b)
        self.assertRaises(TypeError, lambda a, b: a < b)
        self.assertRaises(TypeError, lambda a, b: a <= b)

    # -------------------------------
    # ----- test math operators -----
    # -------------------------------

    def test_space_plus_space(self):
        """Test space + space"""
        result = coords.Cartesian(self.p1.x + self.p2.x,
                                  self.p1.y + self.p2.y,
                                  self.p1.z + self.p2.z)
        a = self.p1 + self.p2
        self.assertSpacesAreEqual(result, a)


    def test_inplace_add(self):
        """Test space +="""
        result = coords.Cartesian(self.p1.x + self.p2.x,
                                  self.p1.y + self.p2.y,
                                  self.p1.z + self.p2.z)
        a = self.p1
        a += self.p2
        self.assertSpacesAreEqual(result, a)


    def test_space_plus_double(self):
        """Test space + double"""
        # no python conversion constructor for this implementation of space
        self.assertRaises(TypeError, lambda: self.p1 + self.p2.x)


    def test_space_minus_space(self):
        """Test space - space"""
        result = coords.Cartesian(self.p1.x - self.p2.x,
                                  self.p1.y - self.p2.y,
                                  self.p1.z - self.p2.z)
        a = self.p1 - self.p2
        self.assertSpacesAreEqual(result, a)


    def test_inplace_subtract(self):
        """Test space -="""
        result = coords.Cartesian(self.p1.x - self.p2.x,
                                  self.p1.y - self.p2.y,
                                  self.p1.z - self.p2.z)
        a = self.p1
        a -= self.p2
        self.assertSpacesAreEqual(result, a)



    @unittest.skip('TODO boost unitary minus?')
    def test_unitary_minus(self):
        """Test space = -space"""
        result = coords.Cartesian(-self.p1.x,
                                  -self.p1.y,
                                  -self.p1.z)
        a = -self.p1
        self.assertTrue(result == a)


    def test_space_minus_double(self):
        """Test space - double"""
        # no python conversion constructor for this implementation of space
        self.assertRaises(TypeError, lambda: self.p1 - self.p2.x)


    def test_space_times_double_0(self):
        """Test space * double (scale)"""
        result = coords.Cartesian(0.5, 0.5, 0.5)
        a = coords.Cartesian(1,1,1) # positive to avoid unitary minus problem
        scale = 0.5
        a *= scale
        self.assertSpacesAreEqual(result, a)


    def test_space_times_double_1(self):
        """Test space * double (scale)"""
        result = coords.Cartesian(0.5, 0.5, 0.5)
        a = coords.Cartesian(1,1,1) # positive to avoid unitary minus problem
        scale = 0.5
        a = a * scale
        self.assertSpacesAreEqual(result, a)


    def test_double_times_space_2(self):
        """Test space * double (scale)"""
        result = coords.Cartesian(0.5, 0.5, 0.5)
        a = coords.Cartesian(1,1,1) # positive to avoid unitary minus problem
        scale = 0.5
        a = scale * a
        self.assertSpacesAreEqual(result, a)


    def test_space_times_space(self):
        """Test space * space dot product"""
        result = self.p1.x * self.p2.x + self.p1.y * self.p2.y + self.p1.z * self.p2.z
        a = self.p1 * self.p2
        self.assertAlmostEqual(result, a, self.places)


    def test_inplace_multiply(self):
        """Test space *= dot product"""
        result = self.p1.x * self.p2.x + self.p1.y * self.p2.y + self.p1.z * self.p2.z
        a = self.p1
        a *= self.p2
        self.assertAlmostEqual(result, a, self.places)


    def test_dot_product(self):
        """Test space dot product function"""
        result = self.p1.x * self.p2.x + self.p1.y * self.p2.y + self.p1.z * self.p2.z
        a = coords.dot(self.p1, self.p2)
        self.assertEqual(result, a)


    def test_x_cross_y(self):
        """Test x cross y is z"""
        a = coords.cross(coords.Cartesian.Ux, coords.Cartesian.Uy)
        self.assertSpacesAreEqual(coords.Cartesian.Uz, a)


    def test_y_cross_z(self):
        """Test x cross y is z"""
        a = coords.cross(coords.Cartesian.Uy, coords.Cartesian.Uz)
        self.assertSpacesAreEqual(coords.Cartesian.Ux, a)


    def test_z_cross_x(self):
        """Test x cross y is z"""
        a = coords.cross(coords.Cartesian.Uz, coords.Cartesian.Ux)
        self.assertSpacesAreEqual(coords.Cartesian.Uy, a)


    def test_cross_1(self):
        """Test more arbitrary cross product"""
        a = coords.Cartesian(1, 1, 1)
        b = coords.Cartesian(0, 0, 0.5)
        c = coords.cross(a, b)
        self.assertSpacesAreEqual(coords.Cartesian(0.5, -0.5, 0), c)


    def test_divide(self):
        """Test divide (scale)"""
        result = coords.Cartesian(self.p1.x / 2.0,
                                  self.p1.y / 2.0,
                                  self.p1.z / 2.0)
        a = self.p1 / 2.0
        self.assertSpacesAreEqual(result, a)


    def test_inplace_divide(self):
        """Test inplace divide (scale)"""
        result = coords.Cartesian(self.p1.x / 2.0,
                                  self.p1.y / 2.0,
                                  self.p1.z / 2.0)
        a = self.p1
        a /= 2.0
        self.assertSpacesAreEqual(result, a)


    def test_divide_by_zero(self):
        """Test space / 0"""
        a1 = self.p1
        self.assertRaises(RuntimeError, lambda a: a / 0, a1)


# end Cartesian


# ---------------------
# ----- spherical -----
# ---------------------

class Testspherical(unittest.TestCase):

    def setUp(self):

        """Set up test parameters."""

        self.places = 7 # precision

        # random test points

        self.lower_range = -1.0e3
        self.upper_range =  1.0e3

        self.p1 = coords.spherical(random.uniform(self.lower_range, self.upper_range),
                                   coords.angle(random.uniform(self.lower_range, self.upper_range)),
                                   coords.angle(random.uniform(self.lower_range, self.upper_range)))

        self.p2 = coords.spherical(random.uniform(self.lower_range, self.upper_range),
                                   coords.angle(random.uniform(self.lower_range, self.upper_range)),
                                   coords.angle(random.uniform(self.lower_range, self.upper_range)))


    def assertSpacesAreEqual(self, lhs_space, rhs_space):
        """Space assert helper method."""
        # TODO wrap operator==()
        self.assertAlmostEqual(lhs_space.r, rhs_space.r, places=self.places)
        self.assertAlmostEqual(lhs_space.theta.value, rhs_space.theta.value, places=self.places)
        self.assertAlmostEqual(lhs_space.phi.value, rhs_space.phi.value, places=self.places)

    # -----------------------------
    # ----- test constructors -----
    # -----------------------------

    def test_default_constructor(self):
        """Test default constructor"""
        a = coords.spherical()
        self.assertEqual(0, a.r)
        self.assertEqual(0, a.theta.value)
        self.assertEqual(0, a.phi.value)


    def test_r_constructor(self):
        """Test r constructor"""
        a = coords.spherical(self.p1.r)
        self.assertEqual(self.p1.r, a.r)
        self.assertEqual(0, a.theta.value)
        self.assertEqual(0, a.phi.value)


    def test_r_theta_constructor(self):
        """Test r theta constructor"""
        a = coords.spherical(self.p1.r, self.p1.theta)
        self.assertEqual(self.p1.r, a.r)
        self.assertEqual(self.p1.theta.value, a.theta.value)
        self.assertEqual(0, a.phi.value)


    def test_r_theta_phi_constructor(self):
        """Test r theta phi constructor"""
        a = coords.spherical(self.p1.r, self.p1.theta, self.p1.phi)
        self.assertSpacesAreEqual(self.p1, a)


    @unittest.skip('Not available in boost')
    def test_r_theta_phi_constructor2(self):
        """Test r theta phi constructor by named args"""
        a = coords.spherical(z=self.p1.phi, x=self.p1.r, y=self.p1.theta)
        self.assertSpacesAreEqual(self.p1, a)


    def test_r_theta_phi_assignments(self):
        """Test space r theta phi assignment operators"""
        a = coords.spherical()
        a.r = self.p1.r
        a.theta = self.p1.theta
        a.phi = self.p1.phi
        self.assertSpacesAreEqual(self.p1, a)


    def test_copy_constructor_1(self):
        """Test copy constructor"""
        a = coords.spherical(self.p1)
        self.assertSpacesAreEqual(self.p1, a)

    def test_copy_assign_1(self):
        """Test copy assignment operator.
        This is a Python reference copy not a deep copy."""
        a = self.p1
        self.assertSpacesAreEqual(self.p1, a)

    def test_copy_assign_2(self):
        """Test copy assignment operator is shallow"""
        a = coords.spherical(1, coords.angle(2), coords.angle(3))
        b = a
        b.r = 4.0
        self.assertSpacesAreEqual(a, b)
        self.assertEqual(4, b.r)

    @unittest.skip('Not available in boost')
    def test_copy_assign_3(self):
        """Test copy assignment operator can use deep copy"""
        import copy
        a = coords.spherical(1,2,3)
        b = copy.deepcopy(a)
        b.r = 4.0
        self.assertEqual(1, a.r)
        self.assertEqual(4, b.r)

    def test_string_constructor_exception(self):
        """Test string constructor exception"""

        self.assertRaises(TypeError, lambda a: coords.spherical(a), 'some_string')
        self.assertRaises(TypeError, lambda a: coords.spherical(1, a), 'some_string')
        self.assertRaises(TypeError, lambda a: coords.spherical(1, -1, a), 'some_string')

    def test_string_assignment_exception(self):
        """Test string assignment exception"""

        self.assertRaises(TypeError, self.p1.r, 'some_string')
        self.assertRaises(TypeError, self.p1.theta, 'some_string')
        self.assertRaises(TypeError, self.p1.phi, 'some_string')

    def test_construct_from_cartesian(self):
        """Test cartesian conversion constructor"""
        a = coords.Cartesian(1, 1, 1)
        b = coords.spherical(a)
        self.assertAlmostEqual(math.sqrt(2 + 1), b.r) # xy = sqrt(2), z = 1
        self.assertAlmostEqual(math.atan(math.sqrt(2)/1), b.theta.radians)
        self.assertAlmostEqual(45, b.phi.value)

    # --------------------------------
    # ----- test unitary methods -----
    # --------------------------------

    def test_str(self):
        """Test str"""
        # precision is controlled by boost and not always passing the random number test
        a_point = coords.spherical(1.23, coords.angle(-4.56), coords.angle(7.89))
        a_str = '<spherical><r>1.23</r><theta>-4.56</theta><phi>7.89</phi></spherical>'
        self.assertEqual(a_str, str(a_point))


    @unittest.skip('Not available in boost')
    def test_repr(self):
        """Test repr"""

        # %s precision is controlled by space.cpp and designed to match this test
        a_repr = '(%(x)s, %(y)s, %(z)s)' % {'x':self.p1.r,
                                            'y':self.p1.theta,
                                            'z':self.p1.phi}
        self.assertEqual(a_repr, repr(self.p1))


    # ----------------------------
    # ----- test richcompare -----
    # ----------------------------

    @unittest.skip('TODO boost wrap operator==()')
    def test_space_eq_space(self):
        """Test space == space"""
        result = coords.spherical(self.p1.r + self.p2.r,
                                  self.p1.theta + self.p2.theta,
                                  self.p1.phi + self.p2.phi)
        a = self.p1 + self.p2
        self.assertTrue(result == a)


    @unittest.skip('TODO boost wrap operator==()')
    def test_space_ne_space(self):
        """Test space != space"""
        a = coords.spherical(1, 2, 3)
        b = coords.spherical(-1, 2, 3)
        self.assertTrue(a != b) # True because comparing memory addresses not value.


    @unittest.skip('TODO boost wrap operator==()')
    def test_space_eq_space1(self):
        """Test space == space"""
        a = coords.spherical(1, 2, 3)
        b = coords.spherical(1, 2, 3)
        self.assertTrue(a == b)


    @unittest.skip('TODO boost wrap operator==()')
    def test_space_eq_space2(self):
        """Test space == space"""
        a = coords.spherical(1, 2, 3)
        b = coords.spherical(-1, 2, 3)
        self.assertFalse(a == b)


    @unittest.skip('TODO boost wrap operator!=()')
    def test_space_ne_space1(self):
        """Test space != space"""
        a = coords.spherical(1, 2, 3)
        b = coords.spherical(-1, 2, 3)
        self.assertTrue(a != b)


    @unittest.skip('TODO boost wrap operator!=()')
    def test_space_ne_space2(self):
        """Test space != space"""
        a = coords.spherical(1, 2, 3)
        b = coords.spherical(1, 2, 3)
        self.assertFalse(a != b)


    @unittest.skip('TODO boost wrap richcompare operators')
    def test_space_noop_richcompare_space(self):
        """Test space >, >=, <, <= space"""
        a = coords.spherical(1, 2, 3)
        b = coords.spherical(4, 5, 5)
        self.assertRaises(TypeError, lambda a, b: a > b)
        self.assertRaises(TypeError, lambda a, b: a >= b)
        self.assertRaises(TypeError, lambda a, b: a < b)
        self.assertRaises(TypeError, lambda a, b: a <= b)

    # -------------------------------
    # ----- test math operators -----
    # -------------------------------

    def test_space_plus_space(self):
        """Test space + space"""
        r1 = coords.Cartesian(self.p1)
        r2 = coords.Cartesian(self.p2)
        r12 = r1 + r2
        result = coords.spherical(r12)

        a = self.p1 + self.p2

        self.assertSpacesAreEqual(result, a)


    def test_inplace_add(self):
        """Test space +="""

        r1 = coords.Cartesian(self.p1)
        r2 = coords.Cartesian(self.p2)
        r12 = r1 + r2
        result = coords.spherical(r12)

        a = self.p1
        a += self.p2

        self.assertSpacesAreEqual(result, a)


    def test_space_plus_double(self):
        """Test space + double"""
        # no python conversion constructor for this implementation of space
        self.assertRaises(TypeError, lambda: self.p1 + self.p2.r)


    def test_space_minus_space(self):
        """Test space - space"""
        r1 = coords.Cartesian(self.p1)
        r2 = coords.Cartesian(self.p2)
        r12 = r1 - r2
        result = coords.spherical(r12)

        a = self.p1 - self.p2

        self.assertSpacesAreEqual(result, a)


    def test_inplace_subtract(self):
        """Test space -="""
        r1 = coords.Cartesian(self.p1)
        r2 = coords.Cartesian(self.p2)
        r12 = r1 - r2
        result = coords.spherical(r12)

        a = self.p1
        a -= self.p2

        self.assertSpacesAreEqual(result, a)


    @unittest.skip('TODO boost unitary minus?')
    def test_unitary_minus(self):
        """Test space = -space"""
        result = coords.spherical(-self.p1.r,
                                  -self.p1.theta,
                                  -self.p1.phi)
        a = -self.p1
        self.assertTrue(result == a)


    def test_space_minus_double(self):
        """Test space - double"""
        # no python conversion constructor for this implementation of space
        self.assertRaises(TypeError, lambda: self.p1 - self.p2.r)


    def test_space_times_double_0(self):
        """Test space * double (scale)"""
        result = coords.spherical(0.5, coords.angle(1), coords.angle(1))
        a = coords.spherical(1, coords.angle(1), coords.angle(1)) # positive to avoid unitary minus problem
        scale = 0.5
        a *= scale
        self.assertSpacesAreEqual(result, a)


    def test_space_times_double_1(self):
        """Test space * double (scale)"""
        result = coords.spherical(0.5, coords.angle(1), coords.angle(1))
        a = coords.spherical(1, coords.angle(1), coords.angle(1)) # positive to avoid unitary minus problem
        scale = 0.5
        a = a * scale
        self.assertSpacesAreEqual(result, a)


    def test_double_times_space_2(self):
        """Test space * double (scale)"""
        result = coords.spherical(0.5, coords.angle(1), coords.angle(1))
        a = coords.spherical(1, coords.angle(1), coords.angle(1)) # positive to avoid unitary minus problem
        scale = 0.5
        a = scale * a
        self.assertSpacesAreEqual(result, a)


    @unittest.skip('TODO spherical dot product?')
    def test_space_times_space(self):
        """Test space * space dot product"""
        result = self.p1.r * self.p2.r + self.p1.theta * self.p2.theta + self.p1.phi * self.p2.phi
        a = self.p1 * self.p2
        self.assertAlmostEqual(result, a, self.places)


    @unittest.skip('TODO spherical dot product?')
    def test_inplace_multiply(self):
        """Test space *= dot product"""
        result = self.p1.r * self.p2.r + self.p1.theta * self.p2.theta + self.p1.phi * self.p2.phi
        a = self.p1
        a *= self.p2
        self.assertAlmostEqual(result, a, self.places)


    @unittest.skip('TODO spherical cross product?')
    def test_dot_product(self):
        """Test space dot product function"""
        result = self.p1.r * self.p2.r + self.p1.theta * self.p2.theta + self.p1.phi * self.p2.phi
        a = coords.dot(self.p1, self.p2)
        self.assertEqual(result, a)


    @unittest.skip('TODO spherical cross product?')
    def test_x_cross_y(self):
        """Test x cross y is z"""
        a = coords.cross(coords.spherical.Ux, coords.spherical.Uy)
        self.assertSpacesAreEqual(coords.spherical.Uz, a)


    @unittest.skip('TODO spherical cross product?')
    def test_y_cross_z(self):
        """Test x cross y is z"""
        a = coords.cross(coords.spherical.Uy, coords.spherical.Uz)
        self.assertSpacesAreEqual(coords.spherical.Ux, a)


    @unittest.skip('TODO spherical cross product?')
    def test_z_cross_x(self):
        """Test x cross y is z"""
        a = coords.cross(coords.spherical.Uz, coords.spherical.Ux)
        self.assertSpacesAreEqual(coords.spherical.Uy, a)


    @unittest.skip('TODO spherical cross product?')
    def test_cross_1(self):
        """Test more arbitrary cross product"""
        a = coords.spherical(1, 1, 1)
        b = coords.spherical(0, 0, 0.5)
        c = coords.cross(a, b)
        self.assertSpacesAreEqual(coords.spherical(0.5, -0.5, 0), c)


    def test_divide(self):
        """Test divide (scale)"""
        result = coords.spherical(self.p1.r / 2.0,
                                  coords.angle(self.p1.theta.value),
                                  coords.angle(self.p1.phi.value))
        a = self.p1 / 2.0
        self.assertSpacesAreEqual(result, a)


    def test_inplace_divide(self):
        """Test inplace divide (scale)"""
        result = coords.spherical(self.p1.r / 2.0,
                                  coords.angle(self.p1.theta.value),
                                  coords.angle(self.p1.phi.value))
        a = self.p1
        a /= 2.0
        self.assertSpacesAreEqual(result, a)


    def test_divide_by_zero(self):
        """Test space / 0"""
        a1 = self.p1
        self.assertRaises(RuntimeError, lambda a: a / 0, a1)


# end spherical


if __name__ == '__main__':
    random.seed(time.time())
    unittest.main()
