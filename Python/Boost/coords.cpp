// ==========================================================================
// Filename:    coords.cpp
//
// Description: Contains the python wrappers for the coordinate objects
//              using boost and python.
//
// Author:      L.R. McFarland
// Created:     2014nov13
// ==========================================================================

#include <boost/python.hpp>

#include "angle.h"
#include "Cartesian.h"
#include "spherical.h"

using namespace boost::python;


// overload wrappers

void (Coords::angle::*setangleValue)(const double&) = &Coords::angle::setValue;
void (Coords::angle::*setangleRadians)(const double&) = &Coords::angle::setRadians;

void (Coords::Cartesian::*setx)(const double&) = &Coords::Cartesian::x;
void (Coords::Cartesian::*sety)(const double&) = &Coords::Cartesian::y;
void (Coords::Cartesian::*setz)(const double&) = &Coords::Cartesian::z;


BOOST_PYTHON_MODULE(coords) {


  class_<Coords::angle>("angle")

    .def("deg2rad", &Coords::angle::deg2rad)
    .staticmethod("deg2rad")

    .def("rad2deg", &Coords::angle::rad2deg)
    .staticmethod("rad2deg")

    // constructors
    .def(init<>()) // default
    .def(init<double>()) // degrees
    .def(init<double, double>()) // degrees, minutes
    .def(init<double, double, double>()) // degrees, minutes, seconds

    .def(init<Coords::angle>()) // copy

    // accessors

    .def("getValue", &Coords::angle::getValue)
    .def("setValue", setangleValue)
    .add_property("value", &Coords::angle::getValue, setangleValue)

    .def("getRadians", &Coords::angle::getRadians)
    .def("setRadians", setangleRadians)
    .add_property("radians", &Coords::angle::getRadians, setangleRadians)

    // TODO rich compare

    // operators
    .def(self + Coords::angle())
    .def(Coords::angle() + self)

    .def(self - Coords::angle())
    .def(Coords::angle() - self)

    .def(self * Coords::angle())
    .def(Coords::angle() * self)

    .def(self / Coords::angle())
    .def(Coords::angle() / self)


    // operator<<(), str not repr
    .def(self_ns::str(self_ns::self))

    ; // end of angle class_






  class_<Coords::Cartesian>("Cartesian")

    // static members
    .def_readonly("Uo", &Coords::Cartesian::Uo)
    .def_readonly("Ux", &Coords::Cartesian::Ux)
    .def_readonly("Uy", &Coords::Cartesian::Uy)
    .def_readonly("Uz", &Coords::Cartesian::Uz)

    // constructors
    .def(init<>()) // default
    .def(init<Coords::Cartesian>()) // copy

    .def(init<double>()) // x
    .def(init<double, double>()) // x, y
    .def(init<double, double, double>()) // x, y, z

    // accessors
    .def("getX", &Coords::Cartesian::getX)
    .def("setX", setx)
    .add_property("x", &Coords::Cartesian::getX, setx)

    .def("getY", &Coords::Cartesian::getY)
    .def("setY", sety)
    .add_property("y", &Coords::Cartesian::getY, sety)

    .def("getZ", &Coords::Cartesian::getZ)
    .def("setZ", setz)
    .add_property("z", &Coords::Cartesian::getZ, setz)

    // operator<<(), str not repr
    .def(self_ns::str(self_ns::self))

    // operators
    .def(self + Coords::Cartesian())
    .def(Coords::Cartesian() + self)

    .def(self - Coords::Cartesian())
    .def(Coords::Cartesian() - self)

    .def(self * Coords::Cartesian())
    .def(Coords::Cartesian() * self)

    .def(self / double()) // scale

    // other methods
    .def("magnitude", &Coords::Cartesian::magnitude)
    .def("normalized", &Coords::Cartesian::normalized)


    ; // end of class_

  // functions
  def("cross", Coords::cross);
  def("dot", Coords::dot);







};
