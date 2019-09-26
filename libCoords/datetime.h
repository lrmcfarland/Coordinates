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

  // --------------------
  // ----- TimeZone -----
  // --------------------

  class TimeZone {

  public:

    static const std::string s_format;

#if BOOST_REGEX
    static const boost::regex s_regex;
#else
    static const std::regex s_regex;
#endif

    void isValid(const double& an_offset);
    void throwError(const std::string& a_timezone, const std::string msg);

    const bool& isLocal() const {return m_is_local;}
    const bool& isZulu() const {return m_is_zulu;}
    const bool& hasColon() const {return m_has_colon;}

    // ----- constructors -----

    explicit TimeZone(const std::string& a_timezone="00:00");
    explicit TimeZone(const double& a_timezone);
    explicit TimeZone(const int& a_timezone);

    ~TimeZone() {};

    TimeZone(const TimeZone& a);
    TimeZone& operator=(const TimeZone& rhs);

    // accessors

    const double& offset() const {return m_offset;}


  private:

    bool m_has_colon; // for operator<<() idempotence
    bool m_is_local;
    bool m_is_zulu;

    double m_offset; // timezone as double, e.g. 5:30 > 5.5

  };

  // --------------------
  // ----- DateTime -----
  // --------------------

  class DateTime {

  public:

    static const std::string s_ISO8601_format;

#if BOOST_REGEX
    static const boost::regex s_ISO8601_regex;
#else
    static const std::regex s_ISO8601_regex;
#endif

    static const long int s_gDateNRC; // used in NRC Julian Date calculations.

    static const double   s_LilianDate; // Gregorian calendar adopted Oct. 15, 1582
    static const double   s_ModifiedJulianDate; // 1858-11-17T00:00:00
    static const double   s_TruncatedJulianDate; // 1968-05-24T00:00:00
    static const double   s_J2000; // 2000-01-01T12:00:00Z

    static const double   s_resolution; // for rounding seconds

    // ----- constructors -----

    explicit DateTime(const std::string& an_iso8601_time);

    explicit DateTime(const double& a_jdate);

    explicit DateTime(const int& a_year = 1970, // Unix epoch
		      const int& a_month = 1,
		      const int& a_day = 1,
		      const int& a_hour = 0,
		      const int& a_minute = 0,
		      const double& a_second = 0,
		      const std::string& a_timezone="")
      : m_year(a_year),
      m_month(a_month),
      m_day(a_day),
      m_hour(a_hour),
      m_minute(a_minute),
      m_second(a_second),
      m_timezone(a_timezone)
      {isValid();};

    explicit DateTime(const int& a_year,
		      const int& a_month,
		      const int& a_day,
		      const int& a_hour,
		      const int& a_minute,
		      const double& a_second,
		      const double& a_timezone)
      : m_year(a_year),
      m_month(a_month),
      m_day(a_day),
      m_hour(a_hour),
      m_minute(a_minute),
      m_second(a_second),
      m_timezone(a_timezone)
      {isValid();};

    explicit DateTime(const int& a_year,
		      const int& a_month,
		      const int& a_day,
		      const int& a_hour,
		      const int& a_minute,
		      const double& a_second,
		      const Coords::TimeZone& a_timezone)
      : m_year(a_year),
      m_month(a_month),
      m_day(a_day),
      m_hour(a_hour),
      m_minute(a_minute),
      m_second(a_second),
      m_timezone(a_timezone)
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

    const bool& isLeapYear() const {return m_is_leap_year;}

    // helpers for Python manual wrappers
    const double& LilianDate() const {return s_LilianDate;}
    const double& ModifiedJulianDate() const {return s_ModifiedJulianDate;}
    const double& TruncatedJulianDate() const {return s_TruncatedJulianDate;}
    const double& J2000() const {return s_J2000;}

    const TimeZone& timezone() const {return m_timezone;}

    double   offset() const {return m_timezone.offset();} // copy for boost

    DateTime inTimeZone(const Coords::TimeZone& a_new_timezone) const;

    DateTime inTimezone(const std::string& a_new_timezone) const { // z for boost
      return inTimeZone(Coords::TimeZone(a_new_timezone));}

    DateTime inTimezoneOffset(const double& a_new_timezone) const { // z for boost
      return inTimeZone(Coords::TimeZone(a_new_timezone));}


    // ----- in-place operators -----

    DateTime& operator+=(const double& rhs);
    DateTime& operator-=(const double& rhs);

    // ----- Julian date methods -----

    double   toJulianDate() const {return toModifiedJulianDateAPC() + s_ModifiedJulianDate;}

    DateTime fromJulianDate(const double& jdays) const {
      return fromModifiedJulianDateAPC(jdays - s_ModifiedJulianDate);} // TODO static method?


    double   toJulianDateWiki() const;
    DateTime fromJulianDateWiki(const double& jdays) const; // TODO static method?


    double   toJulianDateNRC() const;
    DateTime fromJulianDateNRC(const double& jdays) const; // TODO static method?


    double   toModifiedJulianDateAPC() const;
    DateTime fromModifiedJulianDateAPC(const double& jdays) const; // TODO static method?



    // TODO J1950, J2000


  private:

    int m_year;
    int m_month;
    int m_day;
    int m_hour;
    int m_minute;
    double m_second;

    bool m_is_leap_year;

    TimeZone m_timezone;

  };



  // +++++++++++++++++++++
  // +++++ operators +++++
  // +++++++++++++++++++++

  DateTime operator+(const DateTime& lhs, const double& rhs);
  DateTime operator+(const double& lhs, const DateTime& rhs);

  DateTime operator-(const DateTime& lhs, const double& rhs);
  DateTime operator-(const double& lhs, const DateTime& rhs);


  double operator-(const DateTime& lhs, const DateTime& rhs); // difference in days


  // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
  // <<<<< output operator<<() <<<<<
  // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  void TimeZone2String(const TimeZone& a_timezone, std::stringstream& a_string);

  // inline for boost. Use hpp instead?
  inline std::ostream& operator<< (std::ostream& os, const Coords::TimeZone& a_timezone) {
    std::stringstream out;
    Coords::TimeZone2String(a_timezone, out);
    return os << out.str();
  }


  void DateTime2String(const DateTime& a_datetime, std::stringstream& a_string);

  // inline for boost. Use hpp instead?
  inline std::ostream& operator<< (std::ostream& os, const Coords::DateTime& a_datetime) {
    std::stringstream out;
    Coords::DateTime2String(a_datetime, out);
    return os << out.str();
  }


} // end namespace Coords
