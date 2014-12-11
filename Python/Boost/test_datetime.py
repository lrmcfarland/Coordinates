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


class TestDateTime(unittest.TestCase):

    def setUp(self):

        """Set up test parameters."""

        self.places = 7 # precision



    # -----------------------------
    # ----- test constructors -----
    # -----------------------------

    def test_lilian_date_accessor(self):
        """Test lilian date accessor"""
        self.assertEqual(2299160.5, coords.datetime.LilianDate)

    def test_modified_date_accessor(self):
        """Test modified date accessor"""
        self.assertEqual(2400000.5, coords.datetime.ModifiedJulianDate)

    def test_truncated_date_accessor(self):
        """Test truncated date accessor"""
        self.assertEqual(2440000.5, coords.datetime.TruncatedJulianDate)


    def test_default_constructor(self):
        """Test default constructor"""
        a = coords.datetime()
        self.assertEqual('1970-01-01T00:00:00', str(a))


    def test_string_constructor(self):
        """Test string constructor"""
        a_date_string = '1962-07-10T07:30:00+05:00'
        a = coords.datetime(a_date_string)
        self.assertEqual(a_date_string, str(a))


    def test_string_constructor_exception(self):
        """Test string constructor exception"""
        self.assertRaises(RuntimeError, lambda a: coords.datetime(a), 'some_string')


    def test_year_constructor(self):
        """Test default constructor"""
        a = coords.datetime(1962)
        self.assertEqual('1962-01-01T00:00:00', str(a))


    def test_julian_date(self):
        """Test operator plus equal"""
        a_date_string = '1858-11-17T00:00:00'
        a = coords.datetime(a_date_string)
        self.assertEqual(coords.datetime.ModifiedJulianDate, a.toJulianDate())


    def test_operator_plus_equal(self):
        """Test operator plus equal"""
        a_date_string = '1962-07-10T07:30:00+05:00'
        a = coords.datetime(a_date_string)
        a += 365
        self.assertEqual('1963-07-10T17:30:00+05:00', str(a))


    def test_operator_plus(self):
        """Test operator plus"""
        a_date_string = '1962-07-10T07:30:00+05:00'
        a = coords.datetime(a_date_string)
        b = coords.datetime
        b = 365 + a
        self.assertEqual('1963-07-10T17:30:00+05:00', str(b))


    def test_operator_minus_equal(self):
        """Test operator minus equal"""
        a_date_string = '1962-07-10T07:30:00+05:00'
        a = coords.datetime(a_date_string)
        a -= 365
        self.assertEqual('1961-07-10T17:30:00+05:00', str(a))




if __name__ == '__main__':
    random.seed(time.time())
    unittest.main()
