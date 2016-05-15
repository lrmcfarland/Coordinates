#!/usr/bin/env python

"""Unit tests for space objects."""

import math
import random
import time
import unittest

import coords


class TestSpace(unittest.TestCase):

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

    # -----------------------------
    # ----- test constructors -----
    # -----------------------------

    def test_default_constructor(self):
        """Test default constructor"""
        a = coords.Cartesian()
        self.assertEqual(0, a.x())
        self.assertEqual(0, a.y())
        self.assertEqual(0, a.z())
        # TODO self.assertTrue(coords.Cartesian.Uo == a)
        # TODO self.assertEqual(coords.Cartesian.Uo.x(), a.x())


    def test_x_constructor(self):
        """Test x constructor"""
        a = coords.Cartesian(self.p1.x())
        self.assertEqual(self.p1.x(), a.x())
        self.assertEqual(0, a.y())
        self.assertEqual(0, a.z())


    def test_xy_constructor(self):
        """Test xy constructor"""
        a = coords.Cartesian(self.p1.x(), self.p1.y())
        self.assertEqual(self.p1.x(), a.x())
        self.assertEqual(self.p1.y(), a.y())
        self.assertEqual(0, a.z())


    def test_xyz_constructor(self):
        """Test xyz constructor"""
        a = coords.Cartesian(self.p1.x(), self.p1.y(), self.p1.z())
        self.assertTrue(self.p1 == a)


    @unittest.skip('TODO keyword constructors')
    def test_xyz_constructor2(self):
        """Test xyz constructor by named args"""
        a = coords.Cartesian(z=self.p1.z, x=self.p1.x, y=self.p1.y)
        self.assertTrue(self.p1 == a)


    def test_xyz_assignments(self):
        """Test space xyz assignment operators"""
        a = coords.Cartesian()
        a.x(self.p1.x())
        a.y(self.p1.y())
        a.z(self.p1.z())
        self.assertTrue(self.p1 == a)


    @unittest.skip('TODO copy constructor')
    def test_copy_constructor_1(self):
        """Test copy constructor"""
        a = coords.Cartesian(self.p1)
        self.assertSpacesAreEqual(self.p1, a)


    def test_copy_assign_1(self):
        """Test copy assignment operator.
        This is a Python reference copy not a deep copy."""
        a = self.p1
        self.assertTrue(self.p1 == a)


    def test_copy_assign_2(self):
        """Test copy assignment operator is shallow"""
        a = coords.Cartesian(1,2,3)
        b = a
        b.x(4.0)
        self.assertTrue(a == b)
        self.assertEqual(4, b.x())

    @unittest.skip('deepcopy not available in swig')
    def test_copy_assign_3(self):
        """Test copy assignment operator can use deep copy"""
        import copy
        a = coords.Cartesian(1,2,3)
        b = copy.deepcopy(a)
        b.x = 4.0
        self.assertEqual(1, a.x)
        self.assertEqual(4, b.x)


    @unittest.skip('TODO needs space.i wrapper')
    def test_string_constructor_exception(self):
        """Test string constructor exception"""

        self.assertRaises(TypeError, lambda a: coords.Cartesian(a), 'some_string')
        self.assertRaises(TypeError, lambda a: coords.Cartesian(1, a), 'some_string')
        self.assertRaises(TypeError, lambda a: coords.Cartesian(1, -1, a), 'some_string')


    @unittest.skip('TODO needs space.i wrapper')
    def test_string_assignment_exception(self):
        """Test string assignment exception"""

        self.assertRaises(TypeError, self.p1.x, 'some_string')
        self.assertRaises(TypeError, self.p1.y, 'some_string')
        self.assertRaises(TypeError, self.p1.z, 'some_string')

    # --------------------------------
    # ----- test unitary methods -----
    # --------------------------------

    def test_str(self):
        """Test str"""

        # %s precision is controlled by space.cpp and designed to match this test
        a_str = '<Cartesian><x>%(x)s</x><y>%(y)s</y><z>%(z)s</z></Cartesian>' % {'x':self.p1.x(),
                                                                                 'y':self.p1.y(),
                                                                                 'z':self.p1.z()}
        self.assertEqual(a_str, str(self.p1))


    def test_repr(self):
        """Test repr"""

        # %s precision is controlled by Cartesian.cpp and designed to match this test
        a_repr = '(%(x)s, %(y)s, %(z)s)' % {'x':self.p1.x(),
                                            'y':self.p1.y(),
                                            'z':self.p1.z()}
        self.assertEqual(a_repr, repr(self.p1))


    def test_magnitude(self):
        """Test space magnitude"""
        root_sum_square = math.sqrt(self.p1.x()*self.p1.x() + \
                                    self.p1.y()*self.p1.y() + \
                                    self.p1.z()*self.p1.z())
        a = coords.Cartesian.magnitude(self.p1)
        self.assertAlmostEqual(root_sum_square, a, self.places)


    def test_normalized(self):
        """Test space normalized"""
        root_sum_square = math.sqrt(self.p1.x()*self.p1.x() + \
                                    self.p1.y()*self.p1.y() + \
                                    self.p1.z()*self.p1.z())

        normalized = coords.Cartesian(self.p1.x()/root_sum_square,
                                      self.p1.y()/root_sum_square,
                                      self.p1.z()/root_sum_square)

        a = coords.Cartesian.normalized(self.p1)
        self.assertTrue(normalized == a)


    # ----------------------------
    # ----- test richcompare -----
    # ----------------------------

    def test_space_eq_space(self):
        """Test space == space"""
        result = coords.Cartesian(self.p1.x() + self.p2.x(),
                                  self.p1.y() + self.p2.y(),
                                  self.p1.z() + self.p2.z())
        a = self.p1 + self.p2
        self.assertTrue(result == a)


    def test_space_ne_space(self):
        """Test space != space"""
        a = coords.Cartesian(1, 2, 3)
        b = coords.Cartesian(-1, 2, 3)
        self.assertTrue(a != b)


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
        result = coords.Cartesian(self.p1.x() + self.p2.x(),
                             self.p1.y() + self.p2.y(),
                             self.p1.z() + self.p2.z())
        a = self.p1 + self.p2
        self.assertTrue(result == a)


    def test_inplace_add(self):
        """Test space +="""
        result = coords.Cartesian(self.p1.x() + self.p2.x(),
                             self.p1.y() + self.p2.y(),
                             self.p1.z() + self.p2.z())
        a = self.p1
        a += self.p2
        self.assertTrue(result == a)


    def test_space_plus_double(self):
        """Test space + double"""
        # no python conversion constructor for this implementation of space
        self.assertRaises(TypeError, lambda a: self.p1 + self.p2.x)


    def test_space_minus_space(self):
        """Test space - space"""
        result = coords.Cartesian(self.p1.x() - self.p2.x(),
                             self.p1.y() - self.p2.y(),
                             self.p1.z() - self.p2.z())
        a = self.p1 - self.p2
        self.assertTrue(result == a)


    def test_inplace_subtract(self):
        """Test space -="""
        result = coords.Cartesian(self.p1.x() - self.p2.x(),
                             self.p1.y() - self.p2.y(),
                             self.p1.z() - self.p2.z())
        a = self.p1
        a -= self.p2
        self.assertTrue(result == a)


    @unittest.skip('TODO SWIG unitary minus?')
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
        self.assertRaises(TypeError, lambda a: self.p1 - self.p2.x)


    @unittest.skip('TODO implicit conversion constructor to make double space(x)?')
    def test_space_times_double(self):
        """Test space * double (scale)"""
        scale = 0.5
        result = self.p1.x * scale + self.p1.y * scale + self.p1.z * scale
        a = self.p1 * scale
        self.assertAlmostEqual(result, a, self.places)


    @unittest.skip('TODO needs space.i wrapper')
    def test_space_times_space(self):
        """Test space * space dot product"""
        result = self.p1.x() * self.p2.x() + self.p1.y() * self.p2.y() + self.p1.z() * self.p2.z()
        a = self.p1 * self.p2
        self.assertAlmostEqual(result, a, self.places)


    @unittest.skip('TODO needs space.i wrapper')
    def test_inplace_multiply(self):
        """Test space *= dot product"""
        result = self.p1.x() * self.p2.x() + self.p1.y() * self.p2.y() + self.p1.z() * self.p2.z()
        a = self.p1
        a *= self.p2
        self.assertAlmostEqual(result, a, self.places)


    def test_dot_product(self):
        """Test space dot product function"""
        result = self.p1.x() * self.p2.x() + self.p1.y() * self.p2.y() + self.p1.z() * self.p2.z()
        a = coords.dot(self.p1, self.p2)
        self.assertTrue(result == a)


    @unittest.skip('TODO space.i wrapper for static members Ux, Uy, Uz, Uo')
    def test_x_cross_y(self):
        """Test x cross y is z"""
        a = space.cross(space.Ux, space.Uy)
        self.assertTrue(space.Uz == a)


    @unittest.skip('TODO space.i wrapper for static members Ux, Uy, Uz, Uo')
    def test_y_cross_z(self):
        """Test x cross y is z"""
        a = space.cross(space.Uy, space.Uz)
        self.assertTrue(space.Ux == a)


    @unittest.skip('TODO space.i wrapper for static members Ux, Uy, Uz, Uo')
    def test_z_cross_x(self):
        """Test x cross y is z"""
        a = space.cross(space.Uz, space.Ux)
        self.assertTrue(space.Uy == a)

    def test_cross_1(self):
        """Test more arbitrary cross product"""
        a = coords.Cartesian(1, 1, 1)
        b = coords.Cartesian(0, 0, 0.5)
        c = coords.cross(a, b)
        self.assertTrue(coords.Cartesian(0.5, -0.5, 0) == c)

    @unittest.skip('TODO needs space.i wrapper')
    def test_divide(self):
        """Test divide (scale)"""
        result = coords.Cartesian(self.p1.x / 2.0,
                             self.p1.y / 2.0,
                             self.p1.z / 2.0)
        a = self.p1 / 2.0
        self.assertTrue(result == a)


    @unittest.skip('TODO needs space.i wrapper')
    def test_inplace_divide(self):
        """Test inplace divide (scale)"""
        result = coords.Cartesian(self.p1.x / 2.0,
                             self.p1.y / 2.0,
                             self.p1.z / 2.0)
        a = self.p1
        a /= 2.0
        self.assertTrue(result == a)


    @unittest.skip('TODO TypeError?')
    def test_divide_by_zero(self):
        """Test space / 0"""
        a1 = self.p1
        self.assertRaises(RuntimeError, lambda a: a / 0.0, a1)


if __name__ == '__main__':
    random.seed(time.time())
    unittest.main()
