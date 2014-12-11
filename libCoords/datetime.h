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

#include <regex>

#include <utils.h>

namespace Coords {

  // ====================
  // ===== DateTime =====
  // ====================

  class DateTime {

  public:

    static const std::string s_ISO8601_format;
    static const std::regex  s_ISO8601_rx;

    static const long int s_gDateNRC; // used in NRC Julian Date calculations.

    static const double   s_LilianDate; // Gregorian calendar adopted Oct. 15, 1582
    static const double   s_ModifiedJulianDate; // 1858-11-17T00:00:00
    static const double   s_TruncatedJulianDate; // 1968-05-23T12:00:00

    explicit DateTime(const std::string& an_iso8601_time);

    explicit DateTime(const int& a_year = 1970, // Unix epoch
		      const int& a_month = 1,
		      const int& a_day = 1,
		      const int& a_hour = 0,
		      const int& a_minute = 0,
		      const double& a_second = 0,
		      const double& a_time_zone = 0)
      : m_year(a_year), m_month(a_month), m_day(a_day),
      m_hour(a_hour), m_minute(a_minute), m_second(a_second),
      m_is_zulu(false), m_has_time_zone_colon(false), m_time_zone(a_time_zone), m_is_leap_year(false)
      {isValid();};

    ~DateTime() {};

    DateTime(const DateTime& a);
    DateTime& operator=(const DateTime& rhs);

    // accessors
    void isValid(const std::string& an_iso8601_time = "") throw (Error);
    const int& year() const {return m_year;}
    const int& month() const {return m_month;}
    const int& day() const {return m_day;}
    const int& hour() const {return m_hour;}
    const int& minute() const {return m_minute;}
    const double& second() const {return m_second;}

    const bool& isZulu() const {return m_is_zulu;}
    const std::string& timeZoneHH() const {return m_time_zone_hh;}
    const std::string& timeZoneMM() const {return m_time_zone_mm;}
    const bool& hasTimeZoneColon() const {return m_has_time_zone_colon;}
    const double& timeZone() const {return m_time_zone;}


    // Julian date methods

    double toJulianDate() const {return toModifiedJulianDateAPC() + s_ModifiedJulianDate;}
    void   fromJulianDate(const double& jdays) {fromModifiedJulianDateAPC(jdays - s_ModifiedJulianDate);}


    double toJulianDateWiki() const;
    void   fromJulianDateWiki(const double& jdays);

    double toJulianDateNRC() const;
    void   fromJulianDateNRC(const double& jdays);

    double toModifiedJulianDateAPC() const;
    void   fromModifiedJulianDateAPC(const double& jdays);


    // TODO J1950, J2000



  private:

    int m_year;
    int m_month;
    int m_day;
    int m_hour;
    int m_minute;
    double m_second;

    bool m_is_zulu;
    std::string m_time_zone_hh;
    std::string m_time_zone_mm;
    bool m_has_time_zone_colon; // for operator<<() idempotence

    double m_time_zone;

    bool m_is_leap_year;

  };



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
