// ================================================================
// Filename:    datetime_unittest.cpp
// Description: This is the gtest unittest of the datetime library.
//
// Author:      L.R. McFarland, lrm@starbug.com
// Created:     2014 Jun 19
// Language:    C++
//
//  datetime is free software: you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  datetime is distributed in the hope that it will be useful, but
//  WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Coordinates.  If not, see <http://www.gnu.org/licenses/>.
// ================================================================

#include <iomanip> // for std::setw() and std::setfill()
#include <sstream>

#include <gtest/gtest.h>

#include <datetime.h>

namespace {

  // ---------------------
  // ----- constants -----
  // ---------------------

  TEST(DateTime, AccessorLilianDate) {
    Coords::DateTime a_datetime("1582-10-15T00:00:00");
    EXPECT_DOUBLE_EQ(2299160.5, Coords::DateTime::s_LilianDate);
    EXPECT_DOUBLE_EQ(2299160.5, a_datetime.toJulianDate());
    EXPECT_DOUBLE_EQ(2299160.5, a_datetime.LilianDate());
  }

  TEST(DateTime, AccessorModifiedJulianDate) {
    Coords::DateTime a_datetime("1858-11-17T00:00:00");
    EXPECT_DOUBLE_EQ(2400000.5, Coords::DateTime::s_ModifiedJulianDate);
    EXPECT_DOUBLE_EQ(2400000.5, a_datetime.toJulianDate());
    EXPECT_DOUBLE_EQ(2400000.5, a_datetime.ModifiedJulianDate());
  }

  TEST(DateTime, AccessorTruncatedJulianDate) {
    Coords::DateTime a_datetime("1968-05-24T00:00:00");
    EXPECT_DOUBLE_EQ(2440000.5, Coords::DateTime::s_TruncatedJulianDate);
    EXPECT_DOUBLE_EQ(2440000.5, a_datetime.toJulianDate());
    EXPECT_DOUBLE_EQ(2440000.5, a_datetime.TruncatedJulianDate());
  }

  TEST(DateTime, AccessorJ2000) {
    Coords::DateTime a_datetime("2000-01-01T12:00:00Z");
    EXPECT_DOUBLE_EQ(2451545.0, Coords::DateTime::s_J2000);
    EXPECT_DOUBLE_EQ(2451545.0, a_datetime.toJulianDate());
    EXPECT_DOUBLE_EQ(2451545.0, a_datetime.J2000());
  }

  // ------------------------
  // ----- constructors -----
  // ------------------------

  TEST(DateTime, DefaultConstructor) {
    Coords::DateTime a_datetime;
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ("1970-01-01T00:00:00", out.str().c_str());
  }

  TEST(DateTime, ParameterConstructorWithTimeZone_pos) {
    Coords::DateTime a_datetime(2014, 12, 8, 13, 30, 00, 4.3);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ("2014-12-08T13:30:00+4.3", out.str().c_str());
  }

  TEST(DateTime, ParameterConstructorWithTimeZone_neg) {
    Coords::DateTime a_datetime(2014, 12, 8, 13, 30, 00, -5.1);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ("2014-12-08T13:30:00-5.1", out.str().c_str());
  }


