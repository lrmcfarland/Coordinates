// ================================================================
// Filename:    utils.h
//
// Description: Utilitiy funcitions for angles.
//
// Author:      L.R. McFarland, lrm@starbug.com
// Created:     2014 Jun 22
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
#include <stdexcept>

namespace Coords {

  // constants
  const double epsilon(1e-16); // machine epsilon for this build to set print precision.

  // exceptions
  class Error : public std::runtime_error {
  public:
  Error(const std::string& msg) : std::runtime_error(msg) {}
  };

  class DivideByZeroError : public Error {
  public:
  DivideByZeroError(const std::string& msg="division by zero is undefined") : Error(msg) {}
  };

  // converters

  double stod(const std::string& a_string);  // TODO stand-in until c++ 11
  int    stoi(const std::string& a_string);  // TODO stand-in until c++ 11

  double degrees2seconds(const double& a_deg, const double& a_min, const double& a_sec);

  // output operator<<
  void degrees2DMSString(const double& a_degrees, std::stringstream& a_string);
  void degrees2HMSString(const double& a_degrees, std::stringstream& a_string);

} // end namespace Coords
