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

#define DEBUG_REGEX 0

#if DEBUG_REGEX
#include <iostream>
#endif

#include<datetime.h>
#include <utils.h>

// ----- static data members -----

const std::string Coords::DateTime::ISO8601_format("((?:-)){0,1}(\\d*)-" // year
						   "((?:0[1-9]|1[12]))-" // month
						   "((?:0[1-9]|1[0-9]|2[0-9]|3[01]))"  // day
						   "T"
						   "([0-5]\\d):" // hour
						   "([0-5]\\d):" // minute
						   "([0-5]\\d(\\.\\d*){0,1})" // second
						   "(?:((?:Z)|(\\+|-)((?:0[1-9]|1[12]))(\\:){0,1}(\\d\\d){0,1})){0,1}" // time zone
						   );

const std::regex Coords::DateTime::ISO8601_rx(Coords::DateTime::ISO8601_format);

Coords::DateTime::DateTime(const std::string& a_iso8601_time) {

  std::smatch m;

  if (!regex_match(a_iso8601_time, m, ISO8601_rx))
    throw Coords::Error("not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][Z|(+|-)hh[:][mm]]");

  m_year = Coords::stoi(m[2]);

  if (m[1] == "-")
    m_year *= -1;

  m_month = Coords::stoi(m[3]);
  m_day = Coords::stoi(m[4]);

  if ((m_month == 9 || m_month == 4 || m_month == 6 || m_month == 11) && m_day > 30)
    throw Coords::Error("Thirty days hath September, April, June and November");

  m_is_leap_year = false;

  if (m_year % 4 == 0 && m_year % 100 != 0)
    m_is_leap_year = true;

  if (m_year % 4 == 0 && m_year % 100 == 0 && m_year % 400 == 0)
    m_is_leap_year = true;

  if (m_is_leap_year) {
    if (m_month == 2 && m_day > 29)
      throw Coords::Error("Except for February all alone. It has 28, but 29 each _leap_ year.");
  } else {
    if (m_month == 2 && m_day > 28)
      throw Coords::Error("Except for February all alone. It has _28_, but 29 each leap year.");
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

      // ASSUMES: constructed from ISO-8601 string

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
