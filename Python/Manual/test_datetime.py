"""Unit tests for coords objects.


To help find Segmentation fault

import sys

def trace(frame, event, arg):
    print("%s, %s:%d" % (event, frame.f_code.co_filename, frame.f_lineno))
    return trace

sys.settrace(trace)

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

        return


    # ------------------------
    # ----- constructors -----
    # ------------------------

    def test_default_constructor(self):
        """Test default constructor"""
        a = coords.datetime()
        self.assertEqual('1970-01-01T00:00:00.0', str(a))

        return


    def test_jdate_constructor_0(self):
        """Test jdate constructor"""
        a_datetime_string = '2019-09-16T19:45:14.1'
        a = coords.datetime(2458743.32308)
        self.assertEqual(a_datetime_string, str(a))

        return


    def test_string_constructor_0(self):
        """Test string no timezone constructor"""
        a_datetime_string = '1962-07-10T07:30:00.0'
        a = coords.datetime(a_datetime_string)
        self.assertEqual(a_datetime_string, str(a))

        return


    def test_string_constructor_1(self):
        """Test string with timezone constructor"""
        a_datetime_string = '1962-07-10T07:30:00.0-05:00'
        a = coords.datetime(a_datetime_string)
        self.assertEqual(a_datetime_string, str(a))

        return


    def test_int_constructor_1(self):
        """Test integer constructor with 2 terms"""
        a_datetime_string = '1962-01-01T00:00:00.0'
        a = coords.datetime(1962)
        self.assertEqual(a_datetime_string, str(a))

        return

    def test_int_constructor_2(self):
        """Test integer constructor with 2 terms"""
        a_datetime_string = '1962-07-01T00:00:00.0'
        a = coords.datetime(1962, 7)
        self.assertEqual(a_datetime_string, str(a))

        return


    def test_int_constructor_3(self):
        """Test integer constructor with 3 terms"""
        a_datetime_string = '1962-07-10T00:00:00.0'
        a = coords.datetime(1962, 7, 10)
        self.assertEqual(a_datetime_string, str(a))

        return


    def test_int_constructor_4(self):
        """Test integer constructor with 4 terms"""
        a_datetime_string = '1962-07-10T08:00:00.0'
        a = coords.datetime(1962, 7, 10, 8)
        self.assertEqual(a_datetime_string, str(a))

        return


    def test_int_constructor_5(self):
        """Test integer constructor with 5 terms"""
        a_datetime_string = '1962-07-10T07:40:00.0'
        a = coords.datetime(1962, 7, 10, 7, 40)
        self.assertEqual(a_datetime_string, str(a))

        return

    def test_int_constructor_6(self):
        """Test integer constructor with 6 terms"""
        a_datetime_string = '1962-07-10T07:30:01.5'
        a = coords.datetime(1962, 7, 10, 7, 30, 1.4995)
        self.assertEqual(a_datetime_string, str(a))

        return


    def test_int_constructor_7(self):
        """Test integer constructor with 7 terms"""
        a_datetime_string = '1962-07-10T07:30:00.0-0500'
        a = coords.datetime(1962, 7, 10, 7, 30, 00, -5)
        self.assertEqual(a_datetime_string, str(a))

        return


    def test_int_constructor_8(self):
        """Test integer constructor with 7 terms"""
        a_datetime_string = '1962-07-10T07:30:00.0-0530'
        a = coords.datetime(1962, 7, 10, 7, 30, 00, -5.5)
        self.assertEqual(a_datetime_string, str(a))

        return


    def test_int_constructor_9(self):
        """Test integer constructor with 7 terms"""
        a_datetime_string = '1962-07-10T07:30:00.0+05:30'
        a = coords.datetime(1962, 7, 10, 7, 30, 00, "+05:30")
        self.assertEqual(a_datetime_string, str(a))

        return


    def test_int_constructor_10(self):
        """Test integer constructor with 7 terms"""
        a_datetime_string = '1962-07-10T07:30:00.0+05:30'
        a = coords.datetime(1962, 7, 10, 7, 30, 00, "5:30")
        self.assertEqual(a_datetime_string, str(a))

        return


    def test_string_constructor_exception(self):
        """Test string constructor exception"""
        self.assertRaises(coords.Error, lambda a: coords.datetime(a), 'some_string')

        return



    # accessors

    def test_julian_date_accessor_1(self):
        """Test default constructor julian date"""
        a = coords.datetime('1970-01-01T00:00:00')
        self.assertEqual(2440587.5, a.toJulianDate())

        return

    def test_julian_date_accessor_2(self):
        """Test juliand date date accessor"""
        a = coords.datetime('1968-05-24T00:00:00.0')
        self.assertEqual(2440000.5, a.toJulianDate())

        return


    def test_lilian_date_accessor(self):
        """Test lilian date accessor"""
        a = coords.datetime()
        self.assertEqual(2299160.5, a.LilianDate)

        # TODO different from Boost's access to static data member
        # e.g. self.assertEqual(2440000.5, coords.datetime.TruncatedJulianDate)

        return


    def test_modified_date_accessor(self):
        """Test modified date accessor"""
        a = coords.datetime()
        self.assertEqual(2400000.5, a.ModifiedJulianDate)

        return


    def test_truncated_julian_date_accessor(self):
        """Test truncated date accessor"""
        a = coords.datetime()
        self.assertEqual(2440000.5, a.TruncatedJulianDate)

        return



    def test_j2000_accessor(self):
        """Test j2000 accessor"""
        a = coords.datetime()
        self.assertEqual(2451545.0, a.J2000)

        return


    def test_default_constructor(self):
        """Test default constructor"""
        a = coords.datetime()
        self.assertEqual('1970-01-01T00:00:00.0Z', str(a))

        return


    def test_string_constructor(self):
        """Test string constructor"""
        a_datetime_string = '1962-07-10T07:30:00.0+05:00'
        a = coords.datetime(a_datetime_string)
        self.assertEqual(a_datetime_string, str(a))

        return

    def test_string_constructor_exception(self):
        """Test string constructor exception"""
        self.assertRaises(coords.Error, lambda a: coords.datetime(a), 'some_string')

        return


    def test_string_constructor_exception_2(self):
        """Test string constructor exception"""
        self.assertRaises(coords.Error, lambda a: coords.datetime(a), '1962-60-10T07:30:00+05:00')

        return


    def test_year_constructor_year(self):
        """Test year constructor"""
        a = coords.datetime(1962)
        self.assertEqual('1962-01-01T00:00:00.0', str(a))

        return


    def test_year_month_constructor(self):
        """Test year, month constructor"""
        a = coords.datetime(1962, 7)
        self.assertEqual('1962-07-01T00:00:00.0', str(a))

        return


    def test_year_month_day_constructor(self):
        """Test year month day constructor"""
        a = coords.datetime(1962, 7, 10)
        self.assertEqual('1962-07-10T00:00:00.0', str(a))

        return


    def test_year_month_day_hour_constructor(self):
        """Test year month day hour constructor"""
        a = coords.datetime(1962, 7, 10, 14)
        self.assertEqual('1962-07-10T14:00:00.0', str(a))

        return


    def test_year_month_day_hour_minute_constructor(self):
        """Test year month day hour constructor"""
        a = coords.datetime(1962, 7, 10, 14, 30)
        self.assertEqual('1962-07-10T14:30:00.0', str(a))

        return


    def test_year_month_day_hour_minute_second_constructor(self):
        """Test year month day hour constructor"""
        a = coords.datetime(1962, 7, 10, 14, 30, 25.29)
        self.assertEqual('1962-07-10T14:30:25.3', str(a))

        return


    def test_year_month_day_hour_minute_second_timezone_constructor(self):
        """Test 7 args constructor"""
        a = coords.datetime(1962, 7, 10, 14, 30, 25.54, -8)
        self.assertEqual('1962-07-10T14:30:25.5-0800', str(a))

        return

    def test_year_month_day_hour_minute_second_timezone_constructor_2(self):
        """Test 7 args timezone constructor"""

        a = coords.datetime(1962, 7, 10, 14, 30, 28.9999, "-08:00")
        self.assertEqual('1962-07-10T14:30:29.0-08:00', str(a))

        return


    def test_year_month_bad_day_hour_minute_second_constructor_exception_1(self):
        """Test bad month exception constructor"""
        self.assertRaises(coords.Error, lambda a: coords.datetime(*a), (1962, 25, 66, 14, 30, 25))
        return


    def test_year_bad_timezone_constructor_exception_2(self):
        """Test bad time zone constructor"""
        self.assertRaises(coords.Error, lambda a: coords.datetime(*a), (1962, 7, 10, 14, 30, 25, "asdf"))


    # Julian date methods

    def test_to_julian_modified_date(self):
        """Test to modified Julian date"""
        a_datetime_string = '1858-11-17T00:00:00.0'
        a = coords.datetime(a_datetime_string)
        self.assertEqual(a.ModifiedJulianDate, a.toJulianDate())

        return


    def test_to_julian_date_as_attribute(self):
        """Test to Julian date as attribute"""
        a_datetime_string = '2014-12-09T00:00:00'
        a = coords.datetime(a_datetime_string)
        self.assertEqual(2457000.5, a.toJulianDate())

        return


    def test_to_julian_date_as_method(self):
        """Test to Julian date as method"""
        a_datetime_string = '2014-12-09T00:00:00'
        a = coords.datetime(a_datetime_string)
        self.assertEqual(2457000.5, a.toJulianDate())

        return


    def test_modified_julian_date_1(self):
        """Test modified Julian date"""
        a = coords.datetime('1858-11-17T00:00:00')
        self.assertEqual(a.ModifiedJulianDate, a.toJulianDate())

        return


    def test_from_julian_date_as_method_1(self):
        """Test from Julian date as method"""

        a = coords.datetime('1800-01-01T12:00')
        self.assertEqual('1800-01-01T12:00:00', str(a))
        self.assertEqual(2378497.0, a.toJulianDate())

        b = a.fromJulianDate(a.ModifiedJulianDate)
        self.assertEqual('1858-11-17T00:00:00', str(b))
        self.assertEqual(2400000.5, b.toJulianDate())

        return

    def test_from_julian_date_as_method_1(self):
        """Test from Julian date as method"""

        a = coords.datetime('1900-01-01T00:00:00-08:00')
        self.assertEqual('1900-01-01T00:00:00.0-08:00', str(a))
        self.assertEqual(2415020.8333333335, a.toJulianDate())

        b = a.fromJulianDate(a.toJulianDate())
        self.assertEqual('1900-01-01T08:00:00.0', str(b))
        self.assertEqual(2415020.8333333335, b.toJulianDate())

        return


    def test_from_julian_date_as_method_exception(self):
        """Test from Julian date as method exception"""
        a_datetime = coords.datetime()
        self.assertRaises(TypeError, lambda a: a.fromJulianDate('asdf'), a_datetime)

        return

    # operators

    # add

    def test_operator_inplace_plus_double(self):
        """Test operator plus equal"""
        a_datetime_string = '1962-07-10T07:30:00+05:00'
        a = coords.datetime(a_datetime_string)
        a += 365.0
        self.assertEqual('1963-07-10T07:30:00.0+05:00', str(a))

        return


    def test_operator_inplace_plus_exception_string(self):
        """Test operator+=(string) exception"""
        a = coords.datetime('1962-07-10T07:30:00+05:00')
        try:
            a += 'asdf'
        except coords.Error as err:
            self.assertTrue(coords.Error == type(err))

        return


    def test_operator_inplace_plus_exception_datetime(self):
        """Test operator+=(datetime) exception"""
        a = coords.datetime('1962-07-10T07:30:00+05:00')
        b = coords.datetime('1962-07-10T07:30:00+05:00')
        try:
            a += b
        except coords.Error as err:
            self.assertTrue(coords.Error == type(err))

        return


    def test_datetime_plus_datetime_1(self):
        """Test datetime + datetime"""
        a = coords.datetime('1962-07-10T07:30:00-06:00')
        b = 365 + a
        self.assertEqual('1963-07-10T07:30:00.0-06:00', str(b))

        return


    def test_datetime_plus_datetime_2(self):
        """Test datetime + datetime"""
        a = coords.datetime('1962-07-10T07:30:00+05:00')
        b = a + 365 # commute
        self.assertEqual('1963-07-10T07:30:00.0+05:00', str(b))

        return


    # minus

    def test_operator_inplace_minus_double(self):
        """Test operator-=(double)"""
        a = coords.datetime('1962-07-10T07:30:00+05:00')
        a -= 365
        self.assertEqual('1961-07-10T07:30:00.0+05:00', str(a))

        return


    def test_operator_inplace_minus_exception_string(self):
        """Test operator-=(string) exception"""
        a_datetime_string = '1962-07-10T07:30:00+05:00'
        a = coords.datetime(a_datetime_string)
        try:
            a -= 'asdf'
        except coords.Error as err:
            self.assertTrue(coords.Error == type(err))

        return


    def test_operator_inplace_minus_exception_datetime(self):
        """Test operator-=(string) exception"""
        a = coords.datetime('1962-07-10T07:30:00+05:00')
        b = coords.datetime('1962-07-10T07:30:00+05:00')
        try:
            a -= b
        except coords.Error as err:
            self.assertTrue(coords.Error == type(err))

        return


    def test_datetime_minus_datetime_1(self):
        """Test datetime - datetime"""
        a = coords.datetime('1962-07-10T07:30:00-05:00')
        b = coords.datetime('1963-07-10T07:30:00-05:00')
        self.assertEqual(365, b - a)

        return


    def test_datetime_minus_datetime_2(self):
        """Test datetime - datetime"""
        a = coords.datetime('1962-07-10T07:30:00+05:00')
        b = coords.datetime('1963-07-10T07:30:00+05:00')
        self.assertEqual(-365.0, a - b)

        return

    # inTimezone shifts

    def test_inTimezone_1(self):
        """Test set time zone method"""
        a = coords.datetime('2019-11-10T07:30:00')
        b = a.inTimezone('-8')
        self.assertEqual(-8, b.offset())
        self.assertEqual('2019-11-09T23:30:00.0-0800', str(b))

        return


    def test_inTimezone_2(self):
        """Test set time zone method"""
        a = coords.datetime('2019-11-10T07:30:00+0530')
        self.assertEqual(5.5, a.offset())
        b = a.inTimezone('08:00')
        self.assertEqual(8, b.offset())
        self.assertEqual('2019-11-10T10:00:00.0+08:00', str(b))

        return


    def test_inTimezoneOffset_1(self):
        """Test set time zone method"""
        a = coords.datetime('2017-10-05T07:30:00-05:00')
        self.assertEqual(-5.0, a.offset())
        b = a.inTimezoneOffset(-8.0)
        self.assertEqual(-8.0, b.offset())
        self.assertEqual('2017-10-05T04:30:00.0-0800', str(b))

        return

    def test_inTimezoneOffset_2(self):
        """Test set time zone method"""
        a = coords.datetime('1999-12-31T08:30:00-0800')
        self.assertEqual(-8, a.offset())
        b = a.inTimezoneOffset(8.0)
        self.assertEqual(8, b.offset())
        self.assertEqual('2000-01-01T00:30:00.0+0800', str(b))

        return


if __name__ == '__main__':
    random.seed(time.time())
    unittest.main()
