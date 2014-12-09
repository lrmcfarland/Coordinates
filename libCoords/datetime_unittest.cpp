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

  TEST(DateTime, BadMonthConstructor_0) {
    try {
      Coords::DateTime a("2014-00-07T12:34:56");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][Z|(+|-)hh[:][mm]]");
    }
  }

  TEST(DateTime, BadMonthConstructor_1) {
    try {
      Coords::DateTime a("2014-13-07T12:34:56");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][Z|(+|-)hh[:][mm]]");
    }
  }

  TEST(DateTime, GoodMonthConstructor_0) {
    // test regex month group 0
    std::string a_string("2014-02-01T12:34:56");
    Coords::DateTime a_datetime(a_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_string.c_str(), out.str().c_str());
    EXPECT_DOUBLE_EQ(0, a_datetime.timeZone());
  }

  TEST(DateTime, GoodMonthConstructor_1) {
    // test regex month group 1
    std::string a_string("2014-12-01T12:34:56");
    Coords::DateTime a_datetime(a_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_string.c_str(), out.str().c_str());
  }


  // days

  TEST(DateTime, BadDayConstructor_0) {
    try {
      Coords::DateTime a("2014-12-00T12:34:56");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][Z|(+|-)hh[:][mm]]");
    }
  }

  TEST(DateTime, BadDayConstructor_1) {
    try {
      Coords::DateTime a("2014-12-32T12:34:56");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][Z|(+|-)hh[:][mm]]");
    }
  }

  TEST(DateTime, BadDayConstructor_2a) {
    // leap year
    Coords::DateTime b("2012-02-29T12:34:56");
    try {
      Coords::DateTime a("2012-02-30T12:34:56");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "Except for February all alone. It has 28, but 29 each _leap_ year.");
    }
  }

  TEST(DateTime, BadDayConstructor_2b) {
    // not leap year
    Coords::DateTime b("2014-02-28T12:34:56");
    try {
      Coords::DateTime a("2014-02-29T12:34:56");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "Except for February all alone. It has _28_, but 29 each leap year.");
    }
  }

  TEST(DateTime, BadDayConstructor_2c) {
    // leap year mod 400 rule
    Coords::DateTime a("2000-02-29T12:34:56");
    try {
      Coords::DateTime a("2000-02-30T12:34:56");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "Except for February all alone. It has 28, but 29 each _leap_ year.");
    }
  }

  TEST(DateTime, BadDayConstructor_2d) {
    // not leap year
      Coords::DateTime a("2100-02-28T12:34:56");
    try {
      Coords::DateTime a("2100-02-29T12:34:56");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "Except for February all alone. It has _28_, but 29 each leap year.");
    }
  }


  TEST(DateTime, BadDayConstructor_4) {
    try {
      Coords::DateTime a("2014-04-31T12:34:56");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "Thirty days hath September, April, June and November");
    }
  }

  TEST(DateTime, BadDayConstructor_6) {
    try {
      Coords::DateTime a("2014-06-31T12:34:56");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "Thirty days hath September, April, June and November");
    }
  }

  TEST(DateTime, BadDayConstructor_9) {
    try {
      Coords::DateTime a("2014-09-31T12:34:56");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "Thirty days hath September, April, June and November");
    }
  }

  TEST(DateTime, BadDayConstructor_11) {
    try {
      Coords::DateTime a("2014-11-31T12:34:56");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "Thirty days hath September, April, June and November");
    }
  }


  TEST(DateTime, GoodDayConstructor_0) {
    // test regex day group 0
    std::string a_string("2014-12-01T12:34:56");
    Coords::DateTime a_datetime(a_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, GoodDayConstructor_1) {
    // test regex day group 2
    std::string a_string("2014-12-12T12:34:56");
    Coords::DateTime a_datetime(a_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, GoodDayConstructor_2) {
    // test regex day group 2
    std::string a_string("2014-12-29T12:34:56");
    Coords::DateTime a_datetime(a_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, GoodDayConstructor_3) {
    // test regex day group 2
    std::string a_string("2014-12-31T12:34:56.789");
    Coords::DateTime a_datetime(a_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_string.c_str(), out.str().c_str());
  }


  // hours

  TEST(DateTime, BadHourConstructor_1) {
    try {
      Coords::DateTime a("2014-12-31T60:34:56");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][Z|(+|-)hh[:][mm]]");
    }
  }


  TEST(DateTime, GoodHourConstructor_0) {
    // test regex day group 0
    std::string a_string("2014-12-01T12:34:56");
    Coords::DateTime a_datetime(a_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_string.c_str(), out.str().c_str());
  }


  // minutes

  TEST(DateTime, BadMinuteConstructor_1) {
    try {
      Coords::DateTime a("2014-12-31T10:62:56");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][Z|(+|-)hh[:][mm]]");
    }
  }


  TEST(DateTime, GoodMinuteConstructor_0) {
    // test regex day group 0
    std::string a_string("2014-12-01T12:34:56");
    Coords::DateTime a_datetime(a_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_string.c_str(), out.str().c_str());
  }


  // seconds

  TEST(DateTime, BadSecondConstructor_1) {
    try {
      Coords::DateTime a("2014-12-31T10:12:66");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][Z|(+|-)hh[:][mm]]");
    }
  }


  TEST(DateTime, GoodSecondConstructor_0) {
    // test regex day group 0
    std::string a_string("2014-12-01T12:34:56.123");
    Coords::DateTime a_datetime(a_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_string.c_str(), out.str().c_str());
  }

  // time zones

  TEST(DateTime, GoodTimeZoneConstructor_Zulu) {
    std::string a_string("2014-12-07T12:34:56.78Z");
    Coords::DateTime a_datetime(a_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, BadTimeZoneConstructor_1) {
    try {
      Coords::DateTime a("2014-12-07T12:34:56.78+13.987");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][Z|(+|-)hh[:][mm]]");
    }
  }

  TEST(DateTime, BadTimeZoneConstructor_2) {
    try {
      Coords::DateTime a("2014-12-07T12:34:56.78-13.987");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][Z|(+|-)hh[:][mm]]");
    }
  }

  TEST(DateTime, GoodTimeZoneConstructor_1) {
    std::string a_string("2014-12-07T12:34:56.78+04");
    Coords::DateTime a_datetime(a_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_string.c_str(), out.str().c_str());
  }


  TEST(DateTime, GoodTimeZoneConstructor_2) {
    std::string a_string("2014-12-07T12:34:56.78+0430");
    Coords::DateTime a_datetime(a_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_string.c_str(), out.str().c_str());
    EXPECT_DOUBLE_EQ(4.5, a_datetime.timeZone());
  }

  TEST(DateTime, GoodTimeZoneConstructor_3) {
    std::string a_string("2014-12-07T12:34:56.78+04:15");
    Coords::DateTime a_datetime(a_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_string.c_str(), out.str().c_str());
    EXPECT_DOUBLE_EQ(4.25, a_datetime.timeZone());
  }

  TEST(DateTime, GoodTimeZoneConstructor_4) {
    std::string a_string("2014-12-07T12:34:56.78-04");
    Coords::DateTime a_datetime(a_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_string.c_str(), out.str().c_str());
  }


  TEST(DateTime, GoodTimeZoneConstructor_5) {
    std::string a_string("2014-12-07T12:34:56.78-0430");
    Coords::DateTime a_datetime(a_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_string.c_str(), out.str().c_str());
    EXPECT_DOUBLE_EQ(-4.5, a_datetime.timeZone());
  }

  TEST(DateTime, GoodTimeZoneConstructor_6) {
    std::string a_string("2014-12-07T12:34:56.78-04:45");
    Coords::DateTime a_datetime(a_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_string.c_str(), out.str().c_str());
    EXPECT_DOUBLE_EQ(-4.75, a_datetime.timeZone());
  }

  TEST(DateTime, LetsGetBiblical_1) {
    std::string a_string("-5579-03-20T12:00:00");
    Coords::DateTime a_datetime(a_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_string.c_str(), out.str().c_str());
  }


} // end anonymous namespace



// ==================
// ===== main() =====
// ==================

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
