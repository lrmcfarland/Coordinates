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


// --------------------
// ----- TimeZone -----
// --------------------

const std::string Coords::TimeZone::s_format("Z|(\\+|-){0,1}(0[0-9]|1[012]|[0-9])(\\:){0,1}([0-5]\\d){0,1}");

#if BOOST_REGEX
const boost::regex Coords::TimeZone::s_regex(Coords::TimeZone::s_format);
#else
const std::regex Coords::TimeZone::s_regex(Coords::TimeZone::s_format);
#endif


Coords::TimeZone::TimeZone(const std::string& a_timezone)
  : m_has_colon(false),
    m_is_local(false),
    m_is_zulu(false),
    m_offset(0)
{

  if (a_timezone == "") {
    m_is_local = true;
    return;
  }

#if BOOST_REGEX
  boost::smatch timezone_match;
  if (!boost::regex_match(a_timezone, timezone_match, s_regex)) {
#else
  std::smatch timezone_match;
  if (!std::regex_match(a_timezone, timezone_match, s_regex)) {
#endif
    std::stringstream emsg;
    emsg << a_timezone
	 << " unsupported timezone format: [Z|(+|-)hh[:mm]] for -12 < hh < 12";
    throw Coords::Error(emsg.str());
  }

#if DEBUG_REGEX
  for (int i = 0; i < 6; ++i)
    std::cout << "timezone_match[" << i << "]" << timezone_match[i] << std::endl;
#endif

  if (timezone_match[0] == "Z") {
    m_is_zulu = true;
    m_offset = 0;

  } else {

    // m_sign = timezone_match[1];
    // m_hours = timezone_match[2];
    // m_minutes = timezone_match[4];

    m_offset = Coords::stod(timezone_match[2]);

    if (timezone_match[3] == ":")
      m_has_colon = true;

    if (timezone_match[4] != "")
      m_offset += Coords::stod(timezone_match[4])/60.0;

    if (timezone_match[1] == "-")
      m_offset *= -1;

  }

  isValid(m_offset);

}


Coords::TimeZone::TimeZone(const double& a_timezone)
  : m_has_colon(false),
    m_is_local(false),
    m_is_zulu(false),
    m_offset(a_timezone)
{
  isValid(m_offset);

  if (a_timezone == 0)
    m_is_zulu = true;
}

Coords::TimeZone::TimeZone(const int& a_timezone)
  : m_has_colon(false),
    m_is_local(false),
    m_is_zulu(false),
    m_offset(a_timezone)
{
  isValid(m_offset);

  if (a_timezone == 0)
    m_is_zulu = true;
}


// ----- copy constructor -----

Coords::TimeZone::TimeZone(const Coords::TimeZone& a) {
  m_has_colon = a.m_has_colon;
  m_is_local = a.m_is_local;
  m_is_zulu = a.m_is_zulu;
  m_offset = a.m_offset;
}

// ----- copy assignment -----

Coords::TimeZone& Coords::TimeZone::operator=(const Coords::TimeZone& rhs) {
  if (this == &rhs) return *this;
  m_has_colon = rhs.m_has_colon;
  m_is_local = rhs.m_is_local;
  m_is_zulu = rhs.m_is_zulu;
  m_offset = rhs.m_offset;
  return *this;
}

void Coords::TimeZone::isValid(const double& an_offset) {

  if (an_offset < -12 || an_offset > 12) {
    std::stringstream emsg;
    emsg << an_offset;
    throwError(emsg.str(), "time zone out of range.");
  }

}

void Coords::TimeZone::throwError(const std::string& a_timezone, const std::string msg) {
  std::stringstream emsg;
  std::stringstream current_timezone;

  if (a_timezone == "")
    current_timezone << *this;
  else
    current_timezone << a_timezone;

  emsg << current_timezone.str() << ": " << msg;
  throw Coords::Error(emsg.str());
}


// --------------------
// ----- DateTime -----
// --------------------


const std::string Coords::DateTime::s_ISO8601_format(
	   "(-){0,1}(\\d*)-" // year
	   "(0[1-9]|1[012])-" // month
	   "(0[1-9]|1\\d|2\\d|3[01])" // day
	   "T"
	   "([01]\\d|2[0-3])" // hour
	   ":"
	   "([0-5]\\d)" // minute
	   "(:"
	   "([0-5]\\d(\\.\\d*){0,1})" // second
	   "([Z\\+-]{0,1}[\\d:]*){0,1}" // time zone
	   "){0,1}"
						     );

#if BOOST_REGEX
const boost::regex Coords::DateTime::s_ISO8601_regex(Coords::DateTime::s_ISO8601_format);
#else
const std::regex Coords::DateTime::s_ISO8601_regex(Coords::DateTime::s_ISO8601_format);
#endif

const long int Coords::DateTime::s_gDateNRC(15+31L*(10+12L*1582));
const double Coords::DateTime::s_LilianDate(2299160.5);
const double Coords::DateTime::s_ModifiedJulianDate(2400000.5);
const double Coords::DateTime::s_TruncatedJulianDate(2440000.5);
const double Coords::DateTime::s_J2000(2451545.0);
const double Coords::DateTime::s_resolution(0.0001);

Coords::DateTime::DateTime(const std::string& an_iso8601_time)
  : m_year(1970),
  m_month(1),
  m_day(1),
  m_hour(0),
  m_minute(0),
  m_second(0),
  m_timezone(0)
{


#if BOOST_REGEX
  boost::smatch iso8601_match;
  if (!boost::regex_match(an_iso8601_time, iso8601_match, s_ISO8601_regex)) {
#else
  std::smatch iso8601_match;
  if (!std::regex_match(an_iso8601_time, iso8601_match, s_ISO8601_regex)) {
#endif
    std::stringstream emsg;
    emsg << an_iso8601_time
	 << " not in limited ISO-8601 format: year-mm-ddThh:mm:ss[.s*][Z|(+|-)hh[:mm]]";
    throw Coords::Error(emsg.str());
  }

#if DEBUG_REGEX
  for (int i = 0; i < 12; ++i)
    std::cout << "iso8601_match[" << i << "]" << iso8601_match[i] << std::endl;
#endif

  m_year = Coords::stoi(iso8601_match[2]);

  if (iso8601_match[1] == "-")
    m_year *= -1;

  m_month = Coords::stoi(iso8601_match[3]);
  m_day = Coords::stoi(iso8601_match[4]);

  if ((m_year % 4 == 0 && m_year % 100 != 0) || m_year % 400 == 0)
    m_is_leap_year = true;
  else
    m_is_leap_year = false;

  m_hour = Coords::stoi(iso8601_match[5]);
  m_minute = Coords::stoi(iso8601_match[6]);
  m_second = Coords::stod(iso8601_match[8]);

  m_timezone = TimeZone(iso8601_match[10]);

  isValid(an_iso8601_time);
}

Coords::DateTime::DateTime(const double& a_jdate)
  : m_year(1970),
  m_month(1),
  m_day(1),
  m_hour(0),
  m_minute(0),
  m_second(0),
  m_timezone(0)
{
  *this = this->fromJulianDate(a_jdate);

  std::stringstream emsg;
  emsg << "jdate: " << a_jdate;
  isValid(emsg.str());
}


void Coords::DateTime::throwError(const std::string& a_datetime, const std::string msg) {
  std::stringstream emsg;
  std::stringstream current_time;

  if (a_datetime == "")
    current_time << *this;
  else
    current_time << a_datetime;

  emsg << current_time.str() << ": " << msg;
  throw Coords::Error(emsg.str());
}

void Coords::DateTime::isValid(const std::string& an_iso8601_time) {

  if (m_month < 1 || m_month > 12)
    throwError(an_iso8601_time, "month out of range.");

  if (m_day < 1 || m_day > 31)
    throwError(an_iso8601_time, "day out of range.");

  if ((m_month == 9 || m_month == 4 || m_month == 6 || m_month == 11) && m_day > 30)
    throwError(an_iso8601_time, "Thirty days hath September, April, June and November");

  if (m_is_leap_year) {

    if (m_month == 2 && m_day > 29)
      throwError(an_iso8601_time, "Except for February all alone. It has 28, but 29 each _leap_ year.");

  } else {

    if (m_month == 2 && m_day > 28)
      throwError(an_iso8601_time, "Except for February all alone. It has _28_, but 29 each leap year.");

  }

  if (m_hour < 0 || m_hour > 24)
    throwError(an_iso8601_time, "hour out of range.");

  if (m_minute < 0 || m_minute > 60)
    throwError(an_iso8601_time, "minute out of range.");

  if (m_second < 0 || m_second > 60)
    throwError(an_iso8601_time, "second out of range.");

}

// ----- copy constructor -----

Coords::DateTime::DateTime(const Coords::DateTime& a) {
  m_year = a.m_year;
  m_month = a.m_month;
  m_day = a.m_day;
  m_hour = a.m_hour;
  m_minute = a.m_minute;
  m_second = a.m_second;
  m_is_leap_year = a.m_is_leap_year;
  m_timezone = a.m_timezone;
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
  m_is_leap_year = rhs.m_is_leap_year;
  m_timezone = rhs.m_timezone;
  return *this;
}

// ----- operators -----

Coords::DateTime& Coords::DateTime::operator+=(const double& rhs_days) {
  // TODO not efficient as expected of += too many copies.
  double jdate = this->toJulianDate();
  DateTime zdate = this->fromJulianDate(jdate + rhs_days); // TODO construct from jdate?

  *this = zdate.inTimeZone(m_timezone);

  return *this;

}


 Coords::DateTime& Coords::DateTime::operator-=(const double& rhs_days) {
  // TODO not efficient as expected of -= too many copies.
  double jdate = this->toJulianDate();
  DateTime zdate = this->fromJulianDate(jdate - rhs_days); // TODO construct from jdate?

  *this = zdate.inTimeZone(m_timezone);

  return *this;

}


Coords::DateTime Coords::operator+(const Coords::DateTime& lhs, const double& rhs) {
  Coords::DateTime temp(lhs);
  return temp += rhs;
}


Coords::DateTime Coords::operator+(const double& lhs, const Coords::DateTime& rhs) {
  return rhs + lhs; // commute
}


Coords::DateTime Coords::operator-(const Coords::DateTime& lhs, const double& rhs) {
  Coords::DateTime temp(lhs);
  return temp -= rhs;
}


Coords::DateTime Coords::operator-(const double& lhs, const Coords::DateTime& rhs) {
  return rhs - lhs; // commute
}


double Coords::operator-(const Coords::DateTime& lhs, const Coords::DateTime& rhs) {
  // returns difference in days
  return lhs.toJulianDate() - rhs.toJulianDate();
}


Coords::DateTime Coords::DateTime::inTimeZone(const Coords::TimeZone& a_new_timezone) const {

  if (a_new_timezone.offset() == m_timezone.offset())
    return Coords::DateTime(*this);

  double zulu_jdate(this->toJulianDate());
  DateTime z_datetime(this->fromJulianDate(zulu_jdate + a_new_timezone.offset()/24.0));

  Coords::DateTime new_datetime(z_datetime.year(),
				z_datetime.month(),
				z_datetime.day(),
				z_datetime.hour(),
				z_datetime.minute(),
				z_datetime.second(),
				a_new_timezone);

  return new_datetime;

}


// --------------------------
// ----- as Julian Date -----
// --------------------------


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

  double partial_day(Coords::degrees2seconds(m_hour, m_minute, m_second)/86400.0);

  return static_cast<double>(jdays) + partial_day - m_timezone.offset()/24.0;

}

// TODO static method?
Coords::DateTime Coords::DateTime::fromModifiedJulianDateAPC(const double& jdays) const {

  // Calculates Gregorian calendar date from Julian day number.
  // from Astronomy on the Personal Computer, Montenbruck and Pfleger, p. 15-16

  // ASSUMES: jdays are Modified Julian Days


  int a_year(0);
  int a_month(0);
  int a_day(0);
  int a_hour(0);
  int a_minute(0);
  double a_second(0);

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

  a_day = c - e - static_cast<int>(30.6001 * f);
  a_month = f - 1 - 12*(f/14);
  a_year = d - 4715 - ((7+a_month)/10);

  double d_hour = 24.0 * (jdays - floor(jdays));
  a_hour = d_hour; // implicit cast to int

  double d_minute = 60.0 * (d_hour - floor(d_hour));
  a_minute = d_minute; // implicit cast to int

  a_second = 60.0 * (d_minute - floor(d_minute));


  Coords::DateTime new_datetime(a_year, a_month, a_day, a_hour, a_minute, a_second);

  // TODO a_is_zulu = true;

  return new_datetime;

}


// -----------------------------
// ----- Numerical Recipes -----
// -----------------------------


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

  double partial_day(Coords::degrees2seconds(m_hour + m_timezone.offset(), m_minute, m_second)/86400.0);

  return static_cast<double>(jdays) + partial_day - m_timezone.offset()/24.0;

}


