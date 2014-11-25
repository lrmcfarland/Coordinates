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
        self.assertSpacesAreEqual(coords.Uo, a)


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


    def test_string_constructor_exception(self):
        """Test string constructor exception"""

        self.assertRaises(coords.Error, lambda a: coords.Cartesian(a), 'some_string')
        self.assertRaises(coords.Error, lambda a: coords.Cartesian(1, a), 'some_string')
        self.assertRaises(coords.Error, lambda a: coords.Cartesian(1, -1, a), 'some_string')


    def test_string_assignment_exception(self):
        """Test string assignment exception"""

        self.assertRaises(TypeError, self.p1.x, 'some_string')
        self.assertRaises(TypeError, self.p1.y, 'some_string')
        self.assertRaises(TypeError, self.p1.z, 'some_string')


    @unittest.skip('TODO conversion')
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
        a = coords.Cartesian().magnitude(self.p1)
        self.assertAlmostEqual(root_sum_square, a, self.places)


    def test_normalized(self):
        """Test space normalized"""
        root_sum_square = math.sqrt(self.p1.x*self.p1.x + self.p1.y*self.p1.y + self.p1.z*self.p1.z)
        normalized = coords.Cartesian(self.p1.x/root_sum_square,
                                      self.p1.y/root_sum_square,
                                      self.p1.z/root_sum_square)

        a = coords.Cartesian().normalized(self.p1)
        self.assertSpacesAreEqual(normalized, a)

    # ----------------------------
    # ----- test richcompare -----
    # ----------------------------

    def test_space_eq_space(self):
        """Test space == space"""
        result = coords.Cartesian(self.p1.x, self.p1.y, self.p1.z)
        a = coords.Cartesian(self.p1.x, self.p1.y, self.p1.z)
        self.assertTrue(result == a)


    def test_space_ne_space(self):
        """Test space != space"""
        a = coords.Cartesian(1, 2, 3)
        b = coords.Cartesian(-1, 2, 3)
        self.assertTrue(a != b) # True because comparing memory addresses not value.


    def test_space_eq_space1(self):
        """Test space == space"""
        a = coords.Cartesian(1, 2, 3)
        b = coords.Cartesian(1, 2, 3)
        self.assertTrue(a == b)


    def test_space_eq_space2(self):
        """Test space == space"""
        a = coords.Cartesian(1, 2, 3)
        b = coords.Cartesian(-1, 2, 3)
        self.assertFalse(a == b)


    def test_space_ne_space1(self):
        """Test space != space"""
        a = coords.Cartesian(1, 2, 3)
        b = coords.Cartesian(-1, 2, 3)
        self.assertTrue(a != b)


    def test_space_ne_space2(self):
        """Test space != space"""
        a = coords.Cartesian(1, 2, 3)
        b = coords.Cartesian(1, 2, 3)
        self.assertFalse(a != b)


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
        # c++ explicit conversion constructor not definded for double
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


    def test_unitary_minus(self):
        """Test space = -space"""
        result = coords.Cartesian(-self.p1.x,
                                  -self.p1.y,
                                  -self.p1.z)
        a = -self.p1
        self.assertTrue(result == a)


    def test_space_minus_double(self):
        """Test space - double"""
        # c++ explicit conversion constructor not definded for double
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
        a = coords.Cartesian().dot(self.p1, self.p2)
        self.assertEqual(result, a)


    def test_x_cross_y(self):
        """Test x cross y is z"""
        a = coords.Cartesian().cross(coords.Ux, coords.Uy)
        self.assertSpacesAreEqual(coords.Uz, a)


    def test_y_cross_z(self):
        """Test x cross y is z"""
        a = coords.Cartesian().cross(coords.Uy, coords.Uz)
        self.assertSpacesAreEqual(coords.Ux, a)


    def test_z_cross_x(self):
        """Test x cross y is z"""
        a = coords.Cartesian().cross(coords.Uz, coords.Ux)
        self.assertSpacesAreEqual(coords.Uy, a)


    def test_cross_1(self):
        """Test more arbitrary cross product"""
        a = coords.Cartesian(1, 1, 1)
        b = coords.Cartesian(0, 0, 0.5)
        c = coords.Cartesian().cross(a, b)
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
        self.assertRaises(coords.Error, lambda a: a / 0, a1)
        # Note: different from Boost catching RuntimeError


if __name__ == '__main__':
    random.seed(time.time())
    unittest.main()
