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
//  along with Coordinates. If not, see <http://www.gnu.org/licenses/>.
// ==================================================================

#include <angle.h>
#include <utils.h>

// =================
// ===== angle =====
// =================

// ----- constructors -----

Coords::angle::angle(const double& a_deg,
		     const double& a_min,
		     const double& a_sec) {
  degrees(degrees2seconds(a_deg, a_min, a_sec)/3600.0);
}

Coords::angle::angle(const std::string& a_deg,
		     const std::string& a_min,
		     const std::string& a_sec) {
  degrees(angle(Coords::stod(a_deg),
		Coords::stod(a_min),
		Coords::stod(a_sec)).degrees());
  // TODO bad string exception with C++11 stod
  // TODO delegating constructors in C++11
}


// copy constructor
Coords::angle::angle(const Coords::angle& a) {
  m_degrees = a.degrees();
}

// copy assignment
Coords::angle& Coords::angle::operator=(const Coords::angle& rhs) {
  if (this == &rhs) return *this;
  m_degrees = rhs.degrees();
  return *this;
}

// ----- bool operators -----

bool Coords::angle::operator== (const Coords::angle& rhs) const {
  return m_degrees == rhs.degrees();
}

bool Coords::angle::operator!= (const Coords::angle& rhs) const {
  return !operator==(rhs);
}

bool Coords::angle::operator< (const Coords::angle& rhs) const {
  return m_degrees < rhs.degrees();
}

bool Coords::angle::operator<= (const Coords::angle& rhs) const {
  return m_degrees <= rhs.degrees();
}

bool Coords::angle::operator> (const Coords::angle& rhs) const {
  return m_degrees > rhs.degrees();
}

bool Coords::angle::operator>= (const Coords::angle& rhs) const {
  return m_degrees >= rhs.degrees();
}

// ----- in-place operators -----

Coords::angle& Coords::angle::operator+=(const Coords::angle& rhs) {
  m_degrees += rhs.degrees();
  return *this;
}

Coords::angle& Coords::angle::operator-=(const Coords::angle& rhs) {
  m_degrees -= rhs.degrees();
  return *this;
}

Coords::angle& Coords::angle::operator*=(const Coords::angle& rhs) {
  m_degrees *= rhs.degrees();
  return *this;
}

Coords::angle& Coords::angle::operator/=(const Coords::angle& rhs) {
  if (rhs.degrees() == 0)
    throw DivideByZeroError();
  m_degrees /= rhs.degrees();
  return *this;
}

// ---------------------
// ----- operators -----
// ---------------------

Coords::angle Coords::operator+(const Coords::angle& lhs, const Coords::angle& rhs) {
  return Coords::angle(lhs.degrees() + rhs.degrees());
}

Coords::angle Coords::operator-(const Coords::angle& lhs, const Coords::angle& rhs) {
  return Coords::angle(lhs.degrees() - rhs.degrees());
}

Coords::angle Coords::operator-(const Coords::angle& rhs) {
  return Coords::angle(-rhs.degrees());
}

Coords::angle Coords::operator*(const Coords::angle& lhs, const Coords::angle& rhs) {
  return Coords::angle(lhs.degrees() * rhs.degrees());
}

Coords::angle Coords::operator/(const Coords::angle& lhs, const Coords::angle& rhs) {
  if (rhs.degrees() == 0)
    throw DivideByZeroError();
  return Coords::angle(lhs.degrees() / rhs.degrees());
}

// -------------------------
// ----- other methods -----
// -------------------------

void Coords::angle::normalize(const double& begin, const double& end) {
  // from http://stackoverflow.com/questions/1628386/normalise-orientation-between-0-and-360
  const double width(end - begin);
  const double offset(m_degrees - begin);
  m_degrees = (offset - (floor(offset/width) * width)) + begin;
}

// ====================
// ===== Latitude =====
// ====================

const double Coords::Latitude::g_north_pole(90);
const double Coords::Latitude::g_south_pole(-90);

// ----- constructors -----

Coords::Latitude::Latitude(const double& a_deg,
			   const double& a_min,
			   const double& a_sec)
  : angle(a_deg, a_min, a_sec) {

  if (degrees() > g_north_pole)
    throw Coords::Error("maximum exceeded");

  if (degrees() < g_south_pole)
    throw Coords::Error("minimum exceeded");
}

Coords::Latitude::Latitude(const std::string& a_deg,
			   const std::string& a_min,
			   const std::string& a_sec)
  : angle(a_deg, a_min, a_sec) {

  if (degrees() > g_north_pole)
    throw Coords::Error("maximum exceeded");

  if (degrees() < g_south_pole)
    throw Coords::Error("minimum exceeded");
}

// =======================
// ===== Declination =====
// =======================

const double Coords::Declination::g_north_pole(90);
const double Coords::Declination::g_south_pole(-90);

// ----- constructors -----

Coords::Declination::Declination(const double& a_deg,
				 const double& a_min,
				 const double& a_sec)
  : angle(a_deg, a_min, a_sec) {

  if (degrees() > g_north_pole)
    throw Coords::Error("maximum exceeded");

  if (degrees() < g_south_pole)
    throw Coords::Error("minimum exceeded");
}

Coords::Declination::Declination(const std::string& a_deg,
				 const std::string& a_min,
				 const std::string& a_sec)
  : angle(a_deg, a_min, a_sec) {

  if (degrees() > g_north_pole)
    throw Coords::Error("maximum exceeded");

  if (degrees() < g_south_pole)
    throw Coords::Error("minimum exceeded");
}
