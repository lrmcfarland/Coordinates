// ================================================================
// Filename:    angle.h
//
// Description: This defines an angle class for use in physics
//              applications.
//
// Author:      L.R. McFarland
// Created:     2014 Nov 06
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
// ================================================================


#pragma once

#include <cmath>

#include <utils.h>




namespace Coords {

  // =================
  // ===== angle =====
  // =================

  class angle {
    // base class for latitude, longitude, declination and right ascension

  public:

    // angle unit convertors
    static double deg2rad(const double& deg) {return deg*M_PI/180.0;}
    static double rad2deg(const double& rad) {return rad*180.0/M_PI;}

    static double deg2RA(const double& deg)  {return deg*24.0/360.0;} // Right Ascension
    static double RA2deg(const double& ra)   {return ra*360.0/24.0;}

    // ----- ctor and dtor -----

    explicit angle(const double& a_deg = 0.0,
		   const double& a_min = 0.0,
		   const double& a_sec = 0.0);

    explicit angle(const std::string& a_deg, // The ambiguity is in the box.
		   const std::string& a_min = "0",
		   const std::string& a_sec = "0");

    virtual ~angle() {};

    angle(const angle& a);
    angle& operator=(const angle& rhs);

    // ----- accessors -----
    void          degrees(const double& a_degrees) {m_degrees = a_degrees;}
    const double& degrees() const                  {return m_degrees;}

    void          setDegrees(const double& a_degrees) {m_degrees = a_degrees;}
    double        getDegrees() const                  {return m_degrees;} // for boost

    void          radians(const double& a_degrees) {degrees(rad2deg(a_degrees));}
    double        radians() const                  {return deg2rad(degrees());}

    void          setRadians(const double& a_degrees) {degrees(rad2deg(a_degrees));}
    double        getRadians() const                  {return deg2rad(degrees());} // for boost

    void          RA(const double& a_degrees) {degrees(RA2deg(a_degrees));} // Right Ascension
    double        RA() const                  {return deg2RA(degrees());}

    void          setRA(const double& a_degrees) {degrees(RA2deg(a_degrees));}
    double        getRA() const                  {return deg2RA(degrees());} // for boost

    // ----- boolean operators -----

    bool operator== (const angle& rhs) const;
    bool operator!= (const angle& rhs) const;

    bool operator< (const angle& rhs) const;
    bool operator<= (const angle& rhs) const;

    bool operator> (const angle& rhs) const;
    bool operator>= (const angle& rhs) const;

    // ----- in-place operators -----

    angle& operator+=(const angle& rhs);
    angle& operator-=(const angle& rhs);

    angle& operator*=(const angle& rhs);
    angle& operator/=(const angle& rhs);


    // ----- other methods -----
    void normalize(const double& begin=0.0, const double& end=360);  // TODO normalized -> return a new copy?

    angle complement() const {return angle(90 - degrees());}

  private:

    double m_degrees;

  };


  // ---------------------
  // ----- operators -----
  // ---------------------

  angle operator+ (const angle& lhs, const angle& rhs);
  angle operator- (const angle& lhs, const angle& rhs);
  angle operator- (const angle& rhs); // unary minus

  angle operator* (const angle& lhs, const angle& rhs);
  angle operator/ (const angle& lhs, const angle& rhs);


  // -------------------------------
  // ----- output operator<<() -----
  // -------------------------------

  // inline for boost. Use hpp instead?
  inline std::ostream& operator<< (std::ostream& os, const Coords::angle& a) {
    std::stringstream out;
    Coords::degrees2HMSString(a.degrees(), out);
    return os << out.str();
  }


  // --------------------
  // ----- Latitude -----
  // --------------------

  // special case so spherical can convert to theta

  class Latitude : public angle {

  public:
    static const double g_north_pole;
    static const double g_south_pole;

    explicit Latitude(const double& a_deg = 0.0,
		      const double& a_min = 0.0,
		      const double& a_sec = 0.0);

    explicit Latitude(const std::string& a_deg,
		      const std::string& a_min = "0.0",
		      const std::string& a_sec = "0.0");

    virtual ~Latitude() {};

  };


  // -----------------------
  // ----- Declination -----
  // -----------------------

  // Boost doesn't like this as just a typedef of Latitude:
  // RuntimeWarning: to-Python converter for Coords::Latitude already
  // registered; second conversion method ignored. import coords


  class Declination : public angle {

  public:
    static const double g_north_pole;
    static const double g_south_pole;

    explicit Declination(const double& a_deg = 0.0,
			 const double& a_min = 0.0,
			 const double& a_sec = 0.0);

    explicit Declination(const std::string& a_deg,
			 const std::string& a_min = "0.0",
			 const std::string& a_sec = "0.0");

    virtual ~Declination() {};

  };



} // end namespace Coords