// TODO static method?
Coords::DateTime Coords::DateTime::fromJulianDateNRC(const double& jdays) const {

  // Calculates Gregorian calendar date from Julian day number.
  // From Numerical Recipes in C, pp. 14-15

  int a_year(0);
  int a_month(0);
  int a_day(0);
  int a_hour(0);
  int a_minute(0);
  double a_second(0);


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

  a_day = jb - jd - static_cast<long int>(30.6001*je);
  a_month = je - 1;

  if (a_month > 12)
    a_month -= 12;

  a_year = jc - 4715;

  if (a_month > 2)
    --a_year;

  if (a_year <= 0)
    --a_year;


  Coords::DateTime new_datetime(a_year, a_month, a_day, a_hour, a_minute, a_second);

  return new_datetime;

}


// ---------------------
// ----- Wikipedia -----
// ---------------------


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

  double partial_day(Coords::degrees2seconds(m_hour, m_minute, m_second)/86400.0);
  partial_day += m_timezone.offset()/24.0;

  return static_cast<double>(jdays) + partial_day - m_timezone.offset()/24.0;

}


// TODO static method?
Coords::DateTime Coords::DateTime::fromJulianDateWiki(const double& jdays) const {

  // from http://en.wikipedia.org/wiki/Julian_day

  // TODO: this does not correct for the Lilian date change. See unit tests.

  int a_year(0);
  int a_month(0);
  int a_day(0);
  int a_hour(0);
  int a_minute(0);
  double a_second(0);

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

  a_day = (h%s)/u + 1;

  a_month = (h/s + m) % n + 1;

  a_year = e/p - y + (n + m - a_month)/n;

  double d_hour = 24.0 * (jdays - floor(jdays));
  a_hour = d_hour; // implicit cast to int
  a_hour -= m_timezone.offset();

  double d_minute = 60.0 * (d_hour - floor(d_hour));
  a_minute = d_minute; // implicit cast to int

  a_second = 60.0 * (d_minute - floor(d_minute));


  Coords::DateTime new_datetime(a_year, a_month, a_day, a_hour, a_minute, a_second);

  return new_datetime;
}


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<
// <<<<< string utilities <<<<<
// <<<<<<<<<<<<<<<<<<<<<<<<<<<<


