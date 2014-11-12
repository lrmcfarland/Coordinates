// ==================================================================
// Filename:    angle.cpp
//
// Description: This implements the angle class.
//
// Author:      L.R. McFarland
// Created:     2014 June 19
// Language:    C++
//
//  angle is free software: you can redistribute it and/or modify it
//  under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  angle is distributed in the hope that it will be useful, but
//  WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Orbits.  If not, see <http://www.gnu.org/licenses/>.
// ==================================================================

#include <angle.h>
#include <utils.h>

// =================
// ===== angle =====
// =================

// ----- constructors -----

Coords::angle::angle(const double& a_deg_or_hr,
		     const double& a_min,
		     const double& a_sec) {
  value(degrees2seconds(a_deg_or_hr, a_min, a_sec)/3600.0);
}

Coords::angle::angle(const std::string& a_deg_or_hr,
		     const std::string& a_min,
		     const std::string& a_sec) {
  value(angle(Coords::stod(a_deg_or_hr),
	      Coords::stod(a_min),
	      Coords::stod(a_sec)).value());
  // TODO bad string exception with C++11 stod
  // TODO delegating constructors in C++11
}


// copy constructor
Coords::angle::angle(const Coords::angle& a) {
  m_value = a.value();
}

// copy assignment
Coords::angle& Coords::angle::operator=(const Coords::angle& rhs) {
  if (this == &rhs) return *this;
  m_value = rhs.value();
  return *this;
}

// ----- bool operators -----

bool Coords::angle::operator== (const Coords::angle& rhs) const {
  return m_value == rhs.value();
}

bool Coords::angle::operator!= (const Coords::angle& rhs) const {
  return !operator==(rhs);
}

bool Coords::angle::operator< (const Coords::angle& rhs) const {
  return m_value < rhs.value();
}

bool Coords::angle::operator<= (const Coords::angle& rhs) const {
  return m_value <= rhs.value();
}

bool Coords::angle::operator> (const Coords::angle& rhs) const {
  return m_value > rhs.value();
}

bool Coords::angle::operator>= (const Coords::angle& rhs) const {
  return m_value >= rhs.value();
}

// ----- in-place operators -----

Coords::angle& Coords::angle::operator+=(const Coords::angle& rhs) {
  m_value += rhs.value();
  return *this;
}

Coords::angle& Coords::angle::operator-=(const Coords::angle& rhs) {
  m_value -= rhs.value();
  return *this;
}

Coords::angle& Coords::angle::operator*=(const Coords::angle& rhs) {
  m_value *= rhs.value();
  return *this;
}

Coords::angle& Coords::angle::operator/=(const Coords::angle& rhs) throw (DivideByZeroError) {
  if (rhs.value() == 0)
    throw DivideByZeroError();
  m_value /= rhs.value();
  return *this;
}

// ---------------------
// ----- operators -----
// ---------------------

Coords::angle Coords::operator+(const Coords::angle& lhs, const Coords::angle& rhs) {
  return Coords::angle(lhs.value() + rhs.value());
}

Coords::angle Coords::operator-(const Coords::angle& lhs, const Coords::angle& rhs) {
  return Coords::angle(lhs.value() - rhs.value());
}

Coords::angle Coords::operator-(const Coords::angle& rhs) {
  return Coords::angle(-rhs.value());
}

Coords::angle Coords::operator*(const Coords::angle& lhs, const Coords::angle& rhs) {
  return Coords::angle(lhs.value() * rhs.value());
}

Coords::angle Coords::operator/(const Coords::angle& lhs, const Coords::angle& rhs)
  throw (DivideByZeroError) {
  if (rhs.value() == 0)
    throw DivideByZeroError();
  return Coords::angle(lhs.value() / rhs.value());
}

// -------------------------
// ----- other methods -----
// -------------------------

// TODO RA?
// TODO this is not right
// >>> rad2deg(math.asin(math.sin(deg2rad(45))))
// 45.0
// >>> rad2deg(math.asin(math.sin(deg2rad(45+360))))
// -44.999999999999936
// >>> rad2deg(math.asin(math.sin(deg2rad(45-360))))
// -45.00000000000001
// >>> rad2deg(math.asin(math.sin(deg2rad(45+360*2))))
// 44.99999999999992
// >>> rad2deg(math.asin(math.sin(deg2rad(45+360*3))))
// -45.00000000000001
// >>> rad2deg(math.asin(math.sin(deg2rad(45+360*4))))
// 44.99999999999999
// TODO fix this angle(45 * 10).normalize() != 180
// >>> rad2deg(math.acos(math.cos(deg2rad(45+360*4))))
// 45.00000000000001
// >>> rad2deg(math.acos(math.cos(deg2rad(45+360*5))))
// 314.99999999999983

void Coords::angle::normalize() {
  // bring back into 0-360 range
  while (m_value > 360)
    m_value -= 360;
}

