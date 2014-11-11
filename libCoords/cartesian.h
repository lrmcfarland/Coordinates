// ================================================================
// Filename:    cartesian.h
//
// Description: This defines a cartesian coordinate class for physics
//              applications. Implemented as classic Cartesian three
//              space coordinates.
//
// Author:      L.R. McFarland
// Created:     12 May 2004
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

#include <cmath>
#include <deque>
#include <fstream>
#include <vector>

#include <utils.h>

namespace Coords {

  class Angle;
  class spherical;

  class cartesian {
  public:

    // ----- unit vectors -----

    static const cartesian Uo; // zero
    static const cartesian Ux;
    static const cartesian Uy;
    static const cartesian Uz;

    // ----- ctor and dtor -----

    explicit cartesian(const double& a = 0.0,
		       const double& b = 0.0,
		       const double& c = 0.0)
      : m_x(a), m_y(b), m_z(c) {}; // ctors, including default.

    explicit cartesian(const std::string& a, // The ambiguity is in the box.
		       const std::string& b="0",
		       const std::string& c="0");

    explicit cartesian(const spherical& a);
    
    ~cartesian() {};

    cartesian(const cartesian& a);
    cartesian& operator=(const cartesian& rhs);

    // ----- accessors -----

    void          x(const double& rhs) {m_x = rhs;}
    const double& x() const            {return m_x;}
    double        getX() const         {return m_x;} // for boost python wrappers

    void          y(const double& rhs) {m_y = rhs;}
    const double& y() const            {return m_y;}
    double        getY() const         {return m_y;} // for boost python wrappers

    void          z(const double& rhs) {m_z = rhs;}
    const double& z() const            {return m_z;}
    double        getZ() const         {return m_z;} // for boost python wrappers

    // ----- bool operators -----

    bool operator==(const cartesian& rhs) const;
    bool operator!=(const cartesian& rhs) const;

    // ----- in-place operators -----

    cartesian& operator+=(const cartesian& rhs);
    cartesian& operator-=(const cartesian& rhs);

    cartesian& operator*=(const double& rhs); // scale
    cartesian& operator/=(const double& rhs) throw (DivideByZeroError);

    // ----- other methods -----

    void zero() {x(0.0); y(0.0); z(0.0);};

    double magnitude()  const;
    double magnitude2() const;

    cartesian normalized() const throw (DivideByZeroError);

  private:

    // ----- data members -----

    double m_x, m_y, m_z;

  };


  // ---------------------
  // ----- operators -----
  // ---------------------

  cartesian operator+(const cartesian& lhs, const cartesian& rhs);
  cartesian operator-(const cartesian& lhs, const cartesian& rhs);
  cartesian operator-(const cartesian& rhs); // unitary minus


  // explicit double cast to force scale and not dot product of default cartesian ctor.
  cartesian operator*(const cartesian& lhs, const double& rhs); // scale
  cartesian operator*(const double& lhs, const cartesian& rhs); // scale

  cartesian operator/(const cartesian& lhs, const double& rhs) throw (DivideByZeroError); // scale
  cartesian operator/(const double& lhs, const cartesian& rhs) throw (DivideByZeroError); // scale

  // vector products
  double operator*(const cartesian& lhs, const cartesian& rhs); // dot product
  double dot(const cartesian& a, const cartesian& b);  // vector dot product
  cartesian cross(const cartesian& a, const cartesian& b);  // vector cross product

  // -------------------------------
  // ----- output operator<<() -----
  // -------------------------------

  // inline for boost. Use hpp instead?
  inline std::ostream& operator<< (std::ostream& os, const cartesian& a) {
    os << "<cartesian>"
       << "<x>" << a.x() << "</x>"
       << "<y>" << a.y() << "</y>"
       << "<z>" << a.z() << "</z>"
       << "</cartesian>";
    return os;
  }


  // -------------------------
  // ----- class rotator -----
  // -------------------------

  // supports rotating cartesian vectors about cartesian axies.

  class rotator {
  public:

    rotator(const cartesian& a_axis); // ctor, no default
    ~rotator() {}; // dtor

    rotator(const rotator& a); // copy ctor
    rotator& operator=(const rotator& rhs); // assignment ctor

    const cartesian& axis() const              {return m_axis;}
    void         axis(const cartesian& a_axis);

    cartesian rotate(const cartesian& a_heading, const double& a_radians); // TODO use angle

  private:

    cartesian                              m_axis;
    std::vector< std::vector<double> > m_rotation_matrix;

    // for optimization
    bool                               m_is_new_axis;
    double                             m_old_radians;

  };


  // -----------------------------------
  // ----- class CartesianRecorder -----
  // -----------------------------------

  // implements a simple deque to store three cartesian data.
  // It is intended for use to store and later plotting positions and
  // other three cartesian data.

  class CartesianRecorderIOError : public Error {
  public:
  CartesianRecorderIOError(const std::string& msg) : Error(msg) {}
  };


  class CartesianRecorder {

  public:

    static const unsigned int default_size; /// default size limit for deque

    CartesianRecorder(const unsigned int& a_size_limit=CartesianRecorder::default_size);
    ~CartesianRecorder() {}; // dtor

    CartesianRecorder(const CartesianRecorder& a);   // copy ctor
    CartesianRecorder& operator=(const CartesianRecorder& a); // copy assignment

    const unsigned int& sizeLimit() const       {return m_size_limit;}
    void                sizeLimit(const int& a) {m_size_limit = a;}

    unsigned long size() const            {return m_data.size();}
    const cartesian& get(const unsigned int& idx) {return m_data[idx];}

    void push(cartesian a);
    void clear() {m_data.clear();}

    void write2R(const std::string& flnm, bool skip_Uo=true);

  private:

    unsigned int       m_size_limit; /// size limit of position deque

    std::deque<cartesian>  m_data;       /// data deque


  };

} // end namespace Coords