void Coords::TimeZone2String(const Coords::TimeZone& a_timezone, std::stringstream& a_string) {

  if (a_timezone.isZulu())
    a_string << "Z";

  if (a_timezone.offset() != 0) {

    int hours(a_timezone.offset());
    double minutes(0);

    if (a_timezone.offset() < 0) {

      minutes = -60.0 * (a_timezone.offset() - hours);

      a_string << "-";
      a_string << std::setw(2) << std::setfill('0') << -hours;

    } else {

      minutes = 60.0 * (a_timezone.offset() - hours);

      a_string << "+";
      a_string << std::setw(2) << std::setfill('0') << hours;

    }

    if (a_timezone.hasColon())
      a_string << ":";

    a_string << std::setw(2) << std::setfill('0') << fabs(minutes);

  }

}

void Coords::DateTime2String(const Coords::DateTime& a_datetime, std::stringstream& a_string) {

  int a_year(a_datetime.year());
  int a_month(a_datetime.month());
  int a_day(a_datetime.day());
  int a_hour(a_datetime.hour());
  int a_minute(a_datetime.minute());
  double a_second(a_datetime.second());

  // Round seconds (operator<<() output only) to cover rounding issues in calculation.

  if (fabs(a_second) < DateTime::s_resolution)
    a_second = 0.0;

  if (60 - a_second < DateTime::s_resolution && a_second > 0.0) {
    a_second = 0.0;
    a_minute += 1;
  }

  if (a_minute == 60) {
    a_minute = 0;
    a_hour += 1;
  }

  if (a_hour == 24) {
    a_hour = 0.0;
    a_day += 1;
  }

  a_string << a_year << "-"
	   << std::setw(2) << std::setfill('0') << a_month << "-"
	   << std::setw(2) << std::setfill('0') << a_day
	   << "T"
	   << std::setw(2) << std::setfill('0') << a_hour << ":"
	   << std::setw(2) << std::setfill('0') << a_minute << ":"
	   << std::setw(2) << std::setfill('0') << a_second
	   << a_datetime.timezone();

}
