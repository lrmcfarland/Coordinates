"""Unit tests for coords objects."""

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

    # ------------------------
    # ----- constructors -----
    # ------------------------

    def test_default_constructor(self):
        """Test default constructor"""
        a = coords.datetime()
        self.assertEqual('1970-01-01T00:00:00', str(a))

        return


    def test_string_constructor(self):
        """Test string constructor"""
        a_date_string = '1962-07-10T07:30:00-05:00'
        a = coords.datetime(a_date_string)
        self.assertEqual(a_date_string, str(a))

        return


    def test_string_constructor_exception(self):
        """Test string constructor exception"""
        self.assertRaises(RuntimeError, lambda a: coords.datetime(a), 'some_string')

        return


    def test_year_constructor(self):
        """Test default constructor"""
        a = coords.datetime(1962)
        self.assertEqual('1962-01-01T00:00:00', str(a))

        return


    # ---------------------
    # ----- accessors -----
    # ---------------------

    def test_lilian_date_accessor(self):
        """Test lilian date accessor"""
        self.assertEqual(2299160.5, coords.datetime.LilianDate)
        # TODO boost different from manual accessors

        return


    def test_modified_date_accessor(self):
        """Test modified date accessor"""
        self.assertEqual(2400000.5, coords.datetime.ModifiedJulianDate)

        return


    def test_truncated_date_accessor(self):
        """Test truncated date accessor"""
        self.assertEqual(2440000.5, coords.datetime.TruncatedJulianDate)

        return


    def test_j2000_date_accessor(self):
        """Test truncated date accessor"""
        self.assertEqual(2451545.0, coords.datetime.J2000)

        return


    # time element accessors

    def test_year_accessor(self):
        """Test get year accessor"""
        a = coords.datetime('2016-04-02T21:34:45')
        self.assertEqual(2016, a.year)

        return


    def test_month_accessor(self):
        """Test get month accessor"""
        a = coords.datetime('2016-04-02T21:34:45')
        self.assertEqual(4, a.month)

        return


    def test_day_accessor(self):
        """Test get day accessor"""
        a = coords.datetime('2016-04-02T21:34:45')
        self.assertEqual(2, a.day)

        return


    def test_hour_accessor(self):
        """Test get hour accessor"""
        a = coords.datetime('2016-04-02T21:34:45')
        self.assertEqual(21, a.hour)

        return


    def test_minute_accessor(self):
        """Test get minute accessor"""
        a = coords.datetime('2016-04-02T21:34:45')
        self.assertEqual(34, a.minute)

        return


    def test_second_accessor(self):
        """Test get second accessor"""
        a = coords.datetime('2016-04-02T21:34:45.67')
        self.assertEqual(45.67, a.second)

        return


    def test_to_julian_date(self):
        """Test to Julian date"""
        a_date_string = '1858-11-17T00:00:00'
        a = coords.datetime(a_date_string)
        self.assertEqual(coords.datetime.ModifiedJulianDate, a.toJulianDate())

        return


    def test_from_julian_date(self):
        """Test from Julian date"""
        a = coords.datetime()
        b = a.fromJulianDate(coords.datetime.ModifiedJulianDate)
        self.assertEqual(coords.datetime.ModifiedJulianDate, b.toJulianDate())

        return


    # --------------------------------
    # ----- arithmetic operators -----
    # --------------------------------

    def test_operator_plus_equal(self):
        """Test operator plus equal"""
        a_date_string = '1962-07-10T07:30:00+05:00'
        a = coords.datetime(a_date_string)
        a += 365
        self.assertEqual('1963-07-10T07:30:00+05:00', str(a))

        return


    def test_operator_plus(self):
        """Test operator plus"""
        a_date_string = '1962-07-10T07:30:00-06:00'
        a = coords.datetime(a_date_string)
        b = coords.datetime
        b = 365 + a
        self.assertEqual('1963-07-10T07:30:00-06:00', str(b))

        return


    def test_operator_plus_equal_hour_1(self):
        """Test operator plus equal one hour"""
        a_date_string = '2016-05-08T07:30:00'
        a = coords.datetime(a_date_string)
        a += 1.0/24
        self.assertEqual('2016-05-08T08:30:00', str(a))

        return


    def test_operator_plus_equal_hour_2(self):
        """Test operator plus equal one hour at end of year"""
        a_date_string = '2016-12-31T23:30:00'
        a = coords.datetime(a_date_string)
        a += 1.0/24
        self.assertEqual('2017-01-01T00:30:00', str(a))

        return


    def test_operator_plus_hour_1(self):
        """Test operator plus one hour"""

        a_date_string = '2016-05-08T07:30:00'

        a = coords.datetime(a_date_string)
        b = a + 1.0/24

        self.assertEqual('2016-05-08T08:30:00', str(b))

        return


    def test_operator_plus_hour_2(self):
        """Test operator minus one hour at end of day"""

        a_date_string = '2016-05-08T23:30:00'

        a = coords.datetime(a_date_string)
        b = a + 1.0/24

        self.assertEqual('2016-05-09T00:30:00', str(b))

        return


    def test_operator_plus_hour_3(self):
        """Test operator minus one hour at end of year"""

        a_date_string = '2016-12-31T23:30:00'

        a = coords.datetime(a_date_string)
        b = a + 1.0/24

        self.assertEqual('2017-01-01T00:30:00', str(b))

        return


    def test_operator_minus_equal_year(self):
        """Test operator minus equal one year"""
        a_date_string = '1962-07-10T07:30:00+05:00'
        a = coords.datetime(a_date_string)
        a -= 365
        self.assertEqual('1961-07-10T07:30:00+05:00', str(a))

        return


    def test_operator_minus_equal_hour_1(self):
        """Test operator minus equal one hour"""
        a_date_string = '2016-05-08T07:30:00'
        a = coords.datetime(a_date_string)
        a -= 1.0/24
        self.assertEqual('2016-05-08T06:30:00', str(a))

        return


    def test_operator_minus_equal_hour_2(self):
        """Test operator minus equal one hour at beginning of a day"""
        a_date_string = '2016-05-08T00:30:00'
        a = coords.datetime(a_date_string)
        a -= 1.0/24
        self.assertEqual('2016-05-07T23:30:00', str(a))

        return


    def test_operator_minus_equal_hour_3(self):
        """Test operator minus equal one hour at beginning of a year"""
        a_date_string = '2016-01-01T00:30:00'
        a = coords.datetime(a_date_string)
        a -= 1.0/24
        self.assertEqual('2015-12-31T23:30:00', str(a))

        return


    def test_operator_minus_datetime_1(self):
        """Test operator minus date time"""
        a = coords.datetime('1962-07-10T07:30:00+05:00')
        b = coords.datetime('1963-07-10T07:30:00+05:00')
        self.assertEqual(365, b - a)

        return


    def test_operator_minus_datetime_2(self):
        """Test operator minus date time"""
        a = coords.datetime('1962-07-10T07:30:00+05:00')
        b = coords.datetime('1963-07-10T07:30:00+05:00')
        self.assertEqual(-365, a - b)

        return


    def test_operator_minus_datetime_2(self):
        """Test operator minus date time"""
        a = coords.datetime('1962-07-10T07:30:00+05:00')
        b = coords.datetime('2012-07-10T07:30:00+05:00')
        self.assertEqual(18263.0, b - a)

        return


    def test_operator_minus_hour_1(self):
        """Test operator minus one hour"""

        a_date_string = '2016-05-08T07:30:00'

        a = coords.datetime(a_date_string)
        b = a - 1.0/24

        self.assertEqual('2016-05-08T06:30:00', str(b))

        return


    def test_operator_minus_hour_2(self):
        """Test operator minus one hour at end of day"""

        a_date_string = '2016-05-08T00:30:00'

        a = coords.datetime(a_date_string)
        b = a - 1.0/24

        self.assertEqual('2016-05-07T23:30:00', str(b))

        return


    # inTimezone


    def test_inTimezone_a(self):
        """Test inTimezone shift 1 to 2"""

        a = coords.datetime('2019-10-08T02:30:00+01:00')

        b = a.inTimezone("02:00")

        self.assertEqual('2019-10-08T03:30:00+02:00', str(b))

        return


    def test_inTimezone_b(self):
        """Test inTimezone shift 5:50 to -8 """

        a = coords.datetime('2019-10-08T02:30:00+0530')

        b = a.inTimezone("-8")

        self.assertEqual('2019-10-07T13:00:00-0800', str(b))

        return


    def test_inTimezoneOffset_b(self):
        """Test inTimezone shift 5:50 to -8 """

        a = coords.datetime('2019-10-08T02:30:00+0530')

        b = a.inTimezoneOffset(-8)

        self.assertEqual('2019-10-07T13:00:00-0800', str(b))

        return



if __name__ == '__main__':
    random.seed(time.time())
    unittest.main()
