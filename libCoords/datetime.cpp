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
#include <iostream> // for debugging regex with std::cout

#include<datetime.h>
#include <utils.h>

// ----- static data members -----

// TODO numeric constructor has similar range checks or remove?

// TODO limits on years to more than four digits?
// TODO negative years, i.e. BCE? Not ISO8601? Use numeric constructor?

// TODO time zone as hh|hhmm|hh:mm as in standard, not in decimal hours

const std::string Coords::DateTime::ISO8601_format("(\\d{4})-" // year
						   "((?:0[1-9]|1[12]))-" // month
						   "((?:0[1-9]|1[0-9]|2[0-9]|3[01]))"  // day
						   "T"
						   "([0-5]\\d):" // hour
						   "([0-5]\\d):" // minute
						   "([0-5]\\d(\\.\\d*){0,1})" // second
						   "(?:((?:Z)|(\\+|-)((?:0[1-9]|1[12]))(\\.\\d*){0,1})){0,1}" // time zone
						   );

// TODO "(?:((?:Z)|(\\+|-)((?:0[1-9]|1[12])|(?:0[1-9]|1[12]\\:[05]\\d)|(?:0[1-9]|1[12]|[05]|\\d))))" // time zone

const std::regex Coords::DateTime::ISO8601_rx(Coords::DateTime::ISO8601_format);

Coords::DateTime::DateTime(const std::string& a_iso8601_time) {

  std::smatch m;

  if (!regex_match(a_iso8601_time, m, ISO8601_rx))
    throw Coords::Error("not ISO8601 format");

  m_year = Coords::stoi(m[1]);
  m_month = Coords::stoi(m[2]);
  m_day = Coords::stoi(m[3]);

  if ((m_month == 9 || m_month == 4 || m_month == 6 || m_month == 11) && m_day > 30)
    throw Coords::Error("Thirty days hath September, April, June and November");

  m_is_leap_year = false;

  if (m_year % 4 == 0 && m_year % 100 != 0)
    m_is_leap_year = true;

  if (m_year % 4 == 0 && m_year % 100 == 0 && m_year % 400 == 0)
    m_is_leap_year = true;

  if (m_is_leap_year) {
    if (m_month == 2 && m_day > 29)
      throw Coords::Error("Except for February all alone. It has 28, but 29 each leap year.");
  } else {
    if (m_month == 2 && m_day > 28)
      throw Coords::Error("Except for February all alone. It has 28, but 29 each leap year.");
  }

  m_hour = Coords::stoi(m[4]);
  m_minute = Coords::stoi(m[5]);
  m_second = Coords::stod(m[6]);

  if (m[8] == "Z") {
    m_is_zulu = true;
    m_time_zone = 0;
  } else 
    m_time_zone = Coords::stod(m[8]);

#if 0
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

  if (a_datetime.timeZone() > 0)
    a_string << "+" << std::setw(2) << std::setfill('0') << a_datetime.timeZone();

  if (a_datetime.timeZone() < 0)
    a_string << "-" << std::setw(2) << std::setfill('0') << -a_datetime.timeZone();


}
