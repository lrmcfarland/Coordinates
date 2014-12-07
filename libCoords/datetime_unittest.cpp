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
    Coords::DateTime a;
    // TODO expect something
  }

  // TODO years?

  // months

  TEST(DateTime, BadMonthConstructor_0) {
    try {
      Coords::DateTime a("2014-00-07T12:34:56");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "not ISO8601 format");
    }
  }

  TEST(DateTime, BadMonthConstructor_1) {
    try {
      Coords::DateTime a("2014-13-07T12:34:56");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "not ISO8601 format");
    }
  }

  TEST(DateTime, GoodMonthConstructor_0) {
    // test regex month group 0
    std::string a_string("2014-02-01T12:34:56");
    Coords::DateTime a_datetime(a_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_string.c_str(), out.str().c_str());
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
      EXPECT_STREQ(err.what(), "not ISO8601 format");
    }
  }

  TEST(DateTime, BadDayConstructor_1) {
    try {
      Coords::DateTime a("2014-12-32T12:34:56");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "not ISO8601 format");
    }
  }

  TEST(DateTime, BadDayConstructor_2a) {
    // leap year
    Coords::DateTime b("2012-02-29T12:34:56");
    try {
      Coords::DateTime a("2012-02-30T12:34:56");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "Except for February all alone. It has 28, but 29 each leap year.");
    }
  }

  TEST(DateTime, BadDayConstructor_2b) {
    // not leap year
    Coords::DateTime b("2014-02-28T12:34:56");
    try {
      Coords::DateTime a("2014-02-29T12:34:56");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "Except for February all alone. It has 28, but 29 each leap year.");
    }
  }

  TEST(DateTime, BadDayConstructor_2c) {
    // leap year mod 400 rule
    Coords::DateTime a("2000-02-29T12:34:56");
    try {
      Coords::DateTime a("2000-02-30T12:34:56");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "Except for February all alone. It has 28, but 29 each leap year.");
    }
  }

  TEST(DateTime, BadDayConstructor_2d) {
    // not leap year
      Coords::DateTime a("2100-02-28T12:34:56");
    try {
      Coords::DateTime a("2100-02-29T12:34:56");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "Except for February all alone. It has 28, but 29 each leap year.");
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
      EXPECT_STREQ(err.what(), "not ISO8601 format");
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
      EXPECT_STREQ(err.what(), "not ISO8601 format");
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
      EXPECT_STREQ(err.what(), "not ISO8601 format");
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
      EXPECT_STREQ(err.what(), "not ISO8601 format");
    }
  }

  TEST(DateTime, BadTimeZoneConstructor_2) {
    try {
      Coords::DateTime a("2014-12-07T12:34:56.78-13.987");
    } catch (Coords::Error& err) {
      EXPECT_STREQ(err.what(), "not ISO8601 format");
    }
  }

  TEST(DateTime, GoodTimeZoneConstructor_1) {
    std::string a_string("2014-12-07T12:34:56.78+04");
    Coords::DateTime a_datetime(a_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_string.c_str(), out.str().c_str());
  }


  TEST(DISABLED_DateTime, GoodTimeZoneConstructor_2) {
    // TODO pad leading 0s or support hh:mm format
    std::string a_string("2014-12-07T12:34:56.78+04.5");
    Coords::DateTime a_datetime(a_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_string.c_str(), out.str().c_str());
  }

  TEST(DateTime, GoodTimeZoneConstructor_3) {
    std::string a_string("2014-12-07T12:34:56.78-04");
    Coords::DateTime a_datetime(a_string);
    std::stringstream out;
    out << a_datetime;
    EXPECT_STREQ(a_string.c_str(), out.str().c_str());
  }


  TEST(DISABLED_DateTime, GoodTimeZoneConstructor_4) {
    // TODO pad leading 0s or support hh:mm format
    std::string a_string("2014-12-07T12:34:56.78-04.5");
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
