// ================================================================
// Filename:    coords.i
//
// Description: This defines the SWIG wrappers for the coordinate
//              classes for physics applications.
//
// Author:      L.R. McFarland
// Created:     2016 May 15
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
// TODOs
// 1) wrappers for static consts Ux, Uy, Uz, Uo
// 2) make accessors look like properties, e.g. space a.x not a.x(), a.x = 4.0 not a.x(4.0)
// 3) mixed operators like space * double
//

%module coords
%{
#include "angle.h"
#include "Cartesian.h"
%}

namespace Coords {

  // =================
  // ===== angle =====
  // =================

  class angle {
  public:

    // constructors
    angle();
    angle(double x);
    angle(double x, double y);
    angle(double x, double y, double z);
    ~angle();

    // accessors
    void degrees(double a);
    double degrees();

    void setDegrees(double a);
    double getDegrees();

  };

  // =====================
  // ===== Cartesian =====
  // =====================


  class Cartesian {
  public:

    // TODO consts
    %immutable;
    Cartesian Uo; // TODO compiles but not working
    %mutable;

    // constructors
    Cartesian();
    Cartesian(double x);
    Cartesian(double x, double y);
    Cartesian(double x, double y, double z);
    ~Cartesian();

    // accessors
    void x(double a);
    double x();

    void y(double a);
    double y();

    void z(double a);
    double z();


    %extend {
      // from http://www.swig.org/Doc1.3/SWIGPlus.html#SWIGPlus_class_extension

      char* __str__() {
        static const int bsz(128);
        static char temp[bsz];
        // hardcoded precision
        snprintf(temp, bsz, "<Cartesian><x>%.12g</x><y>%.12g</y><z>%.12g</z></Cartesian>",
                 $self->x(), $self->y(), $self->z());
        return &temp[0];
      }

      char* __repr__() {
        static const int bsz(64);
        static char temp[bsz];
        // hardcoded precision
        snprintf(temp, bsz, "(%.12g, %.12g, %.12g)", $self->x(), $self->y(), $self->z());
        return &temp[0];
      }
    }

    // operators

    %rename(__eq__) operator==;
    %rename(__ne__) operator!=;

    %rename(__add__) operator+;
    %rename(__iadd__) operator+=;

    %rename(__sub__) operator-;
    %rename(__isub__) operator-=;

    // TODO multiply and divide by doubles

    // other methods
    double magnitude();
    double magnitude2();
    Cartesian normalized();

  };

  // functions
  Cartesian cross(const Cartesian& a, const Cartesian& b);
  double dot(const Cartesian& a, const Cartesian& b);

  // extensions

  %extend Cartesian {
    bool __eq__(const Cartesian& rhs) {
      Coords::Cartesian result;
      return *$self == rhs;
    }

    bool __ne__(const Cartesian& rhs) {
      Coords::Cartesian result;
      return *$self != rhs;
    }

    Cartesian __add__(const Cartesian& rhs) {
      Coords::Cartesian result;
      return *$self + rhs;
    }

    const Cartesian& __iadd__(const Cartesian& rhs) {
      *$self += rhs;
      return *$self;
    }

    Cartesian __sub__(const Cartesian& rhs) {
      Coords::Cartesian result;
      return *$self - rhs;
    }

    const Cartesian& __isub__(const Cartesian& rhs) {
      *$self -= rhs;
      return *$self;
    }



  }; // end extend Coords









} // end namespace coords

