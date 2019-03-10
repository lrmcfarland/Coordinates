// ==================================================================
// Filename:    spherical.cpp
//
// Description: This implements a spherical coordinate class for physics
//              applications.
//
// Author:      L.R. McFarland
// Created:     2014 Nov 07
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

#include <angle.h>
#include <Cartesian.h>
#include <spherical.h>

// -----------------------
// ----- class space -----
// -----------------------


// ----- constructor from string for building from xml ----
Coords::spherical::spherical(const std::string& a,
			     const std::string& b,
			     const std::string& c)
  : m_r(0), m_theta(0), m_phi(0) {
  m_r     = Coords::stod(a);
  m_theta = Coords::angle(b);
  m_phi   = Coords::angle(c);
}

Coords::spherical::spherical(const Coords::Cartesian& a)
  : m_r(0), m_theta(0), m_phi(0) {
  r(a.magnitude());
  phi(Coords::angle(Coords::angle::rad2deg(atan2(a.y(), a.x()))));
  double r_xy(sqrt(a.x()*a.x() + a.y()*a.y()));
  theta(Coords::angle(Coords::angle::rad2deg(atan2(r_xy, a.z()))));
}

// ----- copy constructor -----
Coords::spherical::spherical(const Coords::spherical& a) {
  m_r     = a.r();
  m_theta = a.theta();
  m_phi   = a.phi();
};

// ----- copy assignment -----
Coords::spherical& Coords::spherical::operator=(const Coords::spherical& rhs) {
  if (this == &rhs) return *this;
  m_r     = rhs.r();
  m_theta = rhs.theta();
  m_phi   = rhs.phi();
  return *this;
}

// ----- bool operators -----

bool Coords::spherical::operator== (const Coords::spherical& rhs) const {
  return r() == rhs.r() && theta() == rhs.theta() && phi() == rhs.phi();
}

bool Coords::spherical::operator!= (const Coords::spherical& rhs) const {
  return !operator==(rhs);
}

// ----- in-place operators -----

Coords::spherical& Coords::spherical::operator+=(const Coords::spherical& rhs) {
  // meh. not very in-place.
  Coords::spherical sum(*this + rhs);
  m_r     = sum.r();
  m_theta = sum.theta();
  m_phi   = sum.phi();
  return *this;
}


Coords::spherical& Coords::spherical::operator-=(const Coords::spherical& rhs) {
  // meh. not very in-place.
  Coords::spherical diff(*this - rhs);
  m_r     = diff.r();
  m_theta = diff.theta();
  m_phi   = diff.phi();
  return *this;
}



// scale
Coords::spherical& Coords::spherical::operator*=(const double& rhs) {
  m_r *= rhs;
  return *this;
}

Coords::spherical& Coords::spherical::operator/=(const double& rhs) {
  if (rhs == 0)
    throw DivideByZeroError();
  m_r /= rhs;
  return *this;
}

// ---------------------
// ----- operators -----
// ---------------------

Coords::spherical Coords::operator+(const Coords::spherical& lhs,
				    const Coords::spherical& rhs) {
  Coords::Cartesian cart_lhs(lhs);
  Coords::Cartesian cart_rhs(rhs);
  Coords::Cartesian cart_sum(cart_lhs + cart_rhs);
  Coords::spherical sum(cart_sum);
  return sum;
}

Coords::spherical Coords::operator-(const Coords::spherical& lhs,
				    const Coords::spherical& rhs) {
  Coords::Cartesian cart_lhs(lhs);
  Coords::Cartesian cart_rhs(rhs);
  Coords::Cartesian cart_diff(cart_lhs - cart_rhs);
  Coords::spherical diff(cart_diff);
  return diff;
}

Coords::spherical Coords::operator-(const Coords::spherical& rhs) {
  return Coords::spherical(-rhs.r(),
			   -rhs.theta(),
			   -rhs.phi());
}

// scale
Coords::spherical Coords::operator*(const Coords::spherical& lhs,
				    const double& rhs) {
  return Coords::spherical(lhs.r() * rhs, lhs.theta(), lhs.phi());
}

Coords::spherical Coords::operator*(const double& lhs,
				    const Coords::spherical& rhs) {
  return Coords::operator*(rhs, lhs);
}

Coords::spherical Coords::operator/(const Coords::spherical& lhs,
				    const double& rhs) {
  if (rhs == 0)
    throw DivideByZeroError();
  return Coords::spherical(lhs.r() / rhs, lhs.theta(), lhs.phi());
}

Coords::spherical Coords::operator/(const double& lhs,
				    const Coords::spherical& rhs) {
  if (rhs.r() == 0)
    throw DivideByZeroError();
  return Coords::spherical(lhs / rhs.r(), rhs.theta(), rhs.phi());
}