  TEST(DateTime, ParameterConstructorWithBadMonth_1) {
    try {
      Coords::DateTime a_datetime(2014, -2, 8, 13, 30, 00, -5.1);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "2014--2-08T13:30:00-5.1: month out of range.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, ParameterConstructorWithBadMonth_2) {
    try {
      Coords::DateTime a_datetime(2014, 13, 8, 13, 30, 00, -5.1);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "2014-13-08T13:30:00-5.1: month out of range.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, ParameterConstructorWithBadDay_1) {
    try {
      Coords::DateTime a_datetime(2014, 2, -4, 13, 30, 00, -5.1);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "2014-02--4T13:30:00-5.1: day out of range.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, ParameterConstructorWithBadDay_2) {
    try {
      Coords::DateTime a_datetime(2014, 12, 33, 13, 30, 00, -5.1);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "2014-12-33T13:30:00-5.1: day out of range.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, ParameterConstructorWithBadHour_1) {
    try {
      Coords::DateTime a_datetime(2014, 2, 4, -1, 30, 00, -5.1);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "2014-02-04T-1:30:00-5.1: hour out of range.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, ParameterConstructorWithBadHour_2) {
    try {
      Coords::DateTime a_datetime(2014, 12, 03, 60, 30, 00, -5.1);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "2014-12-03T60:30:00-5.1: hour out of range.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }


  TEST(DateTime, ParameterConstructorWithBadMinute_1) {
    try {
      Coords::DateTime a_datetime(2014, 2, 4, 13, -3, 00, -5.1);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "2014-02-04T13:-3:00-5.1: minute out of range.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, ParameterConstructorWithBadMinute_2) {
    try {
      Coords::DateTime a_datetime(2014, 12, 03, 13, 61, 00, -5.1);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "2014-12-03T13:61:00-5.1: minute out of range.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, ParameterConstructorWithBadSecond_1) {
    try {
      Coords::DateTime a_datetime(2014, 2, 4, 13, 30, -1, -5.1);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "2014-02-04T13:30:-1-5.1: second out of range.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, ParameterConstructorWithBadSecond_2) {
    try {
      Coords::DateTime a_datetime(2014, 12, 03, 13, 30, 60, -5.1);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "2014-12-03T13:31:00-5.1: second out of range.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, ParameterConstructorWithBadTimeZone_1) {
    try {
      Coords::DateTime a_datetime(2014, 2, 4, 13, 30, 1.5, -15.1);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "2014-02-04T13:30:1.5-15.1: time zone out of range.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, ParameterConstructorWithBadTimeZone_2) {
    try {
      Coords::DateTime a_datetime(2014, 12, 03, 13, 30, 59, 15.1);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "2014-12-03T13:30:59+15.1: time zone out of range.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, CopyConstructor_1) {
    Coords::DateTime a_datetime(2014, 12, 8, 13, 30, 00, 4.3);
    Coords::DateTime another_datetime(a_datetime);

    std::stringstream an_out;
    an_out << a_datetime;

    std::stringstream another_out;
    another_out << another_datetime;

    EXPECT_STREQ(an_out.str().c_str(), another_out.str().c_str());
  }

  TEST(DateTime, CopyAssign_1) {
    Coords::DateTime a_datetime(2014, 12, 8, 13, 30, 00, 4.3);
    Coords::DateTime another_datetime;

    another_datetime = a_datetime;

    std::stringstream an_out;
    an_out << a_datetime;

    std::stringstream another_out;
    another_out << another_datetime;

    EXPECT_STREQ(an_out.str().c_str(), another_out.str().c_str());
  }

  // months

  TEST(DateTime, GoodMonthConstructors) {
    for (int i(1); i < 13; ++i) {
      std::stringstream a_date_string;
      a_date_string << "2014-" << std::setw(2) << std::setfill('0') << i << "-01T12:34:56";
      Coords::DateTime a_datetime(a_date_string.str());
      std::stringstream out;
      out << a_datetime;
      EXPECT_STREQ(a_date_string.str().c_str(), out.str().c_str());
      EXPECT_DOUBLE_EQ(0, a_datetime.timeZone());
    }
  }

  TEST(DateTime, BadMonthConstructor_0) {
    std::string a_date_string("2014-00-07T12:34:56");
    try {
      Coords::DateTime a_datetime(a_date_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_date_string << " not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][Z|(+|-)hh[:][mm]]";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, BadMonthConstructor_13) {
    std::string a_date_string("2014-13-07T12:34:56");
    try {
      Coords::DateTime a_datetime(a_date_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_date_string << " not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][Z|(+|-)hh[:][mm]]";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }


  // days

  TEST(DateTime, GoodDayConstructors_jan) {
    for (int i(1); i < 32; ++i) {
      std::stringstream a_date_string;
      a_date_string << "2014-01-" << std::setw(2) << std::setfill('0') << i << "T12:34:56";
      Coords::DateTime a_datetime(a_date_string.str());
      std::stringstream out;
      out << a_datetime;
      EXPECT_STREQ(a_date_string.str().c_str(), out.str().c_str());
      EXPECT_DOUBLE_EQ(0, a_datetime.timeZone());
    }
  }

  TEST(DateTime, GoodDayConstructors_sep) {
    // Thirty days hath September
    for (int i(1); i < 31; ++i) {
      std::stringstream a_date_string;
      a_date_string << "2014-09-" << std::setw(2) << std::setfill('0') << i << "T12:34:56";
      Coords::DateTime a_datetime(a_date_string.str());
      std::stringstream out;
      out << a_datetime;
      EXPECT_STREQ(a_date_string.str().c_str(), out.str().c_str());
      EXPECT_DOUBLE_EQ(0, a_datetime.timeZone());
    }
  }


  TEST(DateTime, BadDayConstructor_0) {
    std::string a_date_string("2014-12-00T12:34:56");
    try {
      Coords::DateTime a_datetime(a_date_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_date_string << " not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][Z|(+|-)hh[:][mm]]";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, BadDayConstructor_32) {
    std::string a_date_string("2014-12-32T12:34:56");
    try {
      Coords::DateTime a_datetime(a_date_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_date_string << " not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][Z|(+|-)hh[:][mm]]";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, BadDayConstructor_leap_year_1) {
    // leap year
    Coords::DateTime good_date("2012-02-29T12:34:56");
    std::string bad_date("2012-02-30T12:34:56");
    try {
      Coords::DateTime a_datetime(bad_date);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << bad_date << ": Except for February all alone. It has 28, but 29 each _leap_ year.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, BadDayConstructor_leap_year_2) {
    // not a leap year
    Coords::DateTime good_date("2014-02-28T12:34:56");
    std::string bad_date("2014-02-29T12:34:56");
    try {
      Coords::DateTime a_datetime(bad_date);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << bad_date << ": Except for February all alone. It has _28_, but 29 each leap year.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, BadDayConstructor_leap_year_3) {
    // leap year mod 400 rule
    Coords::DateTime good_date("2000-02-29T12:34:56");
    std::string bad_date("2000-02-30T12:34:56");
    try {
      Coords::DateTime a_datetime(bad_date);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << bad_date << ": Except for February all alone. It has 28, but 29 each _leap_ year.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, BadDayConstructor_leap_year_4) {
    // not a leap year
    Coords::DateTime good_date("2100-02-28T12:34:56");
    std::string bad_date("2100-02-29T12:34:56");
    try {
      Coords::DateTime a_datetime(bad_date);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << bad_date << ": Except for February all alone. It has _28_, but 29 each leap year.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }


  TEST(DateTime, BadDayConstructor_apr31) {
    std::string a_date_string("2014-04-31T12:34:56");
    try {
      Coords::DateTime a_datetime(a_date_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_date_string << ": Thirty days hath September, April, June and November";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, BadDayConstructor_jun31) {
    std::string a_date_string("2014-06-31T12:34:56");
    try {
      Coords::DateTime a_datetime(a_date_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_date_string << ": Thirty days hath September, April, June and November";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, BadDayConstructor_sep31) {
    std::string a_date_string("2014-09-31T12:34:56");
    try {
      Coords::DateTime a_datetime(a_date_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_date_string << ": Thirty days hath September, April, June and November";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, BadDayConstructor_nov31) {
    std::string a_date_string("2014-11-31T12:34:56");
    try {
      Coords::DateTime a_datetime(a_date_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_date_string << ": Thirty days hath September, April, June and November";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }


  // hours

  TEST(DateTime, GoodHourConstructors) {
    for (int i(0); i < 24; ++i) {
      std::stringstream a_date_string;
      a_date_string << "2014-01-01T" << std::setw(2) << std::setfill('0') << i << ":34:56";
      Coords::DateTime a_datetime(a_date_string.str());
      std::stringstream out;
      out << a_datetime;
      EXPECT_STREQ(a_date_string.str().c_str(), out.str().c_str());
      EXPECT_DOUBLE_EQ(0, a_datetime.timeZone());
    }
  }


  TEST(DateTime, BadHourConstructor_1) {
    std::string a_date_string("2014-12-31T24:34:56");
    try {
      Coords::DateTime a_datetime(a_date_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_date_string << " not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][Z|(+|-)hh[:][mm]]";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  // minutes

  TEST(DateTime, GoodMinuteConstructors) {
    for (int i(0); i < 60; ++i) {
      std::stringstream a_date_string;
      a_date_string << "2014-01-01T00:" << std::setw(2) << std::setfill('0') << i << ":56";
      Coords::DateTime a_datetime(a_date_string.str());
      std::stringstream out;
      out << a_datetime;
      EXPECT_STREQ(a_date_string.str().c_str(), out.str().c_str());
      EXPECT_DOUBLE_EQ(0, a_datetime.timeZone());
    }
  }

  TEST(DateTime, BadMinuteConstructor_1) {
    std::string a_date_string("2014-12-31T10:62:56");
    try {
      Coords::DateTime a_datetime(a_date_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_date_string << " not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][Z|(+|-)hh[:][mm]]";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  // seconds

  TEST(DateTime, GoodSecondConstructors) {
    for (int i(0); i < 60; ++i) {
      std::stringstream a_date_string;
      a_date_string << "2014-01-01T00:00:" << std::setw(2) << std::setfill('0') << i;
      Coords::DateTime a_datetime(a_date_string.str());
      std::stringstream out;
      out << a_datetime;
      EXPECT_STREQ(a_date_string.str().c_str(), out.str().c_str());
      EXPECT_DOUBLE_EQ(0, a_datetime.timeZone());
    }
  }

  TEST(DateTime, BadSecondConstructor_1) {
    std::string a_date_string("2014-12-31T10:12:66");
    try {
      Coords::DateTime a_datetime(a_date_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_date_string << " not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][Z|(+|-)hh[:][mm]]";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }


  // time zones

  TEST(DateTime, GoodTimeZoneConstructor_Zulu) {
    std::string a_date_string("2014-12-07T12:34:56.78Z");
    Coords::DateTime a_datetime(a_date_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }


  TEST(DateTime, GoodTimeZoneConstructors_pos_hrs) {
    for (int i(1); i < 13; ++i) {
      std::stringstream a_date_string;
      a_date_string << "2014-03-15T12:34:56+" << std::setw(2) << std::setfill('0') << i;
      Coords::DateTime a_datetime(a_date_string.str());
      std::stringstream out;
      out << a_datetime;
      EXPECT_STREQ(a_date_string.str().c_str(), out.str().c_str());
      EXPECT_DOUBLE_EQ(i, a_datetime.timeZone());
    }
  }

  TEST(DateTime, GoodTimeZoneConstructors_neg_hrs) {
    for (int i(1); i < 13; ++i) {
      std::stringstream a_date_string;
      a_date_string << "2014-03-15T12:34:56-" << std::setw(2) << std::setfill('0') << i;
      Coords::DateTime a_datetime(a_date_string.str());
      std::stringstream out;
      out << a_datetime;
      EXPECT_STREQ(a_date_string.str().c_str(), out.str().c_str());
      EXPECT_DOUBLE_EQ(-i, a_datetime.timeZone());
    }
  }

  TEST(DateTime, GoodTimeZoneConstructors_mins) {
    for (int i(0); i < 60; ++i) {
      std::stringstream a_date_string;
      a_date_string << "2014-03-15T12:34:56+08:" << std::setw(2) << std::setfill('0') << i;
      Coords::DateTime a_datetime(a_date_string.str());
      std::stringstream out;
      out << a_datetime;
      EXPECT_STREQ(a_date_string.str().c_str(), out.str().c_str());
    }
  }


  TEST(DateTime, BadTimeZoneConstructor_1) {
    std::string a_date_string("2014-12-07T12:34:56.78+13.987");
    try {
      Coords::DateTime a_datetime(a_date_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_date_string << " not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][Z|(+|-)hh[:][mm]]";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, BadTimeZoneConstructor_2) {
    std::string a_date_string("2014-12-07T12:34:56.78-13.987");
    try {
      Coords::DateTime a_datetime(a_date_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_date_string << " not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][Z|(+|-)hh[:][mm]]";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }


  TEST(DateTime, GoodTimeZoneConstructor_1) {
    std::string a_date_string("2014-12-07T12:34:56.78+04");
    Coords::DateTime a_datetime(a_date_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }


  TEST(DateTime, GoodTimeZoneConstructor_2) {
    std::string a_date_string("2014-12-07T12:34:56.78+0430");
    Coords::DateTime a_datetime(a_date_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
    EXPECT_DOUBLE_EQ(4.5, a_datetime.timeZone());
  }

  TEST(DateTime, GoodTimeZoneConstructor_3) {
    std::string a_date_string("2014-12-07T12:34:56.78+04:15");
    Coords::DateTime a_datetime(a_date_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
    EXPECT_DOUBLE_EQ(4.25, a_datetime.timeZone());
  }

  TEST(DateTime, GoodTimeZoneConstructor_4) {
    std::string a_date_string("2014-12-07T12:34:56.78-04");
    Coords::DateTime a_datetime(a_date_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }


  TEST(DateTime, GoodTimeZoneConstructor_5) {
    std::string a_date_string("2014-12-07T12:34:56.78-0430");
    Coords::DateTime a_datetime(a_date_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
    EXPECT_DOUBLE_EQ(-4.5, a_datetime.timeZone());
  }

  TEST(DateTime, GoodTimeZoneConstructor_6) {
    std::string a_date_string("2014-12-07T12:34:56.78-04:45");
    Coords::DateTime a_datetime(a_date_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
    EXPECT_DOUBLE_EQ(-4.75, a_datetime.timeZone());
  }

  TEST(DateTime, LetsGetBiblical_1) {
    std::string a_date_string("-5579-03-20T12:00:00");
    Coords::DateTime a_datetime(a_date_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }

  // ------------------------
  // ----- Julian dates -----
  // ------------------------

  // Actual dates taken from http://www.imcce.fr/en/grandpublic/temps/jour_julien.php

  // from http://en.wikipedia.org/wiki/Julian_day
  // JDN 0: noon Jan 01, 4713 BC proleptic Julian calendar,
  //             Nov 24, 4714 BC proleptic Gregorian calendar
  //
  // from http://www.imcce.fr/en/grandpublic/temps/jour_julien.php
  // JDN 0: noon Jan 01, 4712 BC
  //
  // Lilian date: Oct 15, 1582

  // ---------------------------------
  // ----- Julian Date Wikipedia -----
  // ---------------------------------

  TEST(DateTime, JulianDateWiki_0) {
    std::string a_date_string("-4712-01-01T12:00:00");

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(0.5, a_datetime.toJulianDateWiki()); // rounds to day

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateWiki(a_datetime.toJulianDateWiki());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("-4713-11-25T12:00:00", out.str().c_str()); // TODO Gregorian day 0 + year
  }

  TEST(DISABLED_DateTime, JulianDateWiki_Julian_0) {

    // TODO this is off by a day in calculation and switchs to Gregorian on reversal

    std::string a_date_string("-4713-01-01T12:00:00"); // Julian day 0

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(0.5, a_datetime.toJulianDateWiki()); // TODO off by a year -364.5

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateWiki(a_datetime.toJulianDateWiki());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("-4714-11-25T12:00:00", out.str().c_str()); // Gregorian day 0
  }

  TEST(DISABLED_DateTime, JulianDateWiki_Gregorian_0) {

    // TODO this is off by a day

    std::string a_date_string("-4714-11-24T12:00:00"); // Gregorian day 0

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(0.5, a_datetime.toJulianDateWiki()); // ok

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateWiki(a_datetime.toJulianDateWiki());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("-4714-11-25T12:00:00", out.str().c_str()); // Gregorian day 0 - 1
  }


  TEST(DateTime, JulianDateWiki_pre_Julian2Gregorian) {
    std::string a_date_string("1582-10-14T00:00:00"); // Gregorian replaces Julian

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_NEAR(a_datetime.s_LilianDate + 10, a_datetime.toJulianDateWiki(), 0.5);
    // Does not account for 10 day "gap" in calendar change.

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateWiki(a_datetime.toJulianDateWiki());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("1582-10-24T00:00:00", out.str().c_str());
  }

  TEST(DateTime, JulianDateWiki_post_Julian2Gregorian) {
    std::string a_date_string("1582-10-15T00:00:00"); // Gregorian replaces Julian

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_NEAR(a_datetime.s_LilianDate, a_datetime.toJulianDateWiki(), 0.5);
    // toJulianDateWiki() rounds to nearest day

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateWiki(a_datetime.toJulianDateWiki());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }

  TEST(DISABLED_DateTime, JulianDateWiki_pre_modified) {

    // TODO off by a day

    std::string a_date_string("1858-11-16T12:00:00"); // modified date change switches from noon to midnight

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_NEAR(Coords::DateTime::s_ModifiedJulianDate, a_datetime.toJulianDateWiki(), 0.5);
   // toJulianDateWiki() rounds to nearest day

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateWiki(a_datetime.toJulianDateWiki());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str()); // rounds to midnight
  }

  TEST(DateTime, JulianDateWiki_post_modified) {
    std::string a_date_string("1858-11-17T00:00:00"); // modified date change switches from noon to midnight

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_NEAR(Coords::DateTime::s_ModifiedJulianDate, a_datetime.toJulianDateWiki(), 0.5);
   // toJulianDateNRC() rounds to nearest day

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateNRC(a_datetime.toJulianDateWiki());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }


  TEST(DateTime, JulianDateWiki_truncated) {
    std::string a_date_string("1968-05-24T00:00:00"); // truncated Julian date

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_NEAR(Coords::DateTime::s_TruncatedJulianDate, a_datetime.toJulianDateWiki(), 0.5);
   // toJulianDateWiki() rounds to nearest day

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateWiki(a_datetime.toJulianDateWiki());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str()); // rounds to midnight
  }

  TEST(DISABLED_DateTime, JulianDateWiki_J2000) {

    // TODO off by half a day.

    std::string a_date_string("2000-01-01T00:00:00"); // J2000

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(2451544.5, a_datetime.toJulianDateWiki());

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateWiki(a_datetime.toJulianDateWiki());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }


  TEST(DateTime, JulianDateWiki_3) {
    std::string a_date_string("2013-01-01T00:30:00");

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_NEAR(2456293.5208333335, a_datetime.toJulianDateWiki(), 0.500001);
    // toJulianDateWiki() rounds to nearest day. Does not start at noon.

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateWiki(a_datetime.toJulianDateWiki());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, JulianDateWiki_4) {
    std::string a_date_string("2014-12-09T00:00:00");

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_NEAR(2457000.5, a_datetime.toJulianDateWiki(), 0.5);
    // rounds to nearest day

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateWiki(a_datetime.toJulianDateWiki());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }

  // ----------------------------
  // ----- Julian dates NRC -----
  // ----------------------------

  TEST(DateTime, JulianDateNRC_0) {
    std::string a_date_string("-4712-01-01T12:00:00");

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(366.5, a_datetime.toJulianDateNRC()); // TODO off by a year

    // TODO Does not account for year 0?

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("-4712-01-01T00:00:00", out.str().c_str());
    // midnight not noon
  }


  TEST(DISABLED_DateTime, JulianDateNRC_Julian_0) {

    // TODO this is reading Julian in and Gregorian out

    std::string a_date_string("-4713-01-01T12:00:00"); // Julian day 0

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(0.5, a_datetime.toJulianDateNRC()); // ok

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("-4714-11-25T12:00:00", out.str().c_str()); // Gregorian day 0
  }

  TEST(DISABLED_DateTime, JulianDateNRC_Gregorian_0) {

    // TODO this is reading Gregorian in and Gregorian out, but is several days off

    std::string a_date_string("-4714-11-24T12:00:00"); // Gregorian day 0

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(0.5, a_datetime.toJulianDateNRC()); // TODO Actual -37.5

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str()); // ok
  }


  TEST(DateTime, JulianDateNRC_pre_Julian2Gregorian) {
    std::string a_date_string("1582-10-14T00:00:00"); // Gregorian replaces Julian

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_NEAR(a_datetime.s_LilianDate + 10, a_datetime.toJulianDateNRC(), 0.5);
    // does not account for 10 day "gap" in calendar change.

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("1582-10-24T00:00:00", out.str().c_str());
  }

  TEST(DateTime, JulianDateNRC_post_Julian2Gregorian) {
    std::string a_date_string("1582-10-15T00:00:00"); // Gregorian replaces Julian

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_NEAR(a_datetime.s_LilianDate, a_datetime.toJulianDateNRC(), 0.5);
    // toJulianDateNRC() rounds to nearest day

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, JulianDateNRC_pre_modified) {
    std::string a_date_string("1858-11-16T12:00:00"); // modified date change switches from noon to midnight

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_NEAR(Coords::DateTime::s_ModifiedJulianDate, a_datetime.toJulianDateWiki(), 0.5);
   // toJulianDateNRC() rounds to nearest day

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("1858-11-16T00:00:00", out.str().c_str());
    // switches noon to midnight
  }


  TEST(DateTime, JulianDateNRC_post_modified) {
    std::string a_date_string("1858-11-17T00:00:00"); // modified date change switches from noon to midnight

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_NEAR(Coords::DateTime::s_ModifiedJulianDate, a_datetime.toJulianDateNRC(), 0.5);
   // toJulianDateNRC() rounds to nearest day

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, JulianDateNRC_truncated) {
    std::string a_date_string("1968-05-24T00:00:00"); // truncated Julian date

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_NEAR(Coords::DateTime::s_TruncatedJulianDate, a_datetime.toJulianDateNRC(), 0.5);
   // toJulianDateNRC() rounds to nearest day

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }

  TEST(DISABLED_DateTime, JulianDateNRC_J2000) {

    // TODO off by half a day.

    std::string a_date_string("2000-01-01T00:00:00"); // J2000

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(2451544.5, a_datetime.toJulianDateNRC());

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }


  TEST(DateTime, JulianDateNRC_3) {
    std::string a_date_string("2013-01-01T00:30:00");

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_NEAR(2456293.5208333335, a_datetime.toJulianDateNRC(), 0.500001);
    // toJulianDateNRC() rounds to nearest day. Does not start at noon.

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("2013-01-01T00:00:00", out.str().c_str()); // rounds to midnight
  }

  TEST(DateTime, JulianDateNRC_4) {
    std::string a_date_string("2014-12-09T00:00:00");

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_NEAR(2457000.5, a_datetime.toJulianDateNRC(), 0.5);
    // toJulianDateNRC() rounds to nearest day

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }

  // -------------------------------------
  // ----- Modified Julian Dates APC -----
  // -------------------------------------

  TEST(DateTime, ModifiedJulianDateAPC_0) {
    std::string a_date_string("-4712-01-01T12:00:00");

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(-Coords::DateTime::s_ModifiedJulianDate, a_datetime.toModifiedJulianDateAPC());
    // TODO asAPCJulianDate() differs from IMCCE on noon vs. midnight on this date?

    Coords::DateTime another_datetime;
    another_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }

  TEST(DISABLED_DateTime, ModifiedJulianDateAPC_Julian_0) {

    // TODO calculation adds year zero, output rounds by a day

    std::string a_date_string("-4713-01-01T12:00:00"); // Julian day 0

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(-Coords::DateTime::s_ModifiedJulianDate - 365, a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime;
    another_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str()); // rounds by a day
  }

  TEST(DISABLED_DateTime, ModifiedJulianDateAPC_Gregorian_0) {

    // TODO calculation is off by 403, output by a day

    std::string a_date_string("-4714-11-24T12:00:00"); // Gregorian day 0

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(-Coords::DateTime::s_ModifiedJulianDate, a_datetime.toModifiedJulianDateAPC()); // Actual - 403

    Coords::DateTime another_datetime;
    another_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }


  TEST(DateTime, ModifiedJulianDateAPC_pre_Julian2Gregorian) {
    std::string a_date_string("1582-10-14T00:00:00"); // Gregorian replaces Julian

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(a_datetime.s_LilianDate - Coords::DateTime::s_ModifiedJulianDate - 1, // rounding
		     a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime;
    another_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("1582-10-04T00:00:00", out.str().c_str()); // skips 10 days
  }

  TEST(DateTime, ModifiedJulianDateAPC_post_Julian2Gregorian) {
    std::string a_date_string("1582-10-15T00:00:00"); // Gregorian replaces Julian

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(a_datetime.s_LilianDate - Coords::DateTime::s_ModifiedJulianDate,
		     a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime;
    another_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }


  TEST(DateTime, ModifiedJulianDateAPC_pre_modified) {
    std::string a_date_string("1858-11-16T12:00:00"); // modified date change switches from noon to midnight

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(-0.5, a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime;
    another_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, ModifiedJulianDateAPC_post_modified) {
    std::string a_date_string("1858-11-17T00:00:00"); // modified date change switches from noon to midnight

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(0, a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime;
    another_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, ModifiedJulianDateAPC_truncated) {
    std::string a_date_string("1968-05-24T00:00:00"); // truncated Julian date

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(Coords::DateTime::s_TruncatedJulianDate - Coords::DateTime::s_ModifiedJulianDate,
		     a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime;
    another_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, ModifiedJulianDateAPC_J2000) {

    std::string a_date_string("2000-01-01T00:00:00"); // J2000

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(2451544.5 - Coords::DateTime::s_ModifiedJulianDate,
		     a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime;
    another_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, ModifiedJulianDateAPC_J2000_13) {

    // tests 60 seconds rounds up a minute

    std::string a_date_string("2000-01-01T13:00:00"); // J2000

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(51544.541666666664, a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime;
    another_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, ModifiedJulianDateAPC_3) {
    std::string a_date_string("2013-01-01T00:30:00");

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_NEAR(2456293.5208333335 - Coords::DateTime::s_ModifiedJulianDate,
		a_datetime.toModifiedJulianDateAPC(), 0.000000001); // very near

    Coords::DateTime another_datetime;
    another_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, ModifiedJulianDateAPC_4) {
    std::string a_date_string("2014-12-09T00:00:00");

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(2457000.5 - Coords::DateTime::s_ModifiedJulianDate, a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime;
    another_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }


  // ---------------------------------
  // ----- Julian Date operators -----
  // ---------------------------------

  TEST(DateTime, JulianDate_0) {
    std::string a_date_string("-4712-01-01T12:00:00");

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(0.0, a_datetime.toJulianDate());

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDate(a_datetime.toJulianDate());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }


  TEST(DateTime, JulianDate_2014_12_09) {
    std::string a_date_string("2014-12-09T00:00:00");

    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(2457000.5, a_datetime.toJulianDate());

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDate(a_datetime.toJulianDate());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, operator_plus_eq_1) {
    std::string a_date_string("2014-12-09T00:00:00");

    Coords::DateTime a_datetime(a_date_string);

    a_datetime += 1;

    EXPECT_DOUBLE_EQ(2457000.5 + 1, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2014-12-10T00:00:00", out.str().c_str());
  }

  // on edge of day transition
  TEST(DateTime, operator_plus_eq_00_tz1) {
    std::string a_date_string("2014-12-09T00:00:00+0100");

    Coords::DateTime a_datetime(a_date_string);

    a_datetime += 1;

    EXPECT_DOUBLE_EQ(2457001.5416666665, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2014-12-10T00:00:00+0100", out.str().c_str());
  }

  TEST(DateTime, operator_plus_eq_01_tz2) {
    std::string a_date_string("2014-12-09T01:00:00+0200");

    Coords::DateTime a_datetime(a_date_string);

    a_datetime += 1;

    EXPECT_DOUBLE_EQ(2457001.625, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2014-12-10T01:00:00+0200", out.str().c_str());
  }

  TEST(DateTime, operator_plus_eq_00_ntz1) {
    std::string a_date_string("2014-12-09T00:00:00-0100");

    Coords::DateTime a_datetime(a_date_string);

    a_datetime += 1;

    EXPECT_DOUBLE_EQ(2457001.4583333335, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2014-12-10T00:00:00-0100", out.str().c_str());
  }

  TEST(DateTime, operator_plus_eq_23_ntz1) {
    std::string a_date_string("2014-12-09T23:00:00-0100");

    Coords::DateTime a_datetime(a_date_string);

    a_datetime += 1;

    EXPECT_DOUBLE_EQ(2457002.4166666665, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2014-12-10T23:00:00-0100", out.str().c_str());
  }

  TEST(DateTime, operator_plus_eq_30) {
    std::string a_date_string("2014-12-09T14:30:00");

    Coords::DateTime a_datetime(a_date_string);

    a_datetime += 30;

    EXPECT_DOUBLE_EQ(2457031.1041666665, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2015-01-08T14:30:00", out.str().c_str());
  }

  TEST(DateTime, operator_minus_eq_1) {
    std::string a_date_string("2014-12-09T00:00:00");

    Coords::DateTime a_datetime(a_date_string);

    a_datetime -= 1;

    EXPECT_DOUBLE_EQ(2457000.5 - 1, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2014-12-08T00:00:00", out.str().c_str());
  }

  // on edge of day transition
  TEST(DateTime, operator_minus_eq_00_tz1) {
    std::string a_date_string("2014-12-09T00:00:00+0100");

    Coords::DateTime a_datetime(a_date_string);

    a_datetime -= 1;

    EXPECT_DOUBLE_EQ(2456999.5416666665, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2014-12-08T00:00:00+0100", out.str().c_str());
  }

  // on ednge of day transition
  TEST(DateTime, operator_minus_eq_00_ntz1) {
    std::string a_date_string("2014-12-09T00:00:00-0100");

    Coords::DateTime a_datetime(a_date_string);

    a_datetime -= 1;

    EXPECT_DOUBLE_EQ(2456999.4583333335, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2014-12-08T00:00:00-0100", out.str().c_str());
  }

  TEST(DateTime, operator_minus_eq_30) {
    std::string a_date_string("2014-12-09T14:00:00");

    Coords::DateTime a_datetime(a_date_string);

    a_datetime -= 30;

    EXPECT_DOUBLE_EQ(2456971.0833333335, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2014-11-09T14:00:00", out.str().c_str());
  }

  TEST(DateTime, operator_date_plus_1) {
    std::string a_date_string("2014-12-09T00:00:00");

    Coords::DateTime a_datetime(a_date_string);
    Coords::DateTime another_datetime;

    another_datetime = a_datetime + 1;

    EXPECT_DOUBLE_EQ(2457000.5 + 1, another_datetime.toJulianDate());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("2014-12-10T00:00:00", out.str().c_str());
  }

  TEST(DateTime, operator_1_plus_date) {
    std::string a_date_string("2014-12-09T00:00:00");

    Coords::DateTime a_datetime(a_date_string);
    Coords::DateTime another_datetime;

    another_datetime = 1 + a_datetime;

    EXPECT_DOUBLE_EQ(2457000.5 + 1, another_datetime.toJulianDate());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("2014-12-10T00:00:00", out.str().c_str());
  }

  TEST(DateTime, operator_date_plus_30) {
    std::string a_date_string("2014-12-09T14:50:00");

    Coords::DateTime a_datetime(a_date_string);
    Coords::DateTime another_datetime;

    another_datetime = a_datetime + 30;

    EXPECT_DOUBLE_EQ(2457001.1180555555, a_datetime.toJulianDate());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("2015-01-08T14:50:00", out.str().c_str());
  }


  TEST(DateTime, operator_30_plus_date) {
    std::string a_date_string("2014-12-09T14:50:00");

    Coords::DateTime a_datetime(a_date_string);
    Coords::DateTime another_datetime;

    another_datetime = 30 + a_datetime;

    EXPECT_DOUBLE_EQ(2457001.1180555555, a_datetime.toJulianDate());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("2015-01-08T14:50:00", out.str().c_str());
  }


  TEST(DateTime, operator_date_minus_1) {
    std::string a_date_string("2014-12-09T00:00:00");

    Coords::DateTime a_datetime(a_date_string);
    Coords::DateTime another_datetime;

    another_datetime = a_datetime - 1;

    EXPECT_DOUBLE_EQ(2457000.5 - 1, another_datetime.toJulianDate());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("2014-12-08T00:00:00", out.str().c_str());
  }


  TEST(DateTime, operator_date_minus_30) {
    std::string a_date_string("2014-12-09T14:50:00");

    Coords::DateTime a_datetime(a_date_string);
    Coords::DateTime another_datetime;

    another_datetime = a_datetime - 30;

    EXPECT_DOUBLE_EQ(2457001.1180555555, a_datetime.toJulianDate());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("2014-11-09T14:50:00", out.str().c_str());
  }


  TEST(DateTime, operator_date_minus_date_1) {

    Coords::DateTime a_datetime("2014-12-01T00:00:00");
    Coords::DateTime b_datetime("2015-01-01T00:00:00");

    double days = b_datetime - a_datetime;

    EXPECT_DOUBLE_EQ(31, days);

  }

  TEST(DateTime, operator_date_minus_date_2) {

    Coords::DateTime a_datetime("2014-01-01T00:00:00");
    Coords::DateTime b_datetime("2015-01-01T00:00:00");

    double days = b_datetime - a_datetime;

    EXPECT_DOUBLE_EQ(365, days);

  }

  TEST(DateTime, operator_date_minus_date_3) {
    // leap year
    Coords::DateTime a_datetime("2012-01-01T00:00:00");
    Coords::DateTime b_datetime("2013-01-01T00:00:00");

    double days = b_datetime - a_datetime;

    EXPECT_DOUBLE_EQ(366, days);

  }



} // end anonymous namespace



// ==================
// ===== main() =====
// ==================

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
