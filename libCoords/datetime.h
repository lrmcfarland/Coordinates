// ================================================================
// Filename:    datetime.h
//
// Description: Yet another DateTime implementation. This is not
//              intended for anything other than a light weight
//              DateTime object for my astronomy applications.
//
// Author:      L.R. McFarland
// Created:     2014 Nov 06
// Language:    C++
//
//  DateTime is free software: you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  DateTime is distributed in the hope that it will be useful, but
//  WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Coordinates. If not, see <http://www.gnu.org/licenses/>.
// ================================================================

#pragma once


#if BOOST_REGEX
#include <boost/regex.hpp>
#else
#include <regex>
#endif

#include <utils.h>

namespace Coords {

  // ====================
  // ===== DateTime =====
  // ====================

  class DateTime {

  public:

    static const std::string s_ISO8601_format;

#if BOOST_REGEX
    static const boost::regex  s_ISO8601_rx;
#else
    static const std::regex  s_ISO8601_rx;
#endif

    static const long int s_gDateNRC; // used in NRC Julian Date calculations.

    static const double   s_LilianDate; // Gregorian calendar adopted Oct. 15, 1582
    static const double   s_ModifiedJulianDate; // 1858-11-17T00:00:00
    static const double   s_TruncatedJulianDate; // 1968-05-24T00:00:00
    static const double   s_J2000; // 2000-01-01T12:00:00Z

    static const double   s_resolution; // for rounding seconds

    static void adjustForTimezone(int& a_year, int& a_month, int& a_day,
				  int& a_hour, int& a_minute, double& a_second,
				  const double& a_timezone);

    // ----- constructors -----

    explicit DateTime(const std::string& an_iso8601_time);

    explicit DateTime(const int& a_year = 1970, // Unix epoch
		      const int& a_month = 1,
		      const int& a_day = 1,
		      const int& a_hour = 0,
		      const int& a_minute = 0,
		      const double& a_second = 0,
		      const double& a_timezone = 0)
      : m_year(a_year), m_month(a_month), m_day(a_day),
      m_hour(a_hour), m_minute(a_minute), m_second(a_second),
      m_is_zulu(false), m_has_timezone_colon(false), m_timezone(a_timezone), m_is_leap_year(false)
      {isValid();};

    ~DateTime() {};

    DateTime(const DateTime& a);
    DateTime& operator=(const DateTime& rhs);

    // ----- accessors -----

    void isValid(const std::string& an_iso8601_time = "");
    void throwError(const std::string& a_datetime, const std::string msg);

    // there are no set value accessors to force the use of the constructor isValid check
    // i.e. do time component arithmatic in the constructor.

    const int& year() const {return m_year;}
    int     getYear() const {return m_year;} // for boost python wrappers

    const int& month() const {return m_month;}
    int     getMonth() const {return m_month;} // for boost python wrappers

    const int& day() const {return m_day;}
    int     getDay() const {return m_day;}  // for boost python wrappers

    const int& hour() const {return m_hour;}
    int     getHour() const {return m_hour;} // for boost python wrappers

    const int& minute() const {return m_minute;}
    int     getMinute() const {return m_minute;} // for boost python wrappers

    const double& second() const {return m_second;}
    double     getSecond() const {return m_second;} // for boost python wrappers

    const bool& isZulu() const {return m_is_zulu;}
    const std::string& timezoneHH() const {return m_timezone_hh;}
    const std::string& timezoneMM() const {return m_timezone_mm;}
    const bool& hasTimezoneColon() const {return m_has_timezone_colon;}
    double timezone() const {return m_timezone;} // copy of timezone for non-const python wrappers
    void timezone(const double& a_timezone); // changes timezone and hour (day, month, year if needed) to keep UT same

    double getTimezone() {return timezone();} // for boost python wrappers
    void setTimezone(const double& a_timezone) {return timezone(a_timezone);} // for boost python wrappers

    // helpers for Python manual wrappers
    const double& LilianDate() const {return s_LilianDate;}
    const double& ModifiedJulianDate() const {return s_ModifiedJulianDate;}
    const double& TruncatedJulianDate() const {return s_TruncatedJulianDate;}
    const double& J2000() const {return s_J2000;}

    double UT() const {return degrees2seconds(hour() + timezone(), minute(), second())/3600.0;}


    // ----- in-place operators -----

    DateTime& operator+=(const double& rhs);
    DateTime& operator-=(const double& rhs);

    // ----- Julian date methods -----

    double toJulianDate() const {return toModifiedJulianDateAPC() + s_ModifiedJulianDate;}
    void   fromJulianDate(const double& jdays, const double& a_timezone=0) {
      fromModifiedJulianDateAPC(jdays - s_ModifiedJulianDate, a_timezone);
    }


    double toJulianDateWiki() const;
    void   fromJulianDateWiki(const double& jdays);

    double toJulianDateNRC() const;
    void   fromJulianDateNRC(const double& jdays);

    double toModifiedJulianDateAPC() const;
    void   fromModifiedJulianDateAPC(const double& jdays, const double& a_timezone=0);


    // TODO J1950, J2000



  private:

    int m_year;
    int m_month;
    int m_day;
    int m_hour;
    int m_minute;
    double m_second;

    bool m_is_zulu;
    std::string m_timezone_hh;
    std::string m_timezone_mm;
    bool m_has_timezone_colon; // for operator<<() idempotence

    double m_timezone;

    bool m_is_leap_year;

  };



  // ---------------------
  // ----- operators -----
  // ---------------------

  DateTime operator+(const DateTime& lhs, const double& rhs);
  DateTime operator+(const double& lhs, const DateTime& rhs);

  DateTime operator-(const DateTime& lhs, const double& rhs);
  double operator-(const DateTime& lhs, const DateTime& rhs);


  // -------------------------------
  // ----- output operator<<() -----
  // -------------------------------

  void DateTime2String(const DateTime& a_datetime, std::stringstream& a_string);

  // inline for boost. Use hpp instead?
  inline std::ostream& operator<< (std::ostream& os, const Coords::DateTime& a_datetime) {
    std::stringstream out;
    Coords::DateTime2String(a_datetime, out);
    return os << out.str();
  }


} // end namespace Coords
