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
