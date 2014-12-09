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

    static const std::string ISO8601_format;
    static const std::regex  ISO8601_rx;

    explicit DateTime(const std::string& an_iso8601_time);

    // numeric constructor has no range checks. ISO-8601 only or remove? bug or feature?
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
    {};

    ~DateTime() {};

    DateTime(const DateTime& a);
    DateTime& operator=(const DateTime& rhs);

    // accessors

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


    // TODO Julian Date, Modified Julian date, J1950, J2000

    double asJulianDate();


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
