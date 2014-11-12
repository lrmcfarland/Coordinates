// ============================================================
// Filename:    example1.cpp
//
// Description: Example of using libCoords
//
// Authors:     L.R. McFarland
// Created:     2014nov11
// ============================================================

#include <iostream>

#include <angle.h>
#include <Cartesian.h>
#include <spherical.h>

int main () {

    Coords::spherical a(1, Coords::angle(45), Coords::angle(45));
    Coords::spherical b(1, Coords::angle(45), Coords::angle(-45));
    Coords::spherical c;
    c = a + b;

    // a and b are both sin(45) high along z axis
    // +0.5 along x axis (1.0 total)
    // and +/- 0.5 along the y axis (0.0 total)

    Coords::Cartesian cart_c(c);

    std::cout << "a (" << a << ") + " << std::endl
	      << "b (" << b << ") = " << std::endl
	      << "c (" << cart_c << ")" << std::endl;

  return 0;
}
