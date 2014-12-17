// ================================================================
// Filename:    Cartesian.h
//
// Description: This defines a Cartesian coordinate class for physics
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

  class angle;
  class spherical;

  class Cartesian {
  public:

    // ----- unit vectors -----

    static const Cartesian Uo; // zero
    static const Cartesian Ux;
    static const Cartesian Uy;
    static const Cartesian Uz;

    // ----- ctor and dtor -----

    explicit Cartesian(const double& a = 0.0,
		       const double& b = 0.0,
		       const double& c = 0.0)
      : m_x(a), m_y(b), m_z(c) {}; // ctors, including default.

    explicit Cartesian(const std::string& a, // The ambiguity is in the box.
		       const std::string& b="0",
		       const std::string& c="0");

    explicit Cartesian(const spherical& a);

    ~Cartesian() {};

    Cartesian(const Cartesian& a);
    Cartesian& operator=(const Cartesian& rhs);

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

    bool operator==(const Cartesian& rhs) const;
    bool operator!=(const Cartesian& rhs) const;

    // ----- in-place operators -----

    Cartesian& operator+=(const Cartesian& rhs);
    Cartesian& operator-=(const Cartesian& rhs);

    Cartesian& operator*=(const double& rhs); // scale
    Cartesian& operator/=(const double& rhs) throw (DivideByZeroError);

    // ----- other methods -----

    void zero() {x(0.0); y(0.0); z(0.0);};

    double magnitude()  const;
    double magnitude2() const;

    Cartesian normalized() const throw (DivideByZeroError);

  private:

    // ----- data members -----

    double m_x, m_y, m_z;

  };


  // ---------------------
  // ----- operators -----
  // ---------------------

  Cartesian operator+(const Cartesian& lhs, const Cartesian& rhs);
  Cartesian operator-(const Cartesian& lhs, const Cartesian& rhs);
  Cartesian operator-(const Cartesian& rhs); // unary minus


  // explicit double cast to force scale and not dot product of default Cartesian ctor.
  Cartesian operator*(const Cartesian& lhs, const double& rhs); // scale
  Cartesian operator*(const double& lhs, const Cartesian& rhs); // scale

  Cartesian operator/(const Cartesian& lhs, const double& rhs) throw (DivideByZeroError); // scale
  Cartesian operator/(const double& lhs, const Cartesian& rhs) throw (DivideByZeroError); // scale

  // vector products
  double operator*(const Cartesian& lhs, const Cartesian& rhs); // dot product
  double dot(const Cartesian& a, const Cartesian& b);  // vector dot product
  Cartesian cross(const Cartesian& a, const Cartesian& b);  // vector cross product

  // -------------------------------
  // ----- output operator<<() -----
  // -------------------------------

  // inline for boost. Use hpp instead?
  inline std::ostream& operator<< (std::ostream& os, const Cartesian& a) {
    os << "<Cartesian>"
       << "<x>" << a.x() << "</x>"
       << "<y>" << a.y() << "</y>"
       << "<z>" << a.z() << "</z>"
       << "</Cartesian>";
    return os;
  }


  // -------------------------
  // ----- class rotator -----
  // -------------------------

  // supports rotating Cartesian vectors about Cartesian axies.

  class rotator {
  public:

    rotator(const Cartesian& an_axis); // ctor, no default
    ~rotator() {}; // dtor

    const Cartesian& axis() const              {return m_axis;}
    void         axis(const Cartesian& an_axis);

    Cartesian rotate(const Cartesian& a_heading, const angle& an_angle);

  private:

    rotator(const rotator& a); // do not implement
    rotator& operator=(const rotator& rhs); // do not implement

    Cartesian                          m_axis;
    std::vector< std::vector<double> > m_rotation_matrix;

    // for optimization
    bool  m_is_new_axis;
    angle m_current_angle;

  };


  // -----------------------------------
  // ----- class CartesianRecorder -----
  // -----------------------------------

  // implements a simple deque to store three Cartesian data.
  // It is intended for use to store and later plotting positions and
  // other three Cartesian data.

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
    const Cartesian& get(const unsigned int& idx) {return m_data[idx];}

    void push(Cartesian a);
    void clear() {m_data.clear();}

    void write2R(const std::string& flnm, bool skip_Uo=true);

  private:

    unsigned int       m_size_limit; /// size limit of position deque

    std::deque<Cartesian>  m_data;       /// data deque


  };

} // end namespace Coords
