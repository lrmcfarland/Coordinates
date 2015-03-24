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


    # constructors

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

    def test_j2000_accessor(self):
        """Test j2000 accessor"""
        a = coords.datetime()
        self.assertEqual(2451545.0, a.J2000)


    def test_ut_accessor_1(self):
        """Test UT accessor 1"""
        a_date_string = '2015-01-01T12:30:30-08:00'
        a = coords.datetime(a_date_string)
        self.assertEqual(4.508333333333334, a.UT())


    def test_ut_accessor_n1(self):
        """Test UT accessor n1,

        TODO negative time puts it on the previous day
        """

        a_date_string = '2015-01-01T07:30:00-08:00'
        a = coords.datetime(a_date_string)
        self.assertEqual(-1.5, a.UT())


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

    def test_string_constructor_exception_2(self):
        """Test string constructor exception"""
        self.assertRaises(coords.Error, lambda a: coords.datetime(a), '1962-60-10T07:30:00+05:00')

    def test_year_constructor_year(self):
        """Test year constructor"""
        a = coords.datetime(1962)
        self.assertEqual('1962-01-01T00:00:00', str(a))

    def test_year_month_constructor(self):
        """Test year, month constructor"""
        a = coords.datetime(1962, 7)
        self.assertEqual('1962-07-01T00:00:00', str(a))

    def test_year_month_day_constructor(self):
        """Test year month day constructor"""
        a = coords.datetime(1962, 7, 10)
        self.assertEqual('1962-07-10T00:00:00', str(a))

    def test_year_month_day_hour_constructor(self):
        """Test year month day hour constructor"""
        a = coords.datetime(1962, 7, 10, 14)
        self.assertEqual('1962-07-10T14:00:00', str(a))

    def test_year_month_day_hour_minute_constructor(self):
        """Test year month day hour constructor"""
        a = coords.datetime(1962, 7, 10, 14, 30)
        self.assertEqual('1962-07-10T14:30:00', str(a))

    def test_year_month_day_hour_minute_second_constructor(self):
        """Test year month day hour constructor"""
        a = coords.datetime(1962, 7, 10, 14, 30, 25)
        self.assertEqual('1962-07-10T14:30:25', str(a))

    @unittest.skip('this should raise an exception')
    def test_year_month_day_hour_minute_second_constructor_exception(self):
        """Test year month day hour constructor"""
        a = coords.datetime(1962, 25, 66, 14, 30, 25)
        self.assertEqual('1962-07-10T14:30:25', str(a))

    # Julian date methods

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

    # operators

    # add

    def test_operator_inplace_plus_double(self):
        """Test operator plus equal"""
        a_date_string = '1962-07-10T07:30:00+05:00'
        a = coords.datetime(a_date_string)
        a += 365.0
        self.assertEqual('1963-07-10T07:30:00+05:00', str(a))
        self.assertEqual(5, a.timezone())


    def test_operator_inplace_plus_exception_string(self):
        """Test operator+=(string) exception"""
        a = coords.datetime('1962-07-10T07:30:00+05:00')
        try:
            a += 'asdf'
        except coords.Error, err:
            self.assertTrue(coords.Error == type(err))

    def test_operator_inplace_plus_exception_datetime(self):
        """Test operator+=(datetime) exception"""
        a = coords.datetime('1962-07-10T07:30:00+05:00')
        b = coords.datetime('1962-07-10T07:30:00+05:00')
        try:
            a += b
        except coords.Error, err:
            self.assertTrue(coords.Error == type(err))

    def test_datetime_plus_datetime_1(self):
        """Test datetime + datetime"""
        a = coords.datetime('1962-07-10T07:30:00-06:00')
        b = 365 + a
        self.assertEqual('1963-07-10T07:30:00-06:00', str(b))
        self.assertEqual(-6, a.timezone())

    def test_datetime_plus_datetime_2(self):
        """Test datetime + datetime"""
        a = coords.datetime('1962-07-10T07:30:00+05:00')
        b = a + 365 # commute
        self.assertEqual('1963-07-10T07:30:00+05:00', str(b))

    # minus

    def test_operator_inplace_minus_double(self):
        """Test operator-=(double)"""
        a = coords.datetime('1962-07-10T07:30:00+05:00')
        a -= 365
        self.assertEqual('1961-07-10T07:30:00+05:00', str(a))
        self.assertEqual(5, a.timezone())

    def test_operator_inplace_minus_exception_string(self):
        """Test operator-=(string) exception"""
        a_date_string = '1962-07-10T07:30:00+05:00'
        a = coords.datetime(a_date_string)
        try:
            a -= 'asdf'
        except coords.Error, err:
            self.assertTrue(coords.Error == type(err))

    def test_operator_inplace_minus_exception_datetime(self):
        """Test operator-=(string) exception"""
        a = coords.datetime('1962-07-10T07:30:00+05:00')
        b = coords.datetime('1962-07-10T07:30:00+05:00')
        try:
            a -= b
        except coords.Error, err:
            self.assertTrue(coords.Error == type(err))

    def test_datetime_minus_datetime_1(self):
        """Test datetime - datetime"""
        a = coords.datetime('1962-07-10T07:30:00-05:00')
        b = coords.datetime('1963-07-10T07:30:00-05:00')
        self.assertEqual(365, b - a)
        self.assertEqual(-5, a.timezone())


    def test_datetime_minus_datetime_2(self):
        """Test datetime - datetime"""
        a = coords.datetime('1962-07-10T07:30:00+05:00')
        b = coords.datetime('1963-07-10T07:30:00+05:00')
        self.assertEqual(-365.0, a - b)


if __name__ == '__main__':
    random.seed(time.time())
    unittest.main()
