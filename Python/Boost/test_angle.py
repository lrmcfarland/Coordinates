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

        return

    # static methods

    # Note these are different from python/Manual in that they are
    # instance methods not module methods.

    def test_deg2rad_1(self):
        """Test deg2rad"""
        self.assertAlmostEqual(math.pi/2.0, coords.angle.deg2rad(90), self.places)
        return

    def test_deg2rad_2(self):
        """Test deg2rad negative angle"""
        self.assertAlmostEqual(-math.pi, coords.angle.deg2rad(-180), self.places)
        return

    def test_rad2deg_1(self):
        """Test rad2deg 1"""
        self.assertAlmostEqual(270, coords.angle.rad2deg(3.0*math.pi/2.0), self.places)
        return


    def test_rad2deg_2(self):
        """Test rad2deg negative angle"""
        self.assertAlmostEqual(-180, coords.angle.rad2deg(-math.pi), self.places)
        return


    # booleans

    def test_angle1_lt_angle1(self):
        """Test richcompare operator<()"""
        a = coords.angle(10)
        b = coords.angle(20)
        self.assertTrue(a < b)
        return


    def test_angle1_lt_angle2(self):
        """Test richcompare operator<()"""
        a = coords.angle(10)
        b = coords.angle(20)
        self.assertFalse(b < a)
        return


    def test_angle1_le_angle1(self):
        """Test richcompare operator<=()"""
        a = coords.angle(10)
        b = coords.angle(10)
        self.assertTrue(a <= b)
        return


    def test_angle1_le_angle2(self):
        """Test richcompare operator<=()"""
        a = coords.angle(20)
        b = coords.angle(20.6)
        self.assertFalse(b <= a)
        return


    def test_angle1_eq_angle1(self):
        """Test richcompare operator==()"""
        an_angle = coords.angle(1)
        another_angle = coords.angle(1)
        self.assertTrue(an_angle == another_angle)
        return


    def test_angle1_eq_angle2(self):
        """Test richcompare operator==()"""
        an_angle = coords.angle(1)
        another_angle = coords.angle(-1)
        self.assertFalse(an_angle == another_angle)
        return


    def test_angle1_ne_angle1(self):
        """Test richcompare operator!=()"""
        an_angle = coords.angle(1)
        another_angle = coords.angle(1)
        self.assertFalse(an_angle != another_angle)
        return


    def test_angle1_ne_angle2(self):
        """Test richcompare operator==()"""
        an_angle = coords.angle(1)
        another_angle = coords.angle(-1)
        self.assertTrue(an_angle != another_angle)
        return


    def test_angle1_gt_angle1(self):
        """Test richcompare operato>()"""
        a = coords.angle(30)
        b = coords.angle(20)
        self.assertTrue(a > b)
        return


    def test_angle1_gt_angle2(self):
        """Test richcompare operator>()"""
        a = coords.angle(30)
        b = coords.angle(20)
        self.assertFalse(b > a)
        return


    def test_angle1_ge_angle1(self):
        """Test richcompare operator>=()"""
        a = coords.angle(10)
        b = coords.angle(10)
        self.assertTrue(a >= b)
        return


    def test_angle1_ge_angle2(self):
        """Test richcompare operator>=()"""
        a = coords.angle(20.9)
        b = coords.angle(20.6)
        self.assertFalse(b >= a)
        return


    # constructors


    def test_copy_constructor(self):
        """Test copy constructor"""
        an_angle = coords.angle(self.rd1)
        another_angle = coords.angle(an_angle)
        self.assertAlmostEqual(self.rd1, another_angle.degrees, self.places)
        return



    def test_copy_assign(self):
        """Test copy assign
        This is a Python reference copy not a deep copy."""
        an_angle = coords.angle(self.rd1)
        another_angle = coords.angle()
        another_angle = an_angle
        self.assertAlmostEqual(self.rd1, another_angle.degrees, self.places)
        return



    def test_default_constructor(self):
        """Test default constructor"""
        an_angle = coords.angle()
        self.assertEqual(0, an_angle.degrees)
        return



    def test_construct_degrees(self):
        """Test construct_degrees"""
        an_angle = coords.angle(self.rd1)
        self.assertAlmostEqual(self.rd1, an_angle.degrees, self.places)
        return


    def test_construct_degrees_minutes(self):
        """Test construct degrees minutes"""
        an_angle = coords.angle(-45.0, 30.0)
        self.assertEqual(-45.5, an_angle.degrees)
        return


    def test_construct_degrees_minutes_seconds(self):
        """Test construct degrees minutes seconds"""
        an_angle = coords.angle(-44, 59.0, 60)
        self.assertEqual(-45.0, an_angle.degrees)
        return


    def test_mixed_sign_constructor_1(self):
        """Test mixed sign constructor 1"""
        # divide 2 to keep in range
        a = coords.angle(-self.rd1/2, self.rm1, self.rs1)
        b = coords.angle(-self.rd1/2, -self.rm1, self.rs1)
        self.assertAlmostEqual(a.degrees, b.degrees, self.places)
        return


    def test_mixed_sign_constructor_2(self):
        """Test mixed sign constructor 2"""
        a = coords.angle(-self.rd1/2, self.rm1, self.rs1)
        b = coords.angle(-self.rd1/2, self.rm1, -self.rs1)
        self.assertAlmostEqual(a.degrees, b.degrees, self.places)
        return


    def test_mixed_sign_constructor_3(self):
        """Test mixed sign constructor 3"""
        a = coords.angle(-self.rd1/2, self.rm1, self.rs1)
        b = coords.angle(-self.rd1/2, -self.rm1, -self.rs1)
        self.assertAlmostEqual(a.degrees, b.degrees, self.places)
        return



    @unittest.skip('TODO')
    def test_string_constructor_exception(self):
        """Test string constructor exception"""

        self.assertRaises(ArgumentError, lambda a: coords.angle(a), 'some_string')
        self.assertRaises(ArgumentError, lambda a: coords.angle(1, a), 'some_string')
        self.assertRaises(ArgumentError, lambda a: coords.angle(1, -1, a), 'some_string')
        return


    # accessors

    def test_accessors_0(self):
        """Test value accessors 0"""
        an_angle = coords.angle()
        an_angle.degrees = 0
        self.assertEqual(0, an_angle.degrees)
        self.assertEqual(0.0, an_angle.radians)
        self.assertEqual(0.0, an_angle.RA)
        return

    def test_accessors_90(self):
        """Test value accessors 90"""
        an_angle = coords.angle()
        an_angle.degrees = 90
        self.assertEqual(90, an_angle.degrees)
        self.assertEqual(math.pi/2.0, an_angle.radians)
        self.assertEqual(6.0, an_angle.RA)
        return


    def test_accessors_180(self):
        """Test value accessors 180"""
        an_angle = coords.angle()
        an_angle.degrees = 180
        self.assertEqual(180, an_angle.degrees)
        self.assertEqual(math.pi, an_angle.radians)
        self.assertEqual(12.0, an_angle.RA)
        return


    def test_accessors_270(self):
        """Test value accessors 270"""
        an_angle = coords.angle()
        an_angle.degrees = 270
        self.assertEqual(270, an_angle.degrees)
        self.assertEqual(3.0*math.pi/2.0, an_angle.radians)
        self.assertEqual(18.0, an_angle.RA)
        return


    def test_accessors_360(self):
        """Test value accessors 360"""
        an_angle = coords.angle()
        an_angle.degrees = 360
        self.assertEqual(360, an_angle.degrees)
        self.assertEqual(2.0*math.pi, an_angle.radians)
        self.assertEqual(0.0, an_angle.RA)
        return


    def test_accessors_n90(self):
        """Test radians accessor negative"""
        an_angle = coords.angle()
        an_angle.radians = -math.pi/2.0
        self.assertEqual(-90, an_angle.degrees)
        self.assertEqual(-math.pi/2.0, an_angle.radians)
        self.assertEqual(18.0, an_angle.RA)
        return


    @unittest.skip('TODO Boost.Python.ArgumentError')
    def test_accessors_err1(self):
        """Test radians accessor exception"""
        try:
            an_angle = coords.angle()
            an_angle.radians = 'foo'
            self.assertRaises(False) # correct behavior skips this
        except RuntimeError as err:
            self.assertTrue(RuntimeError == type(err))

        return

    # operators

    # add

    def test_inplace_add_angle(self):
        """Test angle += angle"""
        a1 = coords.angle(self.rd1)
        a2 = coords.angle(self.rd2)
        a1 += a2
        self.assertAlmostEqual(self.rd1 + self.rd2, a1.degrees, self.places)
        return


    def test_inplace_add_double(self):
        """Test angle += double"""
        a1 = coords.angle(self.rd1)
        a2 = self.rd2
        a1 += a2
        self.assertAlmostEqual(self.rd1 + self.rd2, a1.degrees, self.places)
        return


    def test_angle_plus_angle(self):
        """Test angle + angle"""
        a1 = coords.angle(self.rd1)
        a2 = coords.angle(self.rd2)
        a3 = coords.angle()
        a3 = a1 + a2
        self.assertAlmostEqual(self.rd1 + self.rd2, a3.degrees, self.places)
        return


    def test_angle_plus_double(self):
        """Test angle + double"""
        a1 = coords.angle(self.rd1)
        a2 = self.rd2
        a3 = coords.angle()
        a3 = a1 + a2
        self.assertAlmostEqual(self.rd1 + self.rd2, a3.degrees, self.places)
        return

    def test_double_plus_angle(self):
        """Test double + angle"""
        a1 = coords.angle(self.rd1)
        a2 = self.rd2
        a3 = coords.angle()
        a3 = a2 + a1
        self.assertAlmostEqual(self.rd1 + self.rd2, a3.degrees, self.places)
        return


    # subtract

    def test_inplace_subtract_angle(self):
        """Test angle -= angle"""
        a1 = coords.angle(self.rd1)
        a2 = coords.angle(self.rd2)
        a1 -= a2
        self.assertAlmostEqual(self.rd1 - self.rd2, a1.degrees, self.places)
        return


    def test_inplace_subtract_double(self):
        """Test angle -= double"""
        a1 = coords.angle(self.rd1)
        a2 = self.rd2
        a1 -= a2
        self.assertAlmostEqual(self.rd1 - self.rd2, a1.degrees, self.places)
        return


    def test_angle_minus_angle(self):
        """Test angle - angle"""
        a1 = coords.angle(self.rd1)
        a2 = coords.angle(self.rd2)
        a3 = coords.angle()
        a3 = a1 - a2
        self.assertAlmostEqual(self.rd1 - self.rd2, a3.degrees, self.places)
        return


    def test_angle_minus_double(self):
        """Test angle - double"""
        a1 = coords.angle(self.rd1)
        a2 = self.rd2
        a3 = coords.angle()
        a3 = a1 - a2
        self.assertAlmostEqual(self.rd1 - self.rd2, a3.degrees, self.places)
        return


    def test_double_minus_angle(self):
        """Test double - angle"""
        a1 = coords.angle(self.rd1)
        a2 = self.rd2
        a3 = coords.angle()
        a3 = a2 - a1
        self.assertAlmostEqual(self.rd2 - self.rd1, a3.degrees, self.places)
        return


    @unittest.skip('TODO')
    def test_unary_minus(self):
        """Test angle = -angle"""
        a1 = coords.angle(self.rd1)
        a2 = -a1
        self.assertAlmostEqual(a1.degrees, -a2.degrees, self.places)
        return


    # multiply

    def test_inplace_multiply_angle(self):
        """Test angle *= angle"""
        a1 = coords.angle(self.rd1)
        a2 = coords.angle(self.rd2)
        a1 *= a2
        self.assertAlmostEqual(self.rd1 * self.rd2, a1.degrees, self.places)
        return


    def test_inplace_multiply_double(self):
        """Test angle *= double"""
        a1 = coords.angle(self.rd1)
        a2 = self.rd2
        a1 *= a2
        self.assertAlmostEqual(self.rd1 * self.rd2, a1.degrees, self.places)
        return


    def test_angle_times_angle(self):
        """Test angle * angle"""
        a1 = coords.angle(self.rd1)
        a2 = coords.angle(self.rd2)
        a3 = a1 * a2
        self.assertAlmostEqual(self.rd1 * self.rd2, a3.degrees, self.places)
        return


    def test_angle_times_double(self):
        """Test angle * double"""
        a1 = coords.angle(self.rd1)
        a2 = self.rd2
        a3 = a1 * a2
        self.assertAlmostEqual(self.rd1 * self.rd2, a3.degrees, self.places)
        return


    def test_double_times_angle(self):
        """Test double * angle"""
        a1 = coords.angle(self.rd1)
        a2 = self.rd2
        a3 = a2 * a1
        self.assertAlmostEqual(self.rd1 * self.rd2, a3.degrees, self.places)
        return


    # divide

    def test_angle_inplace_divide_angle(self):
        """Test angle /= angle"""
        a1 = coords.angle(self.rd1)
        a2 = coords.angle(self.rd2)
        a1 /= a2
        self.assertAlmostEqual(self.rd1 / self.rd2, a1.degrees, self.places)
        return


    def test_angle_inplace_divide_double(self):
        """Test angle /= angle"""
        a1 = coords.angle(self.rd1)
        a2 = self.rd2
        a1 /= a2
        self.assertAlmostEqual(self.rd1 / self.rd2, a1.degrees, self.places)
        return


    def test_angle_divide_angle(self):
        """Test angle / angle"""
        a1 = coords.angle(self.rd1)
        a2 = coords.angle(self.rd2)
        a3 = a1 / a2
        self.assertAlmostEqual(self.rd1 / self.rd2, a3.degrees, self.places)
        return


    def test_angle_divide_double(self):
        """Test angle / double"""
        a1 = coords.angle(self.rd1)
        a2 = self.rd2
        a3 = a1 / a2
        self.assertAlmostEqual(self.rd1 / self.rd2, a3.degrees, self.places)
        return


    def test_double_divide_angle(self):
        """Test double / angle"""
        a1 = coords.angle(self.rd1)
        a2 = self.rd2
        a3 = a2 / a1
        self.assertAlmostEqual(self.rd2 / self.rd1, a3.degrees, self.places)
        return


    def test_angle_divide_zero(self):
        """Test angle / 0"""
        a1 = coords.angle(self.rd1)
        a2 = coords.angle(0)
        self.assertRaises(RuntimeError, lambda a, b: a / b, a1, a2)
        # Note: different from Manual catching coords.Error
        return


    # normalize

    def test_normzlize_1(self):
        """Test normalize 1"""
        a1 = coords.angle(45 + 360)
        a1.normalize(0, 360)
        self.assertAlmostEqual(45, a1.degrees)
        return


    def test_normzlize_2(self):
        """Test normalize 1"""
        a1 = coords.angle(-45 + 360)
        a1.normalize(-180, 180)
        self.assertAlmostEqual(-45, a1.degrees)
        return


    # complement

    def test_complement_45(self):
        """Test complement 45"""
        a1 = coords.angle(45)
        c1 = a1.complement()
        self.assertAlmostEqual(45, c1.degrees)
        return


    def test_complement_60(self):
        """Test complement 60"""
        a1 = coords.angle(60)
        c1 = a1.complement()
        self.assertAlmostEqual(30, c1.degrees)
        return


    # strings

    def test_str(self):
        """Test str"""
        an_angle = coords.angle()
        a_str = '00:00:00.0'
        self.assertEqual(a_str, str(an_angle))
        return


    @unittest.skip('Not available in boost')
    def test_repr(self):
        """Test repr"""
        an_angle = coords.angle()
        a_repr = '''0* 0' 0"'''
        self.assertEqual(a_repr, repr(an_angle))
        return


