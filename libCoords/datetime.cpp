// ================================================================
// Filename:    datetime.cpp
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

#include <iomanip> // for std::setw() and std::setfill()
#include <sstream>

#define DEBUG_REGEX 0

#if DEBUG_REGEX
#include <iostream>
#endif

#include<datetime.h>
#include <utils.h>

// ----- static data members -----

const std::string Coords::DateTime::ISO8601_format("((?:-)){0,1}(\\d*)-" // year
						   "((?:0[1-9]|1[012]))-" // month
						   "((?:0[1-9]|1[0-9]|2[0-9]|3[01]))"  // day
						   "T"
						   "([0-5]\\d):" // hour
						   "([0-5]\\d):" // minute
						   "([0-5]\\d(\\.\\d*){0,1})" // second
						   "(?:((?:Z)|(\\+|-)((?:0[1-9]|1[12]))(\\:){0,1}(\\d\\d){0,1})){0,1}" // time zone
						   );

const std::regex Coords::DateTime::ISO8601_rx(Coords::DateTime::ISO8601_format);

Coords::DateTime::DateTime(const std::string& an_iso8601_time)
  : m_year(1970), m_month(1), m_day(1),
    m_hour(0), m_minute(0), m_second(0),
    m_is_zulu(false), m_has_time_zone_colon(false), m_time_zone(0), m_is_leap_year(false)
{

  std::smatch m;

  if (!regex_match(an_iso8601_time, m, ISO8601_rx)) {
    std::stringstream emsg;
    emsg << an_iso8601_time << " not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][Z|(+|-)hh[:][mm]]";
    throw Coords::Error(emsg.str());
  }

  m_year = Coords::stoi(m[2]);

  if (m[1] == "-")
    m_year *= -1;

  m_month = Coords::stoi(m[3]);
  m_day = Coords::stoi(m[4]);

  if ((m_month == 9 || m_month == 4 || m_month == 6 || m_month == 11) && m_day > 30) {
    std::stringstream emsg;
    emsg << an_iso8601_time << ": Thirty days hath September, April, June and November";
    throw Coords::Error(emsg.str());
  }

  m_is_leap_year = false;

  if (m_year % 4 == 0 && m_year % 100 != 0)
    m_is_leap_year = true;

  if (m_year % 4 == 0 && m_year % 100 == 0 && m_year % 400 == 0)
    m_is_leap_year = true;

  if (m_is_leap_year) {

    if (m_month == 2 && m_day > 29) {
      std::stringstream emsg;
      emsg << an_iso8601_time << ": Except for February all alone. It has 28, but 29 each _leap_ year.";
      throw Coords::Error(emsg.str());
    }

  } else {

    if (m_month == 2 && m_day > 28) {
      std::stringstream emsg;
      emsg << an_iso8601_time << ": Except for February all alone. It has _28_, but 29 each leap year.";
      throw Coords::Error(emsg.str());
    }

  }

  m_hour = Coords::stoi(m[5]);
  m_minute = Coords::stoi(m[6]);
  m_second = Coords::stod(m[7]);

  if (m[9] == "Z") {
    m_is_zulu = true;
    m_time_zone = 0;

  } else {

    m_time_zone_hh = m[11];
    m_time_zone_mm = m[13];

    m_time_zone = Coords::stod(m[11]);

    if (m[12] == ":")
      m_has_time_zone_colon = true;

    if (m[13] != "")
      m_time_zone += Coords::stod(m[13])/60.0;

    if (m[10] == "-")
      m_time_zone *= -1;

  }

#if DEBUG_REGEX
  for (int i = 0; i < 15; ++i)
    std::cout << "m[" << i << "]" << m[i] << std::endl;
#endif

}

// ----- copy constructor -----

Coords::DateTime::DateTime(const Coords::DateTime& a) {
  m_year = a.m_year;
  m_month = a.m_month;
  m_day = a.m_day;
  m_hour = a.m_hour;
  m_minute = a.m_minute;
  m_second = a.m_second;
  m_is_zulu = a.m_is_zulu;
  m_time_zone_hh = a.m_time_zone_hh;
  m_time_zone_mm = a.m_time_zone_mm;
  m_has_time_zone_colon = a.m_has_time_zone_colon;
  m_time_zone = a.m_time_zone;
  m_is_leap_year = a.m_is_leap_year;
}

// ----- copy assignment -----
Coords::DateTime& Coords::DateTime::operator=(const Coords::DateTime& rhs) {
  if (this == &rhs) return *this;
  m_year = rhs.m_year;
  m_month = rhs.m_month;
  m_day = rhs.m_day;
  m_hour = rhs.m_hour;
  m_minute = rhs.m_minute;
  m_second = rhs.m_second;
  m_is_zulu = rhs.m_is_zulu;
  m_time_zone_hh = rhs.m_time_zone_hh;
  m_time_zone_mm = rhs.m_time_zone_mm;
  m_has_time_zone_colon = rhs.m_has_time_zone_colon;
  m_time_zone = rhs.m_time_zone;
  m_is_leap_year = rhs.m_is_leap_year;
  return *this;
}


// ----- string utility -----

void Coords::DateTime2String(const Coords::DateTime& a_datetime, std::stringstream& a_string) {

  a_string << a_datetime.year() << "-"
	   << std::setw(2) << std::setfill('0') << a_datetime.month() << "-"
	   << std::setw(2) << std::setfill('0') << a_datetime.day()
	   << "T"
	   << std::setw(2) << std::setfill('0') << a_datetime.hour() << ":"
	   << std::setw(2) << std::setfill('0') << a_datetime.minute() << ":"
	   << std::setw(2) << std::setfill('0') << a_datetime.second();

  if (a_datetime.isZulu())
    a_string << "Z";

  if (a_datetime.timeZone() != 0) {

    if (a_datetime.timeZoneHH() != "") {

      // ASSUMES: having timeZoneHH means it was constructed from an ISO-8601 string

      if (a_datetime.timeZone() > 0)
	a_string << "+";
      else
	a_string << "-";

      a_string << a_datetime.timeZoneHH();

      if (a_datetime.hasTimeZoneColon())
	a_string << ":";

      if (a_datetime.timeZoneMM() != "")
	a_string << a_datetime.timeZoneMM();

    } else {

      if (a_datetime.timeZone() > 0)
	a_string << "+";
      a_string << a_datetime.timeZone();

    }
  }
}
