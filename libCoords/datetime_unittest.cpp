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
    EXPECT_STREQ("1970-01-01T00:00:00.0", out.str().c_str());
  }


  TEST(DateTime, ParameterConstructorWithTimeZone_as_string_z) {
    Coords::DateTime a_datetime("2019-09-18T17:30:00z");
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ("2019-09-18T17:30:00.0Z", out.str().c_str());
  }


  TEST(DateTime, ParameterConstructorWithTimeZone_as_string_Z) {
    Coords::DateTime a_datetime("2019-09-18T17:30:00Z");
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ("2019-09-18T17:30:00.0Z", out.str().c_str());
  }


  TEST(DateTime, ParameterConstructorWithTimeZone_as_string_Z_no_seconds) {
    std::string a_datetime_string("2014-12-31T10:62:56");

    try {
      Coords::DateTime a_datetime(a_datetime_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_datetime_string << " not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][z|Z|[+|-]hh[[:]mm]]";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }

  }


  TEST(DateTime, ParameterConstructorWithTimeZone_as_string_1) {
    Coords::DateTime a_datetime("2014-12-08T13:30:00+05:00");
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ("2014-12-08T13:30:00.0+05:00", out.str().c_str());
  }


  TEST(DateTime, ParameterConstructorWithTimeZone_as_string_2) {
    Coords::DateTime a_datetime("2014-12-08T13:30:00+0530");
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ("2014-12-08T13:30:00.0+0530", out.str().c_str());
  }


  TEST(DateTime, ParameterConstructorWithTimeZone_as_jdate_1) {
    Coords::DateTime a_datetime(2458743.32308);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ("2019-09-16T19:45:14.1", out.str().c_str());
  }


  TEST(DateTime, ParameterConstructorWithTimeZone_pos1) {
    Coords::DateTime a_datetime(2014, 12, 8, 13, 30, 00, "5");
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ("2014-12-08T13:30:00.0+0500", out.str().c_str());
  }

  TEST(DateTime, ParameterConstructorWithTimeZone_pos_int) {
    Coords::DateTime a_datetime(2014, 12, 8, 13, 30, 00, 5);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ("2014-12-08T13:30:00.0+0500", out.str().c_str());
  }

  TEST(DateTime, ParameterConstructorWithTimeZone_pos_double) {
    Coords::DateTime a_datetime(2014, 12, 8, 13, 30, 00, 5.5);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ("2014-12-08T13:30:00.0+0530", out.str().c_str());
  }

  TEST(DateTime, ParameterConstructorWithTimeZone_pos2) {
    Coords::DateTime a_datetime(2014, 12, 8, 13, 30, 00, "+05:30");
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ("2014-12-08T13:30:00.0+05:30", out.str().c_str());
  }

  TEST(DateTime, ParameterConstructorWithTimeZone_neg1) {
    Coords::DateTime a_datetime(2014, 12, 8, 13, 30, 00, "-0506");
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ("2014-12-08T13:30:00.0-0506", out.str().c_str());
  }


  TEST(DateTime, ParameterConstructorWithTimeZone_1200) {
    Coords::DateTime a_datetime(2019, 9, 13, 14, 30, 45.123, "+12:00");
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ("2019-09-13T14:30:45.1+12:00", out.str().c_str());

  }

  TEST(DateTime, ParameterConstructorWithTimeZone_n1200) {
    Coords::DateTime a_datetime(2019, 9, 14, 9, 15, 10.333, "-1200");
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ("2019-09-14T09:15:10.3-1200", out.str().c_str());

  }


  // data out of range


  TEST(DateTime, ParameterConstructorWithBadMonth_1) {
    try {
      Coords::DateTime a_datetime(2014, -2, 8, 13, 30, 00, "-0506");
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "2014--2-08T13:30:00.0-0506: month out of range.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, ParameterConstructorWithBadMonth_2) {
    try {
      Coords::DateTime a_datetime(2014, 13, 8, 13, 30, 00, "-05:15");
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "2014-13-08T13:30:00.0-05:15: month out of range.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, ParameterConstructorWithBadDay_1) {
    try {
      Coords::DateTime a_datetime(2014, 2, -4, 13, 30, 00, "-05:00");
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "2014-02--4T13:30:00.0-05:00: day out of range.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, ParameterConstructorWithBadDay_2) {
    try {
      Coords::DateTime a_datetime(2014, 12, 33, 13, 30, 00, "-05:06");
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "2014-12-33T13:30:00.0-05:06: day out of range.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, ParameterConstructorWithBadHour_1) {
    try {
      Coords::DateTime a_datetime(2014, 2, 4, -1, 30, 00, "-0506");
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "2014-02-04T-1:30:00.0-0506: hour out of range.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, ParameterConstructorWithBadHour_2) {
    try {
      Coords::DateTime a_datetime(2014, 12, 03, 60, 30, 00, "-05:06");
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "2014-12-03T60:30:00.0-05:06: hour out of range.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }


  TEST(DateTime, ParameterConstructorWithBadMinute_1) {
    try {
      Coords::DateTime a_datetime(2014, 2, 4, 13, -3, 00, "-0515");
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "2014-02-04T13:-3:00.0-0515: minute out of range.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, ParameterConstructorWithBadMinute_2) {
    try {
      Coords::DateTime a_datetime(2014, 12, 03, 13, 61, 00, "-05:45");
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "2014-12-03T13:61:00.0-05:45: minute out of range.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, ParameterConstructorWithBadSecond_1) {
    try {
      Coords::DateTime a_datetime(2014, 2, 4, 13, 30, -1, "-0506");
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "2014-02-04T13:30:-1.0-0506: second out of range.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, ParameterConstructorWithBadSecond_2) {
    try {
      Coords::DateTime a_datetime(2014, 12, 03, 13, 30, 60, "-05:00");
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "2014-12-03T13:31:00.0-05:00: second out of range.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }


  TEST(DateTime, ParameterConstructorWithBadTimeZone_1250) {
    try {
      Coords::DateTime a_datetime(2014, 2, 4, 13, 30, 1.5, "12:01");
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "12.0167: time zone out of range.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, ParameterConstructorWithBadTimeZone_n15) {
    try {
      Coords::DateTime a_datetime(2014, 2, 4, 13, 30, 1.5, "-15:50");
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "-15:50 unsupported timezone format: [z|Z|[+|-]hh[[:]mm]] for -12 < hh < 12";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, ParameterConstructorWithBadTimeZone_2) {
    try {
      Coords::DateTime a_datetime(2014, 12, 03, 13, 30, 59, "15:06");
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << "15:06 unsupported timezone format: [z|Z|[+|-]hh[[:]mm]] for -12 < hh < 12";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, CopyConstructor_1) {
    Coords::DateTime a_datetime(2014, 12, 8, 13, 30, 00, "04:15");
    Coords::DateTime another_datetime(a_datetime);

    std::stringstream out;
    out << a_datetime;

    std::stringstream another_out;
    another_out << another_datetime;

    EXPECT_STREQ(out.str().c_str(), another_out.str().c_str());
  }

  TEST(DateTime, CopyAssign_1) {
    Coords::DateTime a_datetime(2014, 12, 8, 13, 30, 00, "04:30");
    Coords::DateTime another_datetime;

    another_datetime = a_datetime;

    std::stringstream out;
    out << a_datetime;

    std::stringstream another_out;
    another_out << another_datetime;

    EXPECT_STREQ(out.str().c_str(), another_out.str().c_str());
  }

  // months

  TEST(DateTime, GoodMonthConstructors) {
    for (int i(1); i < 13; ++i) {
      std::stringstream a_datetime_string;
      a_datetime_string << "2014-" << std::setw(2) << std::setfill('0') << i << "-01T12:34:56.0";
      Coords::DateTime a_datetime(a_datetime_string.str());
      std::stringstream out;
      out << a_datetime;
      EXPECT_STREQ(a_datetime_string.str().c_str(), out.str().c_str());
      EXPECT_DOUBLE_EQ(0, a_datetime.offset());
    }
  }

  TEST(DateTime, BadMonthConstructor_0) {
    std::string a_datetime_string("2014-00-07T12:34:56");
    try {
      Coords::DateTime a_datetime(a_datetime_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_datetime_string << " not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][z|Z|[+|-]hh[[:]mm]]";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, BadMonthConstructor_13) {
    std::string a_datetime_string("2014-13-07T12:34:56");
    try {
      Coords::DateTime a_datetime(a_datetime_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_datetime_string << " not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][z|Z|[+|-]hh[[:]mm]]";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }


  // days

  TEST(DateTime, GoodDayConstructors_jan) {
    for (int i(1); i < 32; ++i) {
      std::stringstream a_datetime_string;
      a_datetime_string << "2014-01-" << std::setw(2) << std::setfill('0') << i << "T12:34:56.0";
      Coords::DateTime a_datetime(a_datetime_string.str());
      std::stringstream out;
      out << a_datetime;
      EXPECT_STREQ(a_datetime_string.str().c_str(), out.str().c_str());
      EXPECT_DOUBLE_EQ(0, a_datetime.offset());
    }
  }

  TEST(DateTime, GoodDayConstructors_sep) {
    // Thirty days hath September
    for (int i(1); i < 31; ++i) {
      std::stringstream a_datetime_string;
      a_datetime_string << "2014-09-" << std::setw(2) << std::setfill('0') << i << "T12:34:56.0";
      Coords::DateTime a_datetime(a_datetime_string.str());
      std::stringstream out;
      out << a_datetime;
      EXPECT_STREQ(a_datetime_string.str().c_str(), out.str().c_str());
      EXPECT_DOUBLE_EQ(0, a_datetime.offset());
    }
  }


  TEST(DateTime, BadDayConstructor_0) {
    std::string a_datetime_string("2014-12-00T12:34:56");
    try {
      Coords::DateTime a_datetime(a_datetime_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_datetime_string << " not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][z|Z|[+|-]hh[[:]mm]]";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }


  TEST(DateTime, BadDayConstructor_32) {
    std::string a_datetime_string("2014-12-32T12:34:56");
    try {
      Coords::DateTime a_datetime(a_datetime_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_datetime_string << " not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][z|Z|[+|-]hh[[:]mm]]";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }


  TEST(DateTime, ExtraDayConstructor_leap_day_2012) {
    // leap day
    std::stringstream a_datetime_string;
    a_datetime_string << "2012-02-29T12:34:56.3";

    Coords::DateTime a_leap_day("2012-02-29T12:34:56.3");

    std::stringstream out;
    out << a_leap_day;
    EXPECT_STREQ(a_datetime_string.str().c_str(), out.str().c_str());

  }


  TEST(DateTime, TooExtraDayConstructor_leap_day_2012) {
    // leap day +1
    std::string bad_date("2012-02-30T12:34:56");
    try {
      Coords::DateTime a_datetime(bad_date);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << bad_date << ": Except for February all alone. It has 28, but 29 each _leap_ year.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }


  TEST(DateTime, NoExtraConstructor_noleap_day_2014) {
    // not a leap year
    std::stringstream a_datetime_string;
    a_datetime_string << "2014-02-28T01:34:00.0";

    Coords::DateTime a_good_date("2014-02-28T01:34:00");

    std::stringstream out;
    out << a_good_date;
    EXPECT_STREQ(a_datetime_string.str().c_str(), out.str().c_str());

  }


  TEST(DateTime, TooExtraDayConstructor_noleap_day_2014) {
    // not a leap year
    std::string bad_date("2014-02-29T13:34:15");
    try {
      Coords::DateTime a_datetime(bad_date);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << bad_date << ": Except for February all alone. It has _28_, but 29 each leap year.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }


  TEST(DateTime, pad_seconds) {

    std::stringstream a_datetime_string;
    a_datetime_string << "2000-02-29T14:00:01.2";

    Coords::DateTime a_leap_day("2000-02-29T14:00:01.19");

    std::stringstream out;
    out << a_leap_day;
    EXPECT_STREQ(a_datetime_string.str().c_str(), out.str().c_str());

  }

  TEST(DateTime, ExtraDayConstructor_leap_day_2000) {
    // leap year mod 400 rule
    std::stringstream a_datetime_string;
    a_datetime_string << "2000-02-29T14:00:01.1";

    Coords::DateTime a_leap_day("2000-02-29T14:00:01.123");

    std::stringstream out;
    out << a_leap_day;
    EXPECT_STREQ(a_datetime_string.str().c_str(), out.str().c_str());

  }

  TEST(DateTime, TooExtraDayConstructor_leap_day_2000) {
    // leap year mod 400 rule
    std::string bad_date("2000-02-30T15:34:56");
    try {
      Coords::DateTime a_datetime(bad_date);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << bad_date << ": Except for February all alone. It has 28, but 29 each _leap_ year.";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }


  TEST(DateTime, BadDayConstructor_leap_year_2100) {
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
    std::string a_datetime_string("2014-04-31T12:34:56");
    try {
      Coords::DateTime a_datetime(a_datetime_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_datetime_string << ": Thirty days hath September, April, June and November";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, BadDayConstructor_jun31) {
    std::string a_datetime_string("2014-06-31T12:34:56");
    try {
      Coords::DateTime a_datetime(a_datetime_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_datetime_string << ": Thirty days hath September, April, June and November";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, BadDayConstructor_sep31) {
    std::string a_datetime_string("2014-09-31T12:34:56");
    try {
      Coords::DateTime a_datetime(a_datetime_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_datetime_string << ": Thirty days hath September, April, June and November";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, BadDayConstructor_nov31) {
    std::string a_datetime_string("2014-11-31T12:34:56");
    try {
      Coords::DateTime a_datetime(a_datetime_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_datetime_string << ": Thirty days hath September, April, June and November";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }


  // hours

  TEST(DateTime, GoodHourConstructors) {
    for (int i(0); i < 24; ++i) {
      std::stringstream a_datetime_string;
      a_datetime_string << "2014-01-01T" << std::setw(2) << std::setfill('0') << i << ":34:56.0";
      Coords::DateTime a_datetime(a_datetime_string.str());
      std::stringstream out;
      out << a_datetime;
      EXPECT_STREQ(a_datetime_string.str().c_str(), out.str().c_str());
      EXPECT_DOUBLE_EQ(0, a_datetime.offset());
    }
  }


  TEST(DateTime, BadHourConstructor_1) {
    std::string a_datetime_string("2014-12-31T24:34:56");
    try {
      Coords::DateTime a_datetime(a_datetime_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_datetime_string << " not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][z|Z|[+|-]hh[[:]mm]]";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  // minutes

  TEST(DateTime, GoodMinuteConstructors) {
    for (int i(0); i < 60; ++i) {
      std::stringstream a_datetime_string;
      a_datetime_string << "2014-01-01T00:" << std::setw(2) << std::setfill('0') << i << ":56.0";
      Coords::DateTime a_datetime(a_datetime_string.str());
      std::stringstream out;
      out << a_datetime;
      EXPECT_STREQ(a_datetime_string.str().c_str(), out.str().c_str());
      EXPECT_DOUBLE_EQ(0, a_datetime.offset());
    }
  }


  TEST(DateTime, BadMinuteConstructor_1) {
    std::string a_datetime_string("2014-12-31T10:62:56");
    try {
      Coords::DateTime a_datetime(a_datetime_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_datetime_string << " not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][z|Z|[+|-]hh[[:]mm]]";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }


  TEST(DateTime, BadMinuteConstructor_no_seconds) {
    std::string a_datetime_string("2014-12-31T10:62");
    try {
      Coords::DateTime a_datetime(a_datetime_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_datetime_string << " not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][z|Z|[+|-]hh[[:]mm]]";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }


  // seconds

  TEST(DateTime, GoodSecondConstructors) {
    for (int i(0); i < 60; ++i) {
      std::stringstream a_datetime_string;
      a_datetime_string << "2014-01-01T00:00:" << std::setw(2) << std::setfill('0') << i << ".0";
      Coords::DateTime a_datetime(a_datetime_string.str());
      std::stringstream out;
      out << a_datetime;
      EXPECT_STREQ(a_datetime_string.str().c_str(), out.str().c_str());
      EXPECT_DOUBLE_EQ(0, a_datetime.offset());
    }
  }

  TEST(DateTime, BadSecondConstructor_1) {
    std::string a_datetime_string("2014-12-31T10:12:66");
    try {
      Coords::DateTime a_datetime(a_datetime_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_datetime_string << " not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][z|Z|[+|-]hh[[:]mm]]";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }


  // time zones

  TEST(DateTime, GoodTimeZoneConstructor_Zulu) {
    std::string a_datetime_string("2014-12-07T12:34:56.7Z");
    Coords::DateTime a_datetime(a_datetime_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }


  TEST(DateTime, GoodTimeZoneConstructors_pos_hrs) {
    for (int i(1); i < 13; ++i) {
      std::stringstream a_datetime_string;
      a_datetime_string << "2014-03-15T12:34:56.1+" << std::setw(2) << std::setfill('0') << i << ":00";
      Coords::DateTime a_datetime(a_datetime_string.str());
      std::stringstream out;
      out << a_datetime;
      EXPECT_STREQ(a_datetime_string.str().c_str(), out.str().c_str());
      EXPECT_DOUBLE_EQ(i, a_datetime.offset());
    }
  }

  TEST(DateTime, GoodTimeZoneConstructors_neg_hrs) {
    for (int i(1); i < 13; ++i) {
      std::stringstream a_datetime_string;
      a_datetime_string << "2014-03-15T12:34:56.2-" << std::setw(2) << std::setfill('0') << i << "00";
      Coords::DateTime a_datetime(a_datetime_string.str());
      std::stringstream out;
      out << a_datetime;
      EXPECT_STREQ(a_datetime_string.str().c_str(), out.str().c_str());
      EXPECT_DOUBLE_EQ(-i, a_datetime.offset());
    }
  }

  TEST(DateTime, GoodTimeZoneConstructors_mins) {
    for (int i(0); i < 60; ++i) {
      std::stringstream a_datetime_string;
      a_datetime_string << "2014-03-15T12:34:56.3+08:" << std::setw(2) << std::setfill('0') << i;
      Coords::DateTime a_datetime(a_datetime_string.str());
      std::stringstream out;
      out << a_datetime;
      EXPECT_STREQ(a_datetime_string.str().c_str(), out.str().c_str());
    }
  }


  TEST(DateTime, BadTimeZoneConstructor_1) {
    std::string a_datetime_string("2014-12-07T12:34:56.78+13.987");
    try {
      Coords::DateTime a_datetime(a_datetime_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_datetime_string << " not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][z|Z|[+|-]hh[[:]mm]]";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }

  TEST(DateTime, BadTimeZoneConstructor_2) {
    std::string a_datetime_string("2014-12-07T12:34:56.78-13.987");
    try {
      Coords::DateTime a_datetime(a_datetime_string);
    } catch (Coords::Error& err) {
      std::stringstream emsg;
      emsg << a_datetime_string << " not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][z|Z|[+|-]hh[[:]mm]]";
      EXPECT_STREQ(err.what(), emsg.str().c_str());
    }
  }


  TEST(DateTime, GoodTimeZoneConstructor_1) {
    std::string a_datetime_string("2014-12-07T12:34:56.7+0400");
    Coords::DateTime a_datetime(a_datetime_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }


  TEST(DateTime, GoodTimeZoneConstructor_2) {
    std::string a_datetime_string("2014-12-07T12:34:56.6+0430");
    Coords::DateTime a_datetime(a_datetime_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
    EXPECT_DOUBLE_EQ(4.5, a_datetime.offset());
  }

  TEST(DateTime, GoodTimeZoneConstructor_3) {
    std::string a_datetime_string("2014-12-07T12:34:56.5+04:15");
    Coords::DateTime a_datetime(a_datetime_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
    EXPECT_DOUBLE_EQ(4.25, a_datetime.offset());
  }

  TEST(DateTime, GoodTimeZoneConstructor_4) {
    std::string a_datetime_string("2014-12-07T12:34:56.4-0400");
    Coords::DateTime a_datetime(a_datetime_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }


  TEST(DateTime, GoodTimeZoneConstructor_5) {
    std::string a_datetime_string("2014-12-07T12:34:56.3-0430");
    Coords::DateTime a_datetime(a_datetime_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
    EXPECT_DOUBLE_EQ(-4.5, a_datetime.offset());
  }

  TEST(DateTime, GoodTimeZoneConstructor_6) {
    std::string a_datetime_string("2014-12-07T12:34:56.2-04:45");
    Coords::DateTime a_datetime(a_datetime_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
    EXPECT_DOUBLE_EQ(-4.75, a_datetime.offset());
  }

  TEST(DateTime, LetsGetBiblical_1) {
    std::string a_datetime_string("-5579-03-20T12:00:00.0");
    Coords::DateTime a_datetime(a_datetime_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }


  // ----------------------------------
  // ----- time element accessors -----
  // ----------------------------------

  TEST(DateTime, time_element_accessor_1) {
    std::string a_datetime_string("2016-04-02T12:30:05.4-08:00");

    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_EQ(2016, a_datetime.year());
    EXPECT_EQ(04, a_datetime.month());
    EXPECT_EQ(02, a_datetime.day());
    EXPECT_EQ(12, a_datetime.hour());
    EXPECT_EQ(30, a_datetime.minute());
    EXPECT_DOUBLE_EQ(5.4, a_datetime.second());
    EXPECT_DOUBLE_EQ(-8, a_datetime.offset());

  }


  TEST(DateTime, time_element_accessor_default) {
    std::string a_datetime_string("1916-02-29T02:12:34");

    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_EQ(1916, a_datetime.year());
    EXPECT_EQ(02, a_datetime.month());
    EXPECT_EQ(29, a_datetime.day());
    EXPECT_EQ(02, a_datetime.hour());
    EXPECT_EQ(12, a_datetime.minute());
    EXPECT_DOUBLE_EQ(34, a_datetime.second());
    EXPECT_DOUBLE_EQ(0, a_datetime.offset());

  }


  TEST(DateTime, time_element_accessor_z) {
    std::string a_datetime_string("1916-02-29T02:12:00Z");

    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_EQ(1916, a_datetime.year());
    EXPECT_EQ(02, a_datetime.month());
    EXPECT_EQ(29, a_datetime.day());
    EXPECT_EQ(02, a_datetime.hour());
    EXPECT_EQ(12, a_datetime.minute());
    EXPECT_DOUBLE_EQ(0, a_datetime.second());
    EXPECT_DOUBLE_EQ(0, a_datetime.offset());

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
    std::string a_datetime_string("-4712-01-01T12:00:00");

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(0.5, a_datetime.toJulianDateWiki()); // rounds to day

    Coords::DateTime another_datetime(a_datetime.fromJulianDateWiki(a_datetime.toJulianDateWiki()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("-4713-11-25T12:00:00.0", out.str().c_str()); // TODO Gregorian day 0 + year
  }

  TEST(DISABLED_DateTime, JulianDateWiki_Julian_0) {

    // TODO this is off by a day in calculation and switchs to Gregorian on reversal

    std::string a_datetime_string("-4713-01-01T12:00:00"); // Julian day 0

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(0.5, a_datetime.toJulianDateWiki()); // TODO off by a year -364.5

    Coords::DateTime another_datetime(a_datetime.fromJulianDateWiki(a_datetime.toJulianDateWiki()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("-4714-11-25T12:00:00.0", out.str().c_str()); // Gregorian day 0
  }

  TEST(DISABLED_DateTime, JulianDateWiki_Gregorian_0) {

    // TODO this is off by a day

    std::string a_datetime_string("-4714-11-24T12:00:00"); // Gregorian day 0

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(0.5, a_datetime.toJulianDateWiki()); // ok

    Coords::DateTime another_datetime(a_datetime.fromJulianDateWiki(a_datetime.toJulianDateWiki()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("-4714-11-25T12:00:00.0", out.str().c_str()); // Gregorian day 0 - 1
  }


  TEST(DateTime, JulianDateWiki_pre_Julian2Gregorian) {
    std::string a_datetime_string("1582-10-14T00:00:00"); // Gregorian replaces Julian

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_NEAR(a_datetime.s_LilianDate + 10, a_datetime.toJulianDateWiki(), 0.5);
    // Does not account for 10 day "gap" in calendar change.

    Coords::DateTime another_datetime(a_datetime.fromJulianDateWiki(a_datetime.toJulianDateWiki()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("1582-10-24T00:00:00.0", out.str().c_str());
  }

  TEST(DateTime, JulianDateWiki_post_Julian2Gregorian) {
    std::string a_datetime_string("1582-10-15T00:00:00.0"); // Gregorian replaces Julian

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_NEAR(a_datetime.s_LilianDate, a_datetime.toJulianDateWiki(), 0.5);
    // toJulianDateWiki() rounds to nearest day

    Coords::DateTime another_datetime(a_datetime.fromJulianDateWiki(a_datetime.toJulianDateWiki()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }

  TEST(DISABLED_DateTime, JulianDateWiki_pre_modified) {

    // TODO off by a day

    std::string a_datetime_string("1858-11-16T12:00:00.0"); // modified date change switches from noon to midnight

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_NEAR(Coords::DateTime::s_ModifiedJulianDate, a_datetime.toJulianDateWiki(), 0.5);
   // toJulianDateWiki() rounds to nearest day

    Coords::DateTime another_datetime(a_datetime.fromJulianDateWiki(a_datetime.toJulianDateWiki()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str()); // rounds to midnight
  }

  TEST(DateTime, JulianDateWiki_post_modified) {
    std::string a_datetime_string("1858-11-17T00:00:00.0"); // modified date change switches from noon to midnight

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_NEAR(Coords::DateTime::s_ModifiedJulianDate, a_datetime.toJulianDateWiki(), 0.5);
   // toJulianDateNRC() rounds to nearest day

    Coords::DateTime another_datetime(a_datetime.fromJulianDateNRC(a_datetime.toJulianDateWiki()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }


  TEST(DateTime, JulianDateWiki_truncated) {
    std::string a_datetime_string("1968-05-24T00:00:00.0"); // truncated Julian date

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_NEAR(Coords::DateTime::s_TruncatedJulianDate, a_datetime.toJulianDateWiki(), 0.5);
   // toJulianDateWiki() rounds to nearest day

    Coords::DateTime another_datetime(a_datetime.fromJulianDateWiki(a_datetime.toJulianDateWiki()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str()); // rounds to midnight
  }

  TEST(DISABLED_DateTime, JulianDateWiki_J2000) {

    // TODO off by half a day.

    std::string a_datetime_string("2000-01-01T00:00:00.0"); // J2000

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(2451544.5, a_datetime.toJulianDateWiki());

    Coords::DateTime another_datetime(a_datetime.fromJulianDateWiki(a_datetime.toJulianDateWiki()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }


  TEST(DateTime, JulianDateWiki_3) {
    std::string a_datetime_string("2013-01-01T00:30:00.0");

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_NEAR(2456293.5208333335, a_datetime.toJulianDateWiki(), 0.500001);
    // toJulianDateWiki() rounds to nearest day. Does not start at noon.

    Coords::DateTime another_datetime(a_datetime.fromJulianDateWiki(a_datetime.toJulianDateWiki()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, JulianDateWiki_4) {
    std::string a_datetime_string("2014-12-09T00:00:00.0");

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_NEAR(2457000.5, a_datetime.toJulianDateWiki(), 0.5);
    // rounds to nearest day

    Coords::DateTime another_datetime(a_datetime.fromJulianDateWiki(a_datetime.toJulianDateWiki()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }

  // ----------------------------
  // ----- Julian dates NRC -----
  // ----------------------------

  TEST(DateTime, JulianDateNRC_0) {
    std::string a_datetime_string("-4712-01-01T12:00:00.0");

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(366.5, a_datetime.toJulianDateNRC()); // TODO off by a year

    // TODO Does not account for year 0?

    Coords::DateTime another_datetime(a_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("-4712-01-01T00:00:00.0", out.str().c_str());
    // midnight not noon
  }


  TEST(DISABLED_DateTime, JulianDateNRC_Julian_0) {

    // TODO this is reading Julian in and Gregorian out

    std::string a_datetime_string("-4713-01-01T12:00:00.0"); // Julian day 0

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(0.5, a_datetime.toJulianDateNRC()); // ok

    Coords::DateTime another_datetime(a_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("-4714-11-25T12:00:00.0", out.str().c_str()); // Gregorian day 0
  }

  TEST(DISABLED_DateTime, JulianDateNRC_Gregorian_0) {

    // TODO this is reading Gregorian in and Gregorian out, but is several days off

    std::string a_datetime_string("-4714-11-24T12:00:00.0"); // Gregorian day 0

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(0.5, a_datetime.toJulianDateNRC()); // TODO Actual -37.5

    Coords::DateTime another_datetime(a_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str()); // ok
  }


  TEST(DateTime, JulianDateNRC_pre_Julian2Gregorian) {
    std::string a_datetime_string("1582-10-14T00:00:00.0"); // Gregorian replaces Julian

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_NEAR(a_datetime.s_LilianDate + 10, a_datetime.toJulianDateNRC(), 0.5);
    // does not account for 10 day "gap" in calendar change.

    Coords::DateTime another_datetime(a_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("1582-10-24T00:00:00.0", out.str().c_str());
  }

  TEST(DateTime, JulianDateNRC_post_Julian2Gregorian) {
    std::string a_datetime_string("1582-10-15T00:00:00.0"); // Gregorian replaces Julian

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_NEAR(a_datetime.s_LilianDate, a_datetime.toJulianDateNRC(), 0.5);
    // toJulianDateNRC() rounds to nearest day

    Coords::DateTime another_datetime(a_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, JulianDateNRC_pre_modified) {
    std::string a_datetime_string("1858-11-16T12:00:00"); // modified date change switches from noon to midnight

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_NEAR(Coords::DateTime::s_ModifiedJulianDate, a_datetime.toJulianDateWiki(), 0.5);
   // toJulianDateNRC() rounds to nearest day

    Coords::DateTime another_datetime(a_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("1858-11-16T00:00:00.0", out.str().c_str());
    // switches noon to midnight
  }


  TEST(DateTime, JulianDateNRC_post_modified) {
    std::string a_datetime_string("1858-11-17T00:00:00.0"); // modified date change switches from noon to midnight

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_NEAR(Coords::DateTime::s_ModifiedJulianDate, a_datetime.toJulianDateNRC(), 0.5);
   // toJulianDateNRC() rounds to nearest day

    Coords::DateTime another_datetime(a_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, JulianDateNRC_truncated) {
    std::string a_datetime_string("1968-05-24T00:00:00.0"); // truncated Julian date

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_NEAR(Coords::DateTime::s_TruncatedJulianDate, a_datetime.toJulianDateNRC(), 0.5);
   // toJulianDateNRC() rounds to nearest day

    Coords::DateTime another_datetime(a_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }

  TEST(DISABLED_DateTime, JulianDateNRC_J2000) {

    // TODO off by half a day.

    std::string a_datetime_string("2000-01-01T00:00:00.0"); // J2000

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(2451544.5, a_datetime.toJulianDateNRC());

    Coords::DateTime another_datetime(a_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }


  TEST(DateTime, JulianDateNRC_3) {
    std::string a_datetime_string("2013-01-01T00:30:00");

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_NEAR(2456293.5208333335, a_datetime.toJulianDateNRC(), 0.500001);
    // toJulianDateNRC() rounds to nearest day. Does not start at noon.

    Coords::DateTime another_datetime(a_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("2013-01-01T00:00:00.0", out.str().c_str()); // rounds to midnight
  }

  TEST(DateTime, JulianDateNRC_4) {
    std::string a_datetime_string("2014-12-09T00:00:00.0");

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_NEAR(2457000.5, a_datetime.toJulianDateNRC(), 0.5);
    // toJulianDateNRC() rounds to nearest day

    Coords::DateTime another_datetime(a_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }

  // -------------------------------------
  // ----- Modified Julian Dates APC -----
  // -------------------------------------

  TEST(DateTime, ModifiedJulianDateAPC_0) {
    std::string a_datetime_string("-4712-01-01T12:00:00.0");

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(-Coords::DateTime::s_ModifiedJulianDate, a_datetime.toModifiedJulianDateAPC());
    // TODO asAPCJulianDate() differs from IMCCE on noon vs. midnight on this date?

    Coords::DateTime another_datetime(a_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }

  TEST(DISABLED_DateTime, ModifiedJulianDateAPC_Julian_0) {

    // TODO calculation adds year zero, output rounds by a day

    std::string a_datetime_string("-4713-01-01T12:00:00.0"); // Julian day 0

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(-Coords::DateTime::s_ModifiedJulianDate - 365, a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime(a_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str()); // rounds by a day
  }

  TEST(DISABLED_DateTime, ModifiedJulianDateAPC_Gregorian_0) {

    // TODO calculation is off by 403, output by a day

    std::string a_datetime_string("-4714-11-24T12:00:00.0"); // Gregorian day 0

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(-Coords::DateTime::s_ModifiedJulianDate, a_datetime.toModifiedJulianDateAPC()); // Actual - 403

    Coords::DateTime another_datetime(a_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }


  TEST(DateTime, ModifiedJulianDateAPC_pre_Julian2Gregorian) {
    std::string a_datetime_string("1582-10-14T00:00:00"); // Gregorian replaces Julian

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(a_datetime.s_LilianDate - Coords::DateTime::s_ModifiedJulianDate - 1, // rounding
		     a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime(a_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("1582-10-04T00:00:00.0", out.str().c_str()); // skips 10 days
  }

  TEST(DateTime, ModifiedJulianDateAPC_post_Julian2Gregorian) {
    std::string a_datetime_string("1582-10-15T00:00:00.0"); // Gregorian replaces Julian

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(a_datetime.s_LilianDate - Coords::DateTime::s_ModifiedJulianDate,
		     a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime(a_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }


  TEST(DateTime, ModifiedJulianDateAPC_pre_modified) {
    std::string a_datetime_string("1858-11-16T12:00:00.0"); // modified date change switches from noon to midnight

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(-0.5, a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime(a_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, ModifiedJulianDateAPC_post_modified) {
    std::string a_datetime_string("1858-11-17T00:00:00.0"); // modified date change switches from noon to midnight

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(0, a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime(a_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, ModifiedJulianDateAPC_truncated) {
    std::string a_datetime_string("1968-05-24T00:00:00.0"); // truncated Julian date

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(Coords::DateTime::s_TruncatedJulianDate - Coords::DateTime::s_ModifiedJulianDate,
		     a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime(a_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, ModifiedJulianDateAPC_J2000) {

    std::string a_datetime_string("2000-01-01T00:00:00.0"); // J2000

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(2451544.5 - Coords::DateTime::s_ModifiedJulianDate,
		     a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime(a_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, ModifiedJulianDateAPC_J2000_13) {

    // tests 60 seconds rounds up a minute

    std::string a_datetime_string("2000-01-01T13:00:00.0"); // J2000

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(51544.541666666664, a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime(a_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, ModifiedJulianDateAPC_3) {
    std::string a_datetime_string("2013-01-01T00:30:00.0");

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_NEAR(2456293.5208333335 - Coords::DateTime::s_ModifiedJulianDate,
		a_datetime.toModifiedJulianDateAPC(), 0.000000001); // very near

    Coords::DateTime another_datetime(a_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, ModifiedJulianDateAPC_4) {
    std::string a_datetime_string("2014-12-09T00:00:00.0");

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(2457000.5 - Coords::DateTime::s_ModifiedJulianDate, a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime(a_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }


  TEST(DateTime, ModifiedJulianDateAPC_timezone_04) {

    std::string a_datetime_string("2015-05-04T06:00:00-04");
    Coords::DateTime a_datetime(a_datetime_string);

    std::string another_date_string("2015-05-04T06:30:00-04");
    Coords::DateTime another_datetime(another_date_string);

    EXPECT_NEAR(30, (another_datetime.toJulianDate() - a_datetime.toJulianDate())*24*60, 1e-6);

  }


  TEST(DateTime, ModifiedJulianDateAPC_timezone_n08) {

    std::string a_datetime_string("2015-05-04T06:00:00-08");
    Coords::DateTime a_datetime(a_datetime_string);

    std::string another_date_string("2015-05-04T06:30:00-08");
    Coords::DateTime another_datetime(another_date_string);

    EXPECT_NEAR(30, (another_datetime.toJulianDate() - a_datetime.toJulianDate())*24*60, 1e-6);

  }


  TEST(DateTime, ModifiedJulianDateAPC_timezone_08) {

    std::string a_datetime_string("2015-05-04T16:00:00+08");
    Coords::DateTime a_datetime(a_datetime_string);

    std::string another_date_string("2015-05-04T16:30:00+08");
    Coords::DateTime another_datetime(another_date_string);

    EXPECT_NEAR(30, (another_datetime.toJulianDate() - a_datetime.toJulianDate())*24*60, 1e-6);

  }

  // ---------------------------------
  // ----- Julian Date operators -----
  // ---------------------------------

  TEST(DateTime, JulianDate_0) {
    std::string a_datetime_string("-4712-01-01T12:00:00.0");

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(0.0, a_datetime.toJulianDate());

    Coords::DateTime another_datetime(a_datetime.fromJulianDate(a_datetime.toJulianDate()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }


  TEST(DateTime, JulianDate_2014_12_09) {
    std::string a_datetime_string("2014-12-09T00:00:00.0");

    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(2457000.5, a_datetime.toJulianDate());

    Coords::DateTime another_datetime(a_datetime.fromJulianDate(a_datetime.toJulianDate()));

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());
  }


  // ------------------------------------
  // ----- timezone date arithmetic -----
  // ------------------------------------


  TEST(DateTime, operator_plus_eq_1) {
    std::string a_datetime_string("2014-12-09T00:00:00");

    Coords::DateTime a_datetime(a_datetime_string);

    a_datetime += 1;

    EXPECT_DOUBLE_EQ(2457000.5 + 1, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2014-12-10T00:00:00.0", out.str().c_str());
  }



  TEST(DateTime, operator_plus_eq_hour_1) {
    std::string a_datetime_string("2014-12-09T00:00:00");

    Coords::DateTime a_datetime(a_datetime_string);

    a_datetime += 1.0/24.0;

    EXPECT_DOUBLE_EQ(2457000.5 + 1.0/24.0, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2014-12-09T01:00:00.0", out.str().c_str());
  }


  // edge of year transition
  TEST(DateTime, operator_plus_eq_hour_2) {
    std::string a_datetime_string("2014-12-31T23:10:02");

    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_DOUBLE_EQ(2457023.465300926, a_datetime.toJulianDate());

    a_datetime += 1.0/24.0;

    EXPECT_DOUBLE_EQ(2457023.465300926 + 1.0/24.0, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2015-01-01T00:10:02.0", out.str().c_str());
  }


  // on edge of day transition
  TEST(DateTime, operator_plus_eq_00_tz1) {

    // interesting rounding error on this day, m_second =
    // 59.999986588954926 m_minute = 59, m_hour = 23, same as
    // http://aa.usno.navy.mil/cgi-bin/aa_jdconv.pl?form=2&jd=2457001.541666

    std::string a_datetime_string("2014-12-09T00:00:00+0100");

    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_DOUBLE_EQ(2457000.4583333335, a_datetime.toJulianDate());

    a_datetime += 1;

    EXPECT_DOUBLE_EQ(2457001.4583333335, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2014-12-10T00:00:00.0+0100", out.str().c_str());
  }

  TEST(DateTime, operator_plus_eq_01_tz2) {
    std::string a_datetime_string("2014-12-09T01:00:00+0200");

    Coords::DateTime a_datetime(a_datetime_string);

    a_datetime += 1;

    EXPECT_DOUBLE_EQ(2457001.4583333335, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2014-12-10T01:00:00.0+0200", out.str().c_str());
  }

  TEST(DateTime, operator_plus_eq_00_ntz1) {
    std::string a_datetime_string("2014-12-09T00:00:00-0100");

    Coords::DateTime a_datetime(a_datetime_string);

    a_datetime += 1;

    EXPECT_DOUBLE_EQ(2457001.5416666665, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2014-12-10T00:00:00.0-0100", out.str().c_str());
  }

  TEST(DateTime, operator_plus_eq_23_ntz1) {
    std::string a_datetime_string("2014-12-09T23:00:00-0100");

    Coords::DateTime a_datetime(a_datetime_string);

    a_datetime += 1;

    EXPECT_DOUBLE_EQ(2457002.5, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2014-12-10T23:00:00.0-0100", out.str().c_str());
  }

  TEST(DateTime, operator_plus_eq_30) {
    std::string a_datetime_string("2014-12-09T14:30:00");

    Coords::DateTime a_datetime(a_datetime_string);

    a_datetime += 30;

    EXPECT_DOUBLE_EQ(2457031.1041666665, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2015-01-08T14:30:00.0", out.str().c_str());
  }

  TEST(DateTime, operator_minus_eq_1) {
    std::string a_datetime_string("2014-12-09T00:00:00");

    Coords::DateTime a_datetime(a_datetime_string);

    a_datetime -= 1;

    EXPECT_DOUBLE_EQ(2457000.5 - 1, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2014-12-08T00:00:00.0", out.str().c_str());
  }


  TEST(DateTime, operator_minus_eq_hour_1) {

    std::string a_datetime_string("2016-05-08T00:00:00");

    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_DOUBLE_EQ(2457516.5, a_datetime.toJulianDate());

    a_datetime -= 1.0/24.0;

    EXPECT_DOUBLE_EQ(2457516.5 - 1.0/24.0, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2016-05-07T23:00:00.0", out.str().c_str());
  }


  // edge of year transition
  TEST(DateTime, operator_minus_eq_hour_2) {

    std::string a_datetime_string("2016-01-01T00:10:00");

    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_DOUBLE_EQ(2457388.5069444445, a_datetime.toJulianDate());

    a_datetime -= 1.0/24.0;

    EXPECT_DOUBLE_EQ(2457388.5069444445 - 1.0/24.0, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2015-12-31T23:10:00.0", out.str().c_str());
  }


  // on edge of day transition
  TEST(DateTime, operator_minus_eq_00_tz1) {

    std::string a_datetime_string("2014-12-09T00:00:00+0100");

    // another interesting day with rounding errors
    // http://aa.usno.navy.mil/cgi-bin/aa_jdconv.pl?form=2&jd=2456999.458333
    // CE 2014 December 07 22:59:60.0 UT  Sunday

    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_DOUBLE_EQ(2457000.4583333335, a_datetime.toJulianDate()); // 2014-12-08T23:00:00Z

    a_datetime -= 1;

    EXPECT_DOUBLE_EQ(2456999.4583333335, a_datetime.toJulianDate()); // 2014-12-07T23:00:00Z

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2014-12-08T00:00:00.0+0100", out.str().c_str());
  }


  // on edge of day transition
  TEST(DateTime, operator_minus_eq_00_ntz1) {
    std::string a_datetime_string("2014-12-09T00:00:00-0100");

    Coords::DateTime a_datetime(a_datetime_string);

    a_datetime -= 1;

    EXPECT_DOUBLE_EQ(2456999.5416666665, a_datetime.toJulianDate()); // 2014-12-08T01:00:00Z

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2014-12-08T00:00:00.0-0100", out.str().c_str());
  }


  TEST(DateTime, operator_minus_eq_30) {
    std::string a_datetime_string("2014-12-09T14:00:00");

    Coords::DateTime a_datetime(a_datetime_string);

    a_datetime -= 30;

    EXPECT_DOUBLE_EQ(2456971.0833333335, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2014-11-09T14:00:00.0", out.str().c_str());
  }

  TEST(DateTime, operator_date_plus_1) {
    std::string a_datetime_string("2014-12-09T00:00:00");

    Coords::DateTime a_datetime(a_datetime_string);
    Coords::DateTime another_datetime;

    another_datetime = a_datetime + 1;

    EXPECT_DOUBLE_EQ(2457000.5 + 1, another_datetime.toJulianDate());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("2014-12-10T00:00:00.0", out.str().c_str());
  }

  TEST(DateTime, operator_1_plus_date) {
    std::string a_datetime_string("2014-12-09T00:00:00");

    Coords::DateTime a_datetime(a_datetime_string);
    Coords::DateTime another_datetime;

    another_datetime = 1 + a_datetime;

    EXPECT_DOUBLE_EQ(2457000.5 + 1, another_datetime.toJulianDate());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("2014-12-10T00:00:00.0", out.str().c_str());
  }

  TEST(DateTime, operator_date_plus_30) {
    std::string a_datetime_string("2014-12-09T14:50:00");

    Coords::DateTime a_datetime(a_datetime_string);
    Coords::DateTime another_datetime;

    another_datetime = a_datetime + 30;

    EXPECT_DOUBLE_EQ(2457001.1180555555, a_datetime.toJulianDate());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("2015-01-08T14:50:00.0", out.str().c_str());
  }


  TEST(DateTime, operator_30_plus_date) {
    std::string a_datetime_string("2014-12-09T14:50:00");

    Coords::DateTime a_datetime(a_datetime_string);
    Coords::DateTime another_datetime;

    another_datetime = 30 + a_datetime;

    EXPECT_DOUBLE_EQ(2457001.1180555555, a_datetime.toJulianDate());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("2015-01-08T14:50:00.0", out.str().c_str());
  }


  TEST(DateTime, operator_date_minus_1) {
    std::string a_datetime_string("2014-12-09T00:00:00");

    Coords::DateTime a_datetime(a_datetime_string);
    Coords::DateTime another_datetime;

    another_datetime = a_datetime - 1;

    EXPECT_DOUBLE_EQ(2457000.5 - 1, another_datetime.toJulianDate());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("2014-12-08T00:00:00.0", out.str().c_str());
  }


  TEST(DateTime, operator_date_minus_30) {
    std::string a_datetime_string("2014-12-09T14:50:00");

    Coords::DateTime a_datetime(a_datetime_string);
    Coords::DateTime another_datetime;

    another_datetime = a_datetime - 30;

    EXPECT_DOUBLE_EQ(2457001.1180555555, a_datetime.toJulianDate());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("2014-11-09T14:50:00.0", out.str().c_str());
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


  TEST(DateTime, set_timezone_2017jan01_plusequals_one_hour) {

    std::string a_datetime_string("2017-01-01T03:00:00-08:00");
    Coords::DateTime a_datetime(a_datetime_string);

    Coords::DateTime another_datetime(a_datetime);


    another_datetime += 1.0/24;

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("2017-01-01T04:00:00.0-08:00", out.str().c_str());
  }


  TEST(DateTime, set_timezone_2016may09_add_offset) {

    // day change with timezone

    std::string a_datetime_string("2016-05-09T00:00:00-03:00");
    Coords::DateTime a_datetime(a_datetime_string);


    a_datetime += a_datetime.offset()/24; // to center plot at noon

    a_datetime += 1.0/24;
    a_datetime += 1.0/24;
    a_datetime += 1.0/24;
    a_datetime += 1.0/24;

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2016-05-09T01:00:00.0-03:00", out.str().c_str());
  }



  TEST(DateTime, set_timezone_2016may31_foreAmonth) {

    // month change with timezone

    std::string a_datetime_string("2016-05-31T22:00:00+05:30");
    Coords::DateTime a_datetime(a_datetime_string);

    a_datetime += 1.0/24;
    a_datetime += 1.0/24;
    a_datetime += 1.0/24;

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2016-06-01T01:00:00.0+05:30", out.str().c_str());
  }



  TEST(DateTime, zulu_2016june30_foreAmonth) {

    // month change with timezone

    std::string a_datetime_string("2016-06-30T22:00:00-11:00");
    Coords::DateTime a_datetime(a_datetime_string);

    a_datetime += 1.0/24;
    a_datetime += 1.0/24;
    a_datetime += 1.0/24;

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2016-07-01T01:00:00.0-11:00", out.str().c_str());
  }


  TEST(DateTime, set_timezone_2016may09_backAday) {

    // day change with timezone, +03:00

    std::string a_datetime_string("2016-05-09T02:00:00+03:00");
    Coords::DateTime a_datetime(a_datetime_string);

    a_datetime -= 1.0/24;
    a_datetime -= 1.0/24;
    a_datetime -= 1.0/24;
    a_datetime -= 1.0/24;

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2016-05-08T22:00:00.0+03:00", out.str().c_str());
  }


  TEST(DateTime, set_timezone_2015mar01_backAmonth) {

    // non leap year
    // month change with timezone

    std::string a_datetime_string("2015-03-01T02:00:00-03");
    Coords::DateTime a_datetime(a_datetime_string);


    a_datetime -= 1.0/24;
    a_datetime -= 1.0/24;
    a_datetime -= 1.0/24;

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2015-02-28T23:00:00.0-0300", out.str().c_str());
  }


  TEST(DateTime, set_timezone_2015aug08_backAmonth) {

    // non leap year
    // month change with timezone

    std::string a_datetime_string("2015-08-01T02:15:00-04");
    Coords::DateTime a_datetime(a_datetime_string);

    a_datetime -= 1.0/24;
    a_datetime -= 1.0/24;
    a_datetime -= 1.0/24;

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2015-07-31T23:15:00.0-0400", out.str().c_str());
  }



  TEST(DateTime, set_timezone_2016dec31_a) {

    // year change forward

    std::string a_datetime_string("2016-12-31T23:00:00+08:00");
    Coords::DateTime a_datetime(a_datetime_string);

    a_datetime += 1.0/24;

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2017-01-01T00:00:00.0+08:00", out.str().c_str());
  }


  TEST(DateTime, set_timezone_2016dec31_foreAyear) {

    // year change with timezone

    std::string a_datetime_string("2016-12-31T22:00:00-03");
    Coords::DateTime a_datetime(a_datetime_string);

    a_datetime += 1.0/24;
    a_datetime += 1.0/24;
    a_datetime += 1.0/24;
    a_datetime += 1.0/24;

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2017-01-01T02:00:00.0-0300", out.str().c_str());
  }


  TEST(DateTime, set_timezone_2017jan01_b) {

    // year change back

    std::string a_datetime_string("2017-01-01T03:00:00+08:00");
    Coords::DateTime a_datetime(a_datetime_string);

    a_datetime -= 1.0/24;
    a_datetime -= 1.0/24;
    a_datetime -= 1.0/24;

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2017-01-01T00:00:00.0+08:00", out.str().c_str());
  }


  // ----------------------------
  // ----- moving timezones -----
  // ----------------------------


  // no change
  TEST(DateTime, set_timezone_to_same) {

    std::string a_datetime_string("2015-05-21T23:00:00-05:00");
    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_DOUBLE_EQ(2457164.6666666665, a_datetime.toJulianDate());

    Coords::DateTime a_new_datetime(a_datetime.inTimeZone(Coords::TimeZone("-05:00")));

    std::stringstream out;
    out << a_new_datetime;

    EXPECT_STREQ("2015-05-21T23:00:00.0-05:00", out.str().c_str());

  }

  // ----- inTimezone shifts that change months -----


  TEST(DateTime, set_timezone_2015jun01_tzn4_a) {

    // timezone -4 as int month change

    std::string a_datetime_string("2015-06-01T02:00:00");
    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(2457174.5833333335, a_datetime.toJulianDate());

    Coords::DateTime a_new_datetime(a_datetime.inTimezoneOffset(-4));

    std::stringstream out;
    out << a_new_datetime;

    EXPECT_STREQ("2015-05-31T22:00:00.0-0400", out.str().c_str());

  }


  TEST(DateTime, set_timezone_2015jun01_t55z_a) {

    // timezone 5.5 as double month change

    std::string a_datetime_string("2015-06-01T02:00:00");
    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(2457174.5833333335, a_datetime.toJulianDate());

    Coords::DateTime a_new_datetime(a_datetime.inTimezoneOffset(5.5));

    std::stringstream out;
    out << a_new_datetime;

    EXPECT_STREQ("2015-06-01T07:30:00.0+0530", out.str().c_str());

  }


  TEST(DateTime, set_timezone_2015jun01_tz4_na) {

    // timezone 4 June month change

    std::string a_datetime_string("2015-05-31T22:00:00");
    Coords::DateTime a_datetime(a_datetime_string);
    EXPECT_DOUBLE_EQ(2457174.4166666665, a_datetime.toJulianDate());

    Coords::DateTime a_new_datetime(a_datetime.inTimeZone(Coords::TimeZone("+4")));

    std::stringstream out;
    out << a_new_datetime;

    EXPECT_STREQ("2015-06-01T02:00:00.0+0400", out.str().c_str());

  }


  TEST(DateTime, set_timezone_2015jun30_1) {

    // month change forward

    std::string a_datetime_string("2015-06-30T22:15:00-0800");
    Coords::DateTime a_datetime(a_datetime_string);

    Coords::DateTime a_new_datetime(a_datetime.inTimezone("+03:00"));

    EXPECT_DOUBLE_EQ(a_datetime.toJulianDate(), a_new_datetime.toJulianDate());

    std::stringstream out;
    out << a_new_datetime;

    EXPECT_STREQ("2015-07-01T09:15:00.0+03:00", out.str().c_str());

  }

  TEST(DateTime, set_timezone_2015jun30_2) {

    // month change backwards

    std::string a_datetime_string("2015-07-01T04:15:00+0300");
    Coords::DateTime a_datetime(a_datetime_string);

    Coords::DateTime a_new_datetime(a_datetime.inTimeZone(Coords::TimeZone(-8)));

    EXPECT_DOUBLE_EQ(a_datetime.toJulianDate(), a_new_datetime.toJulianDate());

    std::stringstream out;
    out << a_new_datetime;

    EXPECT_STREQ("2015-06-30T17:15:00.0-0800", out.str().c_str());

  }


  TEST(DateTime, set_timezone_2016jun01_n0800) {

    // day change, positive time zone, 31 day month

    std::string a_datetime_string("2016-01-01T04:30:00+0100");
    Coords::DateTime a_datetime(a_datetime_string);

    Coords::DateTime happy_last_year(a_datetime.inTimeZone(Coords::TimeZone("-08:00")));

    EXPECT_DOUBLE_EQ(a_datetime.toJulianDate(), happy_last_year.toJulianDate());

    std::stringstream out;
    out << happy_last_year;

    EXPECT_STREQ("2015-12-31T19:30:00.0-08:00", out.str().c_str());

  }

  // ----- inTimezone shifts that change years -----



  TEST(DateTime, set_timezone_2015jan01_0530) {

    // year change, new deli to new york

    std::string a_datetime_string("2015-01-01T03:30:00+05:30");
    Coords::DateTime a_datetime(a_datetime_string);

    Coords::DateTime happy_new_year(a_datetime.inTimeZone(Coords::TimeZone("-5:00")));

    EXPECT_DOUBLE_EQ(a_datetime.toJulianDate(), happy_new_year.toJulianDate());

    std::stringstream out;
    out << happy_new_year;

    EXPECT_STREQ("2014-12-31T17:00:00.0-05:00", out.str().c_str());


  }


  TEST(DateTime, set_timezone_2015dec31_0800) {

    // year change

    std::string a_datetime_string("2015-12-31T20:45:15");
    Coords::DateTime a_datetime(a_datetime_string);

    Coords::DateTime happy_new_year(a_datetime.inTimeZone(Coords::TimeZone("0800")));

    EXPECT_DOUBLE_EQ(a_datetime.toJulianDate(), happy_new_year.toJulianDate());

    std::stringstream out;
    out << happy_new_year;

    EXPECT_STREQ("2016-01-01T04:45:15.0+0800", out.str().c_str());

  }


  // ----- multiple inTimezone shifts -----

  TEST(DateTime, set_timezone_sf2nd_1) {

    std::string a_datetime_string("2015-06-21T23:00:00-08");
    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_DOUBLE_EQ(2457195.7916666665, a_datetime.toJulianDate());

    Coords::DateTime sf2nd(a_datetime.inTimeZone(Coords::TimeZone("0530")));

    EXPECT_DOUBLE_EQ(2457195.7916666665, sf2nd.toJulianDate());

    std::stringstream out;
    out << sf2nd;

    EXPECT_STREQ("2015-06-22T12:30:00.0+0530", out.str().c_str());

  }


  TEST(DateTime, set_timezone_utopia_2_sf) {

    // there is no timezone -01:00 country?

    std::string a_datetime_string("2015-05-21T05:00:00-01:00");
    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_DOUBLE_EQ(2457163.75, a_datetime.toJulianDate());

    Coords::DateTime utopia2sf(a_datetime.inTimeZone(Coords::TimeZone("-08:00")));

    EXPECT_DOUBLE_EQ(2457163.75, utopia2sf.toJulianDate());

    std::stringstream out;
    out << utopia2sf;

    EXPECT_STREQ("2015-05-20T22:00:00.0-08:00", out.str().c_str());

  }


  TEST(DateTime, set_timezone_berlin2moscow_1) {

    const double zulu_jdate(2458739);

    std::string z_datetime_string("2019-09-12T12:00:00Z");
    Coords::DateTime z_datetime(z_datetime_string);
    EXPECT_DOUBLE_EQ(zulu_jdate, z_datetime.toJulianDate());


    std::string berlin_datetime_string("2019-09-12T14:00:00+02:00");
    Coords::DateTime berlin_datetime(berlin_datetime_string);
    EXPECT_DOUBLE_EQ(zulu_jdate, berlin_datetime.toJulianDate());


    std::string moscow_datetime_string("2019-09-12T15:00:00+03:00");
    Coords::DateTime moscow_datetime(moscow_datetime_string);
    EXPECT_DOUBLE_EQ(zulu_jdate, moscow_datetime.toJulianDate());


    Coords::DateTime b2m(berlin_datetime.inTimeZone(Coords::TimeZone("+03:00")));
    EXPECT_DOUBLE_EQ(zulu_jdate, b2m.toJulianDate());

    std::stringstream out;
    out << b2m;

    EXPECT_STREQ("2019-09-12T15:00:00.0+03:00", out.str().c_str());

  }


  TEST(DateTime, set_timezone_zulu_0) {

    std::string a_datetime_string("2015-05-21T23:00:00Z");
    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_DOUBLE_EQ(2457164.4583333335, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2015-05-21T23:00:00.0Z", out.str().c_str());

  }


  TEST(DateTime, set_timezone_zulu_to_timezone_3) {

    std::string a_datetime_string("2015-05-21T12:00:00Z");
    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_DOUBLE_EQ(2457164, a_datetime.toJulianDate());

    Coords::DateTime a_new_datetime(a_datetime.inTimeZone(Coords::TimeZone("03:00")));

    std::stringstream out;
    out << a_new_datetime;

    EXPECT_STREQ("2015-05-21T15:00:00.0+03:00", out.str().c_str());

  }

  // timezone +0100

  TEST(DateTime, set_timezone_zulu_vs_0100) {
    // timezone zulu vs timezone 1

    std::string z_datetime_string("2015-05-22T00:00:00Z");
    Coords::DateTime z_datetime(z_datetime_string);

    EXPECT_DOUBLE_EQ(2457164.5, z_datetime.toJulianDate());

    std::string a_datetime_string("2015-05-22T01:00:00+0100");
    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_DOUBLE_EQ(2457164.5, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2015-05-22T01:00:00.0+0100", out.str().c_str());

    EXPECT_DOUBLE_EQ(a_datetime.toJulianDate(), z_datetime.toJulianDate());

  }


  // timezone -0100


  TEST(DateTime, set_timezone_zulu_vs_n0100) {
    // timezone zulu vs timezone -1

    std::string z_datetime_string("2015-05-22T00:30:00Z");
    Coords::DateTime z_datetime(z_datetime_string);

    EXPECT_DOUBLE_EQ(2457164.5208333335, z_datetime.toJulianDate());

    std::string a_datetime_string("2015-05-21T23:30:00-0100");
    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_DOUBLE_EQ(2457164.5208333335, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2015-05-21T23:30:00.0-0100", out.str().c_str());

    EXPECT_DOUBLE_EQ(a_datetime.toJulianDate(), z_datetime.toJulianDate());

  }



  // ----- timezone +05:30 -----

  TEST(DateTime, set_timezone_zulu_vs_0530) {
    // timezone zulu vs timezone +05:30

    std::string z_datetime_string("2019-08-15T00:00:00Z");
    Coords::DateTime z_datetime(z_datetime_string);

    EXPECT_DOUBLE_EQ(2458710.5, z_datetime.toJulianDate());

    std::string a_datetime_string("2019-08-15T05:30:00+05:30");
    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_DOUBLE_EQ(2458710.5, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2019-08-15T05:30:00.0+05:30", out.str().c_str());

    EXPECT_DOUBLE_EQ(a_datetime.toJulianDate(), z_datetime.toJulianDate());

  }


  TEST(DateTime, set_timezone_05c30) {
    // timezone +0530 with colon

    std::string a_datetime_string("2019-08-15T21:40:00+05:30");
    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_DOUBLE_EQ(2458711.173611111, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2019-08-15T21:40:00.0+05:30", out.str().c_str());

  }


  TEST(DateTime, set_timezone_0530) {
    // timezone +0530 with out colon

    std::string a_datetime_string("2019-08-15T08:10:00+0530");
    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_DOUBLE_EQ(2458710.6111111115, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2019-08-15T08:10:00.0+0530", out.str().c_str());

  }

  // ----- timezone -0800 -----

  TEST(DateTime, set_timezone_zulu_vs_n0800) {
    // timezone zulu vs timezone -8

    std::string z_datetime_string("2016-07-09T20:00:00Z");
    Coords::DateTime z_datetime(z_datetime_string);

    EXPECT_DOUBLE_EQ(2457579.3333333335, z_datetime.toJulianDate());

    std::string a_datetime_string("2016-07-09T12:00:00-0800");
    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_DOUBLE_EQ(2457579.3333333335, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2016-07-09T12:00:00.0-0800", out.str().c_str());

    EXPECT_DOUBLE_EQ(a_datetime.toJulianDate(), z_datetime.toJulianDate());

  }


  TEST(DateTime, set_timezone_n08c00) {
    // timezone -8 with colon

    std::string a_datetime_string("2016-07-10T04:30:00-08:00");
    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_DOUBLE_EQ(2457580.0208333335, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2016-07-10T04:30:00.0-08:00", out.str().c_str());

  }


  TEST(DateTime, set_timezone_n0800) {
    // timezone -8 with out colon

    std::string a_datetime_string("2016-07-11T20:30:00-0800");
    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_DOUBLE_EQ(2457581.6875, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2016-07-11T20:30:00.0-0800", out.str().c_str());

  }


  // ----- timezone format tests -----


  TEST(DateTime, set_timezone_01c00) {
    // timezone 1 with colon

    std::string a_datetime_string("2015-05-21T23:00:00+01:00");
    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_DOUBLE_EQ(2457164.4166666665, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2015-05-21T23:00:00.0+01:00", out.str().c_str());

  }


  TEST(DateTime, set_timezone_0100) {
    // timezone 1 with out colon

    std::string a_datetime_string("2015-05-21T01:00:00+0100");
    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_DOUBLE_EQ(2457163.5, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2015-05-21T01:00:00.0+0100", out.str().c_str());

  }


  TEST(DateTime, set_timezone_n01c00) {
    // timezone -1 with colon

    std::string a_datetime_string("2016-06-05T11:00:00-01:00");
    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_DOUBLE_EQ(2457545, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2016-06-05T11:00:00.0-01:00", out.str().c_str());

  }


  TEST(DateTime, set_timezone_n0100) {
    // timezone -1 with colon

    std::string a_datetime_string("2016-06-06T23:00:00-0100");
    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_DOUBLE_EQ(2457546.5, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2016-06-06T23:00:00.0-0100", out.str().c_str());

  }


  TEST(DateTime, set_timezone_default) {
    // timezone default

    Coords::DateTime a_datetime(2019, 11, 21, 03, 00, 00);

    EXPECT_DOUBLE_EQ(2458808.625, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2019-11-21T03:00:00.0", out.str().c_str());

  }


  TEST(DateTime, set_timezone_int3) {
    // timezone int 3

    Coords::DateTime a_datetime(2015, 05, 21, 23, 00, 00, 3);

    EXPECT_DOUBLE_EQ(2457164.3333333335, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2015-05-21T23:00:00.0+0300", out.str().c_str());

  }


  TEST(DateTime, set_timezone_n8) {
    // timezone int -8

    Coords::DateTime a_datetime(2019, 12, 26, 12, 00, 10.54321, -8);

    EXPECT_DOUBLE_EQ(2458844.3334553614, a_datetime.toJulianDate());

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2019-12-26T12:00:10.5-0800", out.str().c_str());

  }


  TEST(DateTime, set_timezone_15) {
    // timezone int 15

    EXPECT_THROW(Coords::DateTime a_datetime(2019, 12, 26, 12, 00, 10.54321, 15), Coords::Error);

  }

  TEST(DateTime, set_timezone_n12) {
    // timezone int -12

    EXPECT_THROW(Coords::DateTime a_datetime(2019, 12, 26, 12, 00, 10.54321, -12.1), Coords::Error);

  }


  // ----- timezone out of range -----


  TEST(DateTime, set_timezone_range_too_hi) {

    std::string a_datetime_string("2015-05-21T23:00:00Z");
    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_THROW(a_datetime.inTimeZone(Coords::TimeZone("1201")), Coords::Error);
  }


  TEST(DateTime, set_timezone_range_too_lo) {

    std::string a_datetime_string("2015-05-21T23:00:00Z");
    Coords::DateTime a_datetime(a_datetime_string);

    EXPECT_THROW(a_datetime.inTimeZone(Coords::TimeZone("-12:01")), Coords::Error);
  }




  // ----- leap years -----

  TEST(DateTime, set_timezone_leap_day_2015feb29_b) {

    // leap day in a non leap year

    EXPECT_THROW(Coords::DateTime a_datetime_string("2015-02-29T22:15:00-0800"), Coords::Error);

  }


  TEST(DateTime, set_timezone_leap_day_2016feb29_a) {

    // leap year

    std::string a_datetime_string("2016-02-29T22:15:00-0800");
    Coords::DateTime a_datetime(a_datetime_string);

    Coords::DateTime a_new_datetime(a_datetime.inTimeZone(Coords::TimeZone("+03:00")));

    EXPECT_DOUBLE_EQ(a_datetime.toJulianDate(), a_new_datetime.toJulianDate());

    std::stringstream out;
    out << a_new_datetime;

    EXPECT_STREQ("2016-03-01T09:15:00.0+03:00", out.str().c_str());

  }


  TEST(DateTime, set_timezone_2015feb28_a) {

    // day change, positive time zone, feb non-leap year

    std::string a_datetime_string("2015-03-01T03:30:00+06:00");
    Coords::DateTime a_datetime(a_datetime_string);

    Coords::DateTime a_new_datetime(a_datetime.inTimeZone(Coords::TimeZone("-07:00")));

    EXPECT_DOUBLE_EQ(a_datetime.toJulianDate(), a_new_datetime.toJulianDate());

    std::stringstream out;
    out << a_new_datetime;

    EXPECT_STREQ("2015-02-28T14:30:00.0-07:00", out.str().c_str());

  }


  TEST(DateTime, ouroboros_1) {

    // output == input

    std::string a_datetime_string("2019-09-15T06:30:00.0-08:00");
    Coords::DateTime a_datetime(a_datetime_string);

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ(a_datetime_string.c_str(), out.str().c_str());

    Coords::DateTime b_datetime(out.str());

    EXPECT_DOUBLE_EQ(a_datetime.toJulianDate(), b_datetime.toJulianDate());

  }



  TEST(DateTime, set_timezone_2016mar01_leap_back) {

    // leap day error!

    std::string a_datetime_string("2016-03-01T03:00:00+03");
    Coords::DateTime a_datetime(a_datetime_string);

    a_datetime -= 1.0/24;
    a_datetime -= 1.0/24;
    a_datetime -= 1.0/24;
    a_datetime -= 1.0/24;

    std::stringstream out;
    out << a_datetime;

    EXPECT_STREQ("2016-02-29T23:00:00+03:00", out.str().c_str());
  }






} // end anonymous namespace



// ==================
// ===== main() =====
// ==================

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
