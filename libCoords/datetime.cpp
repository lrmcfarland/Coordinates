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

#include <cmath>
#include <iomanip> // for std::setw() and std::setfill()
#include <sstream>

#define DEBUG_REGEX 0
#if DEBUG_REGEX
#include <iostream>
#endif

#include<datetime.h>
#include <utils.h>

// ----- static data members -----

const std::string Coords::DateTime::s_ISO8601_format("((?:-)){0,1}(\\d*)-" // year
						     "((?:0[1-9]|1[012]))-" // month
						     "((?:0[1-9]|1\\d|2\\d|3[01]))"  // day
						     "T"
						     "((?:[01]\\d|2[0-3])):" // hour
						     "([0-5]\\d):" // minute
						     "([0-5]\\d(\\.\\d*){0,1})" // second
						     "(?:((?:Z)|(\\+|-)((?:0[1-9]|1[12]))(\\:){0,1}([0-5]\\d){0,1})){0,1}" // time zone
						     );

const std::regex Coords::DateTime::s_ISO8601_rx(Coords::DateTime::s_ISO8601_format);

const long int Coords::DateTime::s_gDateNRC(15+31L*(10+12L*1582));
const double Coords::DateTime::s_LilianDate(2299160.5);
const double Coords::DateTime::s_ModifiedJulianDate(2400000.5);
const double Coords::DateTime::s_TruncatedJulianDate(2440000.5);

Coords::DateTime::DateTime(const std::string& an_iso8601_time)
  : m_year(1970), m_month(1), m_day(1),
    m_hour(0), m_minute(0), m_second(0),
    m_is_zulu(false), m_has_time_zone_colon(false), m_time_zone(0), m_is_leap_year(false)
{

  std::smatch m;

  if (!regex_match(an_iso8601_time, m, s_ISO8601_rx)) {
    std::stringstream emsg;
    emsg << an_iso8601_time << " not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][Z|(+|-)hh[:][mm]]";
    throw Coords::Error(emsg.str());
  }

  m_year = Coords::stoi(m[2]);

  if (m[1] == "-")
    m_year *= -1;

  m_month = Coords::stoi(m[3]);
  m_day = Coords::stoi(m[4]);

  if (m_year % 4 == 0 && m_year % 100 != 0)
    m_is_leap_year = true;

  if (m_year % 4 == 0 && m_year % 100 == 0 && m_year % 400 == 0)
    m_is_leap_year = true;

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

  isValid(an_iso8601_time);
}

