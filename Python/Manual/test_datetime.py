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
        a = coords.datetime()
        self.assertEqual(2299160.5, a.LilianDate)

        # TODO different from Boost's access to static data member
        # e.g. self.assertEqual(2440000.5, coords.datetime.TruncatedJulianDate)

    def test_modified_date_accessor(self):
        """Test modified date accessor"""
        a = coords.datetime()
        self.assertEqual(2400000.5, a.ModifiedJulianDate)


    def test_truncated_date_accessor(self):
        """Test truncated date accessor"""
        a = coords.datetime()
        self.assertEqual(2440000.5, a.TruncatedJulianDate)


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
        self.assertRaises(coords.Error, lambda a: coords.datetime(a), 'some_string')


    def test_year_constructor(self):
        """Test default constructor"""
        a = coords.datetime(1962)
        self.assertEqual('1962-01-01T00:00:00', str(a))


    def test_to_julian_modified_date(self):
        """Test to modified Julian date"""
        a_date_string = '1858-11-17T00:00:00'
        a = coords.datetime(a_date_string)
        self.assertEqual(a.ModifiedJulianDate, a.JulianDate)


    def test_to_julian_date_as_attribute(self):
        """Test to Julian date as attribute"""
        a_date_string = '2014-12-09T00:00:00'
        a = coords.datetime(a_date_string)
        self.assertEqual(2457000.5, a.JulianDate)


    def test_to_julian_date_as_method(self):
        """Test to Julian date as method"""
        a_date_string = '2014-12-09T00:00:00'
        a = coords.datetime(a_date_string)
        self.assertEqual(2457000.5, a.toJulianDate())


    def test_from_julian_date_as_attribute(self):
        """Test from Julian date as attribute"""
        a = coords.datetime()
        a.JulianDate = a.ModifiedJulianDate
        self.assertEqual(a.ModifiedJulianDate, a.JulianDate)

    def test_from_julian_date_as_method(self):
        """Test from Julian date as method"""
        a = coords.datetime()
        a.fromJulianDate(a.ModifiedJulianDate)
        self.assertEqual(a.ModifiedJulianDate, a.toJulianDate())


    def test_from_julian_date_as_method_exception(self):
        """Test from Julian date as method exception"""
        a_datetime = coords.datetime()
        self.assertRaises(TypeError, lambda a: a.fromJulianDate('asdf'), a_datetime)


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


    def test_operator_minus_datetime_1(self):
        """Test operator minus date time"""
        a = coords.datetime('1962-07-10T07:30:00+05:00')
        b = coords.datetime('1963-07-10T07:30:00+05:00')
        self.assertEqual(365, b - a)


    def test_operator_minus_datetime_2(self):
        """Test operator minus date time"""
        a = coords.datetime('1962-07-10T07:30:00+05:00')
        b = coords.datetime('1963-07-10T07:30:00+05:00')
        self.assertEqual(-365, a - b)


    def test_operator_minus_datetime_2(self):
        """Test operator minus date time"""
        a = coords.datetime('1962-07-10T07:30:00+05:00')
        b = coords.datetime('2012-07-10T07:30:00+05:00')
        self.assertEqual(18263.0, b - a)



if __name__ == '__main__':
    random.seed(time.time())
    unittest.main()
