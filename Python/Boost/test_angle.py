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



    @unittest.skip('TODO')
    def test_string_constructor_exception(self):
        """Test string constructor exception"""

        self.assertRaises(ArgumentError, lambda a: coords.angle(a), 'some_string')
        self.assertRaises(ArgumentError, lambda a: coords.angle(1, a), 'some_string')
        self.assertRaises(ArgumentError, lambda a: coords.angle(1, -1, a), 'some_string')


    # accessors

    def test_accessors_1(self):
        """Test value accessors 1"""
        an_angle = coords.angle()
        an_angle.value = 90
        self.assertEqual(90, an_angle.value)
        self.assertEqual(math.pi/2.0, an_angle.radians)

    @unittest.skip('TODO Boost.Python.ArgumentError')
    def test_accessors_1b(self):
        """Test value accessor exception"""
        try:
            an_angle = coords.angle()
            an_angle.value = 'foo'
            self.assertRaises(False) # correct behavior skips this
        except RuntimeError, err:
            self.assertTrue(RuntimeError == type(err))


    def test_accessors_2(self):
        """Test radians accessor negative"""
        an_angle = coords.angle()
        an_angle.radians = -math.pi/2.0
        self.assertEqual(-90, an_angle.value)
        self.assertEqual(-math.pi/2.0, an_angle.radians)

    @unittest.skip('TODO Boost.Python.ArgumentError')
    def test_accessors_2a(self):
        """Test radians accessor exception"""
        try:
            an_angle = coords.angle()
            an_angle.radians = 'foo'
            self.assertRaises(False) # correct behavior skips this
        except RuntimeError, err:
            self.assertTrue(RuntimeError == type(err))

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
        # Note: different from Manual catching coords.Error

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


if __name__ == '__main__':
    random.seed(time.time())
    unittest.main()
