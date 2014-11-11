// ================================================================
// Filename:    spherical.h
//
// Description: This defines a spherical coordinate class for physics
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

#pragma once

#include <sstream>

namespace Coords {

  class Angle;
  class cartesian;

  class spherical {
  public:

    // ----- ctor and dtor -----

    explicit spherical(const double& r = 0.0,
		       const Angle& theta = Angle(0.0),
		       const Angle& phi = Angle(0.0))
      : m_r(r), m_theta(theta), m_phi(phi) {}; // ctors, including default.

    explicit spherical(const std::string& r, // The ambiguity is in the box.
		       const std::string& theta="0",
		       const std::string& phi="0");

    explicit spherical(const cartesian& a);

    ~spherical() {};

    spherical(const spherical& a);
    spherical& operator=(const spherical& rhs);

    // ----- accessors -----

    void          r(const double& rhs) {m_r = rhs;}
    const double& r() const            {return m_r;}
    double        getR() const         {return m_r;} // for boost python wrappers

    void          theta(const Angle& rhs)  {m_theta = rhs;}
    const Angle&  theta() const            {return m_theta;}
    Angle         getTheta() const         {return m_theta;} // for boost python wrappers

    void          phi(const Angle& rhs)  {m_phi = rhs;}
    const Angle&  phi() const            {return m_phi;}
    Angle         getPhi() const         {return m_phi;} // for boost python wrappers

    // ----- bool operators -----

    bool operator==(const spherical& rhs) const;
    bool operator!=(const spherical& rhs) const;

    // ----- in-place operators -----

    spherical& operator+=(const spherical& rhs);
    spherical& operator-=(const spherical& rhs);

    spherical& operator*=(const double& rhs); // scale
    spherical& operator/=(const double& rhs) throw (DivideByZeroError);

    // ----- other methods -----

    void zero() {r(0.0); theta(Angle(0.0)); phi(Angle(0.0));};

  private:

    // ----- data members -----

    double m_r;     // Radius: Euclidean distance from the origin (O) to point (P).
    Angle  m_theta; // Inclination: The angle between the zenith direction and the line segment OP
    Angle  m_phi;   // Azimuth: The angle between the x-axis and the projection of OP into the xy plane.

  };

  // ---------------------
  // ----- operators -----
  // ---------------------

  spherical operator+(const spherical& lhs, const spherical& rhs);
  spherical operator-(const spherical& lhs, const spherical& rhs);
  spherical operator-(const spherical& rhs); // unitary minus


  // explicit double cast to force scale and not dot product of default spherical ctor.
  spherical operator*(const spherical& lhs, const double& rhs); // scale
  spherical operator*(const double& lhs, const spherical& rhs); // scale

  spherical operator/(const spherical& lhs, const double& rhs) throw (DivideByZeroError); // scale
  spherical operator/(const double& lhs, const spherical& rhs) throw (DivideByZeroError); // scale

  // vector products
  double operator*(const cartesian& lhs, const cartesian& rhs); // dot product
  double dot(const cartesian& a, const cartesian& b);  // vector dot product
  cartesian cross(const cartesian& a, const cartesian& b);  // vector cross product

  // -------------------------------
  // ----- output operator<<() -----
  // -------------------------------

  // inline for boost. Use hpp instead?
  inline std::ostream& operator<< (std::ostream& os, const Coords::spherical& a) {
    os << "<spherical>"
       << "<r>" << a.r() << "</r>"
       << "<theta>" << a.theta().value() << "</theta>"
       << "<phi>" << a.phi().value() << "</phi>"
       << "</spherical>";
    return os;
  }


} // end namespace Coords