class TestLatitude(unittest.TestCase):

    def setUp(self):

        """Set up test parameters."""

        self.places = 7 # precision
        return


    def test_default_constructor(self):
        """Test default constructor"""
        a_latitude = coords.latitude()
        self.assertEqual(0, a_latitude.degrees)
        return


    def test_constructor_1(self):
        """Test constructor 1"""
        a_latitude = coords.latitude(45)
        self.assertEqual(45, a_latitude.degrees)
        return


    def test_constructor_exception_north(self):
        """Test constructor_exception north"""
        try:
            a_latitude = coords.latitude(100)
            self.assertRaises(False) # correct behavior skips this
        except RuntimeError as err:
            self.assertTrue(RuntimeError == type(err))
        return


    def test_constructor_exception_south(self):
        """Test constructor_exception south"""
        try:
            a_latitude = coords.latitude(-100)
            self.assertRaises(False) # correct behavior skips this
        except RuntimeError as err:
            self.assertTrue(RuntimeError == type(err))
        return


    def test_inplace_add_1(self):
        """Test latitude += latitude"""
        a1 = coords.latitude(10)
        a2 = coords.latitude(10)
        a1 += a2
        self.assertAlmostEqual(20, a1.degrees, self.places)
        return


    def test_inplace_add_2(self):
        """Test latitude += angle"""
        a1 = coords.latitude(10)
        a2 = coords.angle(10)
        a1 += a2
        self.assertAlmostEqual(20, a1.degrees, self.places)
        return


    def test_inplace_add_over_limit(self):
        """Test latitude += latitude"""
        # TODO doesn't prevent this. See Angles with templates repo.
        a1 = coords.latitude(50)
        a2 = coords.latitude(50)
        a1 += a2
        self.assertAlmostEqual(100, a1.degrees, self.places)
        return