void Coords::DateTime::isValid(const std::string& an_iso8601_time) throw (Error) {

  if ((m_month == 9 || m_month == 4 || m_month == 6 || m_month == 11) && m_day > 30) {
    std::stringstream emsg;
    emsg << an_iso8601_time << ": Thirty days hath September, April, June and November";
    throw Coords::Error(emsg.str());
  }

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

// ----- as Julian Date -----

double Coords::DateTime::toJulianDateWiki() const {

  // from http://en.wikipedia.org/wiki/Julian_day

  const long int a(floor(14 - m_month)/12);
  const long int y(m_year + 4800 - a);
  const long int m(m_month + 12*a - 3);

  long int jdays(0);

  if (m_day + 31L*(m_month + 12L*m_year) >= s_gDateNRC) {

    jdays = m_day + floor((153*m + 2)/5) + 365*y + floor(y/4) - floor(y/100) + floor(y/400) - 32045;

  } else {

    jdays = m_day + floor((153*m + 2)/5) + 365*y + floor(y/4) - 32083;

  }

  double partial_day((Coords::degrees2seconds(m_hour, m_minute, m_second) + timeZone()*3600)/86400.0);

  return static_cast<double>(jdays) + partial_day;

}


void Coords::DateTime::fromJulianDateWiki(const double& jdays) {

  // from http://en.wikipedia.org/wiki/Julian_day

  // TODO: this does not correct for the Lilian date change. See unit tests.

  const long int y(4716);
  const long int j(1401);
  const long int m(2);
  const long int n(12);
  const long int r(4);
  const long int p(1461);
  const long int v(3);
  const long int u(5);
  const long int s(153);
  const long int w(2);
  const long int B(274277);
  const long int C(-38);

  long int f(jdays + j + (((4*jdays + B) / 146097)*3)/4 + C);
  long int e(r*f+v);
  long int g((e%p)/r);
  long int h(u*g+w);

  m_day = (h%s)/u + 1;

  m_month = (h/s + m) % n + 1;

  m_year = e/p - y + (n + m - m_month)/n;

  double d_hour = 24.0 * (jdays - floor(jdays)) + timeZone();
  m_hour = d_hour; // implicit cast to int

  double d_minute = 60.0 * (d_hour - floor(d_hour));
  m_minute = d_minute; // implicit cast to int

  m_second = 60.0 * (d_minute - floor(d_minute));

  if (m_second < 0.0001)
    m_second = 0; // meh.

}


double Coords::DateTime::toJulianDateNRC() const {

  // Calculates Julian day number from Gregorian calendar date.
  // From Numerical Recipes in C, pp. 11-12

  // WARNING: this rounds to the nearest day. See datetime_unittest.cpp for details.

  long int l_year(static_cast<long int>(m_year));   // long and local
  long int l_month(static_cast<long int>(m_month));
  long int l_day(static_cast<long int>(m_day));

  long int jdays(0);

  if (m_year == 0)
    throw Coords::Error("There is no year zero in this algorithm, but there should be.");

  if (l_year < 0)
    ++l_year;

  if (l_month > 2) {
    l_month = m_month + 1;
  } else {
    --l_year;
    l_month = m_month + 13;
  }

  jdays = static_cast<long int>(floor(365.25*l_year) + floor(30.6001*l_month) + l_day + 1720995);

  if (m_day + 31L*(m_month + 12L*m_year) >= s_gDateNRC) {
    int ja = static_cast<int>(0.01*l_year);
    jdays += 2 - ja + static_cast<int>(0.25*ja);
  }

  double partial_day((Coords::degrees2seconds(m_hour, m_minute, m_second) + timeZone()*3600)/86400.0);

  return static_cast<double>(jdays) + partial_day;

}

void Coords::DateTime::fromJulianDateNRC(const double& jdays) {

  // Calculates Gregorian calendar date from Julian day number.
  // From Numerical Recipes in C, pp. 14-15

  long int ja(0);
  long int jalpha(0);
  long int jb(0);
  long int jc(0);
  long int jd(0);
  long int je(0);


  if (jdays >= s_gDateNRC) {
    jalpha = static_cast<long int>((static_cast<float>(jdays - 1867216) - 0.25)/36524.25);
    ja = jdays + 1 + jalpha - static_cast<long int>(0.25*jalpha);
  } else
    ja = jdays;

  jb = ja + 1524;
  jc = static_cast<long int>(6680.0 + (static_cast<float>(jb - 2439870) - 122.1)/365.25);
  jd = static_cast<long int>(365 * jc + (0.25*jc));
  je = static_cast<long int>((jb - jd)/30.6001);

  m_day = jb - jd - static_cast<long int>(30.6001*je);
  m_month = je - 1;

  if (m_month > 12)
    m_month -= 12;

  m_year = jc - 4715;

  if (m_month > 2)
    --m_year;

  if (m_year <= 0)
    --m_year;

  // TODO partial day?

}


double Coords::DateTime::toModifiedJulianDateAPC() const {

  // Calculates Julian day number from Gregorian calendar date.
  // from Astronomy on the Personal Computer, Montenbruck and Pfleger, p. 15

  long int l_year(static_cast<long int>(m_year));   // long and local. does not alter m_year.
  long int l_month(static_cast<long int>(m_month));
  long int l_day(static_cast<long int>(m_day));

  long int jdays(0);
  long int b(0);

  if (m_month <= 2) {
    l_month += 12;
    --l_year;
  }

  if ((10000L*l_year + 100L*l_month + l_day) <= 15821004L)
    b = -2 + ((l_year + 4716)/4) - 1179; // Julian calendar
  else
    b = (l_year/400) - (l_year/100) + (l_year/4); // Gregorian calendar

  jdays = 365L*l_year - 679004L + b + static_cast<int>(30.6001*(l_month+1)) + l_day; // at midnight

  double partial_day((Coords::degrees2seconds(m_hour, m_minute, m_second) + timeZone()*3600)/86400.0);

  return static_cast<double>(jdays) + partial_day;

}


void Coords::DateTime::fromModifiedJulianDateAPC(const double& jdays) {

  // Calculates Gregorian calendar date from Julian day number.
  // from Astronomy on the Personal Computer, Montenbruck and Pfleger, p. 15-16

  // ASSUMES: jdays are Modified Julian Days

  long int a(0);
  long int b(0);
  long int c(0);
  long int d(0);
  long int e(0);
  long int f(0);

  a = static_cast<long int>(jdays + 2400001.0);

  if (a < 2299161) {
    b = 0;
    c = a + 1524; // Julian calendar
  } else {
    b = static_cast<long int>((a - 1867216.25)/36524.25);
    c = a + b - (b/4) + 1525; // Gregorian calendar
  }

  d = static_cast<long int>((c - 122.1)/365.25);
  e = 365*d + d/4;
  f = static_cast<long int>((c - e)/30.6001);

  m_day = c - e - static_cast<int>(30.6001 * f);
  m_month = f - 1 - 12*(f/14);
  m_year = d - 4715 - ((7+m_month)/10);

  double d_hour = 24.0 * (jdays - floor(jdays)) + timeZone();
  m_hour = d_hour; // implicit cast to int

  double d_minute = 60.0 * (d_hour - floor(d_hour));
  m_minute = d_minute; // implicit cast to int

  m_second = 60.0 * (d_minute - floor(d_minute));

  if (m_second < 0.00001)
    m_second = 0; // meh.

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
