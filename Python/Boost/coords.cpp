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

using namespace boost::python;


// overload wrappers

void (Coords::angle::*setangleValue)(const double&) = &Coords::angle::setValue;
void (Coords::angle::*setangleRadians)(const double&) = &Coords::angle::setRadians;



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

};
