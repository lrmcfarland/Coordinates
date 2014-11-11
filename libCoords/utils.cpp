// ================================================================
// Filename:    utils.cpp
// Description: Utilitiy funcitions for angles.
//
// Author:      L.R. McFarland, lrm@starbug.com
// Created:     2014 Jun 22
// Language:    C++
//
//  Coords is free software: you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Coords is distributed in the hope that it will be useful, but
//  WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Coords.  If not, see <http://www.gnu.org/licenses/>.
// ================================================================

#include <stdlib.h> // strtod

#include <utils.h>

double Coords::stod(const std::string& a_string) {
  // TODO stand-ins until c++ 11
  // doesn't catch syntax errors
  // but no worse than m_current_x = atof(m_current_characters.c_str());
  double a_double;

#if 0
  // angle_unittest.cpp:74: Failure
  // Value of: 0
  // Expected: f.x()
  // Which is: 2.1651736027593485e-314
  std::stringstream(a_string) >> a_double;
#else
 a_double = strtod(a_string.c_str(), NULL);
#endif

  return a_double;
}

int Coords::stoi(const std::string& a_string) {
  int an_int;
  std::stringstream(a_string) >> an_int;
  return an_int;
}

double Coords::degrees2seconds(const double& a_deg, const double& a_min, const double& a_sec) {
  // for angles and times with deg == hours Expects the minus sign to
  // be only once with the largest non-zero element.  All other
  // positions will ignore the sign. For example (0, -10, 30) ==
  // -10.5 minutes and (0, 10, -30) == 10.5 minutes.

  double seconds(0);
  double _min(a_min);
  double _sec(a_sec);

  if (a_deg > 0) {

    if (a_min < 0)
      _min *= -1;

    if (a_sec < 0)
      _sec *= -1;

    seconds = 3600*a_deg + 60*_min + _sec; // a_deg > 0 && a_min > 0

  } else if (a_deg == 0) {

    if (a_min > 0) {

      if (a_sec < 0)
	_sec *= -1;

      seconds = 60*_min + _sec; // a_deg == 0 && a_min > 0

    } else if (a_min == 0) {

      seconds = a_sec; // a_deg == 0 && a_min == 0

    } else {

      if (a_sec < 0)
	_sec *= -1;

      seconds = 60*_min - _sec; // a_deg == 0 && a_min < 0

    }

  } else {

    if (a_min < 0)
      _min *= -1;

    if (a_sec < 0)
      _sec *= -1;

    seconds = 3600*a_deg - 60*_min - _sec; // a_deg < 0 && a_min < 0
  }

  return seconds;

}
