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
    for (int i(0); i < 60; ++i) {
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
    std::string a_date_string("2014-12-31T60:34:56");
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

  // NRC Julian dates
  // Actual dates taken from http://www.imcce.fr/en/grandpublic/temps/jour_julien.php

  TEST(DISABLED_DateTime, NRCJulianDate_0) {
    std::string a_date_string("-4712-01-01T12:00:00");
    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(0.5, a_datetime.toJulianDateNRC());
    // toJulianDateNRC() rounds to nearest day. Does not start at noon? Does not account for year 0?
  }

  TEST(DISABLED_DateTime, NRCJulianDate_pre_Julian2Gregorian) {
    std::string a_date_string("1582-10-14T00:00:00"); // Gregorian replaces Julian
    Coords::DateTime a_datetime(a_date_string);
    EXPECT_NEAR(2299159.5, a_datetime.toJulianDateNRC(), 0.5);
    // does not account for 10 day "gap" in calendar change.
  }

  TEST(DateTime, NRCJulianDate_post_Julian2Gregorian) {
    std::string a_date_string("1582-10-15T00:00:00"); // Gregorian replaces Julian
    Coords::DateTime a_datetime(a_date_string);
    EXPECT_NEAR(2299160.5, a_datetime.toJulianDateNRC(), 0.5);
    // toJulianDateNRC() rounds to nearest day

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());

  }

  TEST(DateTime, NRCJulianDate_pre_reduced) {
    std::string a_date_string("1858-11-16T12:00:00"); // modified date change switches from noon to midnight
    Coords::DateTime a_datetime(a_date_string);
    EXPECT_NEAR(2400000, a_datetime.toJulianDateNRC(), 0.5);
   // toJulianDateNRC() rounds to nearest day

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("1858-11-16T00:00:00", out.str().c_str());
    // switches noon to midnight
  }

  TEST(DateTime, NRCJulianDate_post_reduced) {
    std::string a_date_string("1858-11-17T00:00:00"); // modified date change switches from noon to midnight
    Coords::DateTime a_datetime(a_date_string);
    EXPECT_NEAR(2400000.5, a_datetime.toJulianDateNRC(), 0.5);
   // toJulianDateNRC() rounds to nearest day

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());

  }

  TEST(DateTime, NRCJulianDate_modified) {
    std::string a_date_string("1968-05-23T12:00:00"); // truncated Julian date
    Coords::DateTime a_datetime(a_date_string);
    EXPECT_NEAR(2440000, a_datetime.toJulianDateNRC(), 0.5);
   // toJulianDateNRC() rounds to nearest day

    Coords::DateTime another_datetime;
    another_datetime.fromJulianDateNRC(a_datetime.toJulianDateNRC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("1968-05-23T00:00:00", out.str().c_str()); // rounds to midnight
  }

  TEST(DateTime, NRCJulianDate_3) {
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

  TEST(DateTime, NRCJulianDate_4) {
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

  // APC Modified Julian Dates

  TEST(DateTime, APCModifiedJulianDate_0) {
    std::string a_date_string("-4712-01-01T12:00:00");
    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(-2400000.5, a_datetime.toModifiedJulianDateAPC());
    // TODO asAPCJulianDate() differs from IMCEE on noon vs. midnight on this date?

    Coords::DateTime another_datetime;
    another_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, APCModifiedJulianDate_pre_Julian2Gregorian) {
    std::string a_date_string("1582-10-14T00:00:00"); // Gregorian replaces Julian
    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(2299159.5 - 2400000.5, a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime;
    another_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ("1582-10-04T00:00:00", out.str().c_str()); // skips 10 days
  }

  TEST(DateTime, APCModifiedJulianDate_post_Julian2Gregorian) {
    std::string a_date_string("1582-10-15T00:00:00"); // Gregorian replaces Julian
    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(2299160.5 - 2400000.5, a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime;
    another_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, APCModifiedJulianDate_pre_reduced) {
    std::string a_date_string("1858-11-16T12:00:00"); // modified date change switches from noon to midnight
    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(2400000 - 2400000.5, a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime;
    another_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, APCModifiedJulianDate_post_reduced) {
    std::string a_date_string("1858-11-17T00:00:00"); // modified date change switches from noon to midnight
    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(2400000.5 - 2400000.5, a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime;
    another_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, APCJulianDate_2) {
    std::string a_date_string("1968-05-23T12:00:00"); // truncated Julian date
    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(2440000 - 2400000.5, a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime;
    another_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, APCModifiedJulianDate_3) {
    std::string a_date_string("2013-01-01T00:30:00");
    Coords::DateTime a_datetime(a_date_string);
    EXPECT_NEAR(2456293.5208333335 - 2400000.5, a_datetime.toModifiedJulianDateAPC(), 0.000000001);

    Coords::DateTime another_datetime;
    another_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, APCModifiedJulianDate_4) {
    std::string a_date_string("2014-12-09T00:00:00");
    Coords::DateTime a_datetime(a_date_string);
    EXPECT_DOUBLE_EQ(2457000.5 - 2400000.5, a_datetime.toModifiedJulianDateAPC());

    Coords::DateTime another_datetime;
    another_datetime.fromModifiedJulianDateAPC(a_datetime.toModifiedJulianDateAPC());

    std::stringstream out;
    out << another_datetime;

    EXPECT_STREQ(a_date_string.c_str(), out.str().c_str());

  }


} // end anonymous namespace



// ==================
// ===== main() =====
// ==================

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