class TestDeclination(unittest.TestCase):

    def setUp(self):

        """Set up test parameters."""

        self.places = 7 # precision
        return


    def test_default_constructor(self):
        """Test default constructor"""
        a_declination = coords.declination()
        self.assertEqual(0, a_declination.degrees)
        return


    def test_constructor_1(self):
        """Test constructor 1"""
        a_declination = coords.declination(45)
        self.assertEqual(45, a_declination.degrees)
        return


    def test_constructor_exception_north(self):
        """Test constructor_exception north"""
        try:
            a_declination = coords.declination(100)
            self.assertRaises(False) # correct behavior skips this
        except RuntimeError as err:
            self.assertTrue(RuntimeError == type(err))
        return


    def test_constructor_exception_south(self):
        """Test constructor_exception south"""
        try:
            a_declination = coords.declination(-100)
            self.assertRaises(False) # correct behavior skips this
        except RuntimeError as err:
            self.assertTrue(RuntimeError == type(err))
        return


    def test_inplace_add_1(self):
        """Test declination += declination"""
        a1 = coords.declination(10)
        a2 = coords.declination(10)
        a1 += a2
        self.assertAlmostEqual(20, a1.degrees, self.places)
        return


    def test_inplace_add_2(self):
        """Test declination += angle"""
        a1 = coords.declination(10)
        a2 = coords.angle(10)
        a1 += a2
        self.assertAlmostEqual(20, a1.degrees, self.places)
        return


    def test_inplace_add_over_limit(self):
        """Test declination += declination"""
        # TODO doesn't prevent this. See Angles with templates repo.
        a1 = coords.declination(50)
        a2 = coords.declination(50)
        a1 += a2
        self.assertAlmostEqual(100, a1.degrees, self.places)
        return



if __name__ == '__main__':
    random.seed(time.time())
    unittest.main()
