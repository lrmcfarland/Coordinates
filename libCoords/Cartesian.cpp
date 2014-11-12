// ==================================================================
// Filename:    Cartesian.cpp
//
// Description: Implements the Cartesian class This file is part of my
//              Orbits software library.
//
// Author:      L.R. McFarland
// Created:     4 Feb 1999
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
// ==================================================================

#include <stdlib.h> // strtod

#include <angle.h>
#include <Cartesian.h>
#include <spherical.h>
#include <utils.h>

// ---------------------------
// ----- class Cartesian -----
// ---------------------------

// ----- static data members -----

const Coords::Cartesian Coords::Cartesian::Uo(0,0,0);
const Coords::Cartesian Coords::Cartesian::Ux(1,0,0);
const Coords::Cartesian Coords::Cartesian::Uy(0,1,0);
const Coords::Cartesian Coords::Cartesian::Uz(0,0,1);

// ----- constructor from string for building from xml ----
Coords::Cartesian::Cartesian(const std::string& a,
			     const std::string& b,
			     const std::string& c)
  : m_x(0), m_y(0), m_z(0) {
  x(Coords::stod(a));
  y(Coords::stod(b));
  z(Coords::stod(c));
}

// ----- conversion constructor to build from spherical coords ----
Coords::Cartesian::Cartesian(const Coords::spherical& a)
  : m_x(0), m_y(0), m_z(0) {

  // ASSUMES: theta is the angle between z and r.
  z(a.r() * cos(a.theta().radians()));
  double r_xy(a.r() * sin(a.theta().radians())); // r projection in xy plane.

  // ASSUMES: phi is the angle between x and r projected into the xy plane.
  y(r_xy * sin(a.phi().radians()));
  x(r_xy * cos(a.phi().radians()));
}

// ----- copy constructor -----
Coords::Cartesian::Cartesian(const Coords::Cartesian& a) {
  x(a.x());
  y(a.y());
  z(a.z());
};

// ----- copy assignment -----
Coords::Cartesian& Coords::Cartesian::operator=(const Coords::Cartesian& rhs) {
  if (this == &rhs) return *this;
  x(rhs.x());
  y(rhs.y());
  z(rhs.z());
  return *this;
}

// ----- bool operators -----

bool Coords::Cartesian::operator== (const Coords::Cartesian& rhs) const {
  return x() == rhs.x() && y() == rhs.y() && z() == rhs.z();
}

bool Coords::Cartesian::operator!= (const Coords::Cartesian& rhs) const {
  return !operator==(rhs);
}

// ----- in-place operators -----

Coords::Cartesian& Coords::Cartesian::operator+=(const Coords::Cartesian& rhs) {
  m_x += rhs.x();
  m_y += rhs.y();
  m_z += rhs.z();
  return *this;
}

Coords::Cartesian& Coords::Cartesian::operator-=(const Coords::Cartesian& rhs) {
  m_x -= rhs.x();
  m_y -= rhs.y();
  m_z -= rhs.z();
  return *this;
}

Coords::Cartesian& Coords::Cartesian::operator*=(const double& rhs) {
  m_x *= rhs;
  m_y *= rhs;
  m_z *= rhs;
  return *this;
}

Coords::Cartesian& Coords::Cartesian::operator/=(const double& rhs) throw (DivideByZeroError) {
  if (rhs == 0)
    throw DivideByZeroError();
  m_x /= rhs;
  m_y /= rhs;
  m_z /= rhs;
  return *this;
}

// ----- normalizing -----

double Coords::Cartesian::magnitude() const {
  return sqrt(magnitude2());
}

double Coords::Cartesian::magnitude2() const {
  return m_x*m_x + m_y*m_y + m_z*m_z;
}

Coords::Cartesian Coords::Cartesian::normalized() const throw (DivideByZeroError) {
  const double h(magnitude());
  return Coords::Cartesian(m_x/h, m_y/h, m_z/h);
}

// ---------------------
// ----- operators -----
// ---------------------

Coords::Cartesian Coords::operator+(const Coords::Cartesian& lhs,
				    const Coords::Cartesian& rhs) {
  return Coords::Cartesian(lhs.x() + rhs.x(),
			   lhs.y() + rhs.y(),
			   lhs.z() + rhs.z());
}

Coords::Cartesian Coords::operator-(const Coords::Cartesian& lhs,
				    const Coords::Cartesian& rhs) {
  return Coords::Cartesian(lhs.x() - rhs.x(),
			   lhs.y() - rhs.y(),
			   lhs.z() - rhs.z());
}

Coords::Cartesian Coords::operator-(const Coords::Cartesian& rhs) {
  return Coords::Cartesian(-rhs.x(),
			   -rhs.y(),
			   -rhs.z());
}

// scale
Coords::Cartesian Coords::operator*(const Coords::Cartesian& lhs,
				    const double& rhs) {
  return Coords::Cartesian(lhs.x() * rhs, lhs.y() * rhs, lhs.z() * rhs);
}

Coords::Cartesian Coords::operator*(const double& lhs,
				    const Coords::Cartesian& rhs) {
  return Coords::operator*(rhs, lhs);
}

Coords::Cartesian Coords::operator/(const Coords::Cartesian& lhs,
				    const double& rhs)
  throw (DivideByZeroError) {
  if (rhs == 0)
    throw DivideByZeroError();
  return Coords::Cartesian(lhs.x() / rhs, lhs.y() / rhs, lhs.z() / rhs);
}

Coords::Cartesian Coords::operator/(const double& lhs,
				    const Coords::Cartesian& rhs)
  throw (DivideByZeroError) {
  if (rhs.x() == 0 || rhs.y() == 0 || rhs.z() == 0)
    throw DivideByZeroError();
  return Coords::Cartesian(lhs / rhs.x(), lhs / rhs.y(), lhs / rhs.z());
}

// ----- vector products -----

double Coords::operator*(const Coords::Cartesian& lhs,
			 const Coords::Cartesian& rhs) {
  return lhs.x()*rhs.x() + lhs.y()*rhs.y() + lhs.z()*rhs.z();
}

double Coords::dot(const Coords::Cartesian& lhs,
		   const Coords::Cartesian& rhs) {
  return lhs * rhs;
}

Coords::Cartesian Coords::cross(const Coords::Cartesian& a,
				const Coords::Cartesian& b) {
  Coords::Cartesian tmp;
  tmp.x(a.y()*b.z() - a.z()*b.y());
  tmp.y(a.z()*b.x() - a.x()*b.z());
  tmp.z(a.x()*b.y() - a.y()*b.x());
  return tmp;
}

// -------------------------
// ----- class rotator -----
// -------------------------

Coords::rotator::rotator(const Coords::Cartesian& a_axis) :
  m_axis(a_axis),
  m_rotation_matrix(3, std::vector<double>(3, 0)),
  m_is_new_axis(true),
  m_old_radians(0.0)
{}


Coords::rotator::rotator(const Coords::rotator& rhs) :
  m_axis(rhs.axis()),
  m_rotation_matrix(rhs.m_rotation_matrix),
  m_is_new_axis(rhs.m_is_new_axis)
{} // TBD test this

Coords::rotator&
Coords::rotator::operator=(const Coords::rotator& rhs) {
  if (this == &rhs) return *this;
  axis(rhs.axis());
  m_rotation_matrix = rhs.m_rotation_matrix;
  m_is_new_axis = rhs.m_is_new_axis;
  return *this;
} // TBD test this



void Coords::rotator::axis(const Coords::Cartesian& a_axis) {
  if (a_axis != m_axis) {
    m_axis = a_axis;
    m_is_new_axis = true;
  }
}

Coords::Cartesian Coords::rotator::rotate(const Coords::Cartesian& a_heading,
					  const double& a_radians) {

  if (m_is_new_axis || m_old_radians != a_radians) {

    double c(cos(a_radians));
    double s(sin(a_radians));

    Coords::Cartesian normal(axis().normalized());

    double t(1-c);

    m_rotation_matrix[0][0] = c + normal.x()*normal.x()*t;
    m_rotation_matrix[1][1] = c + normal.y()*normal.y()*t;
    m_rotation_matrix[2][2] = c + normal.z()*normal.z()*t;

    double t1(normal.x()*normal.y()*t);
    double t2(normal.z()*s);

    m_rotation_matrix[1][0] = t1 + t2;
    m_rotation_matrix[0][1] = t1 - t2;

    t1 = normal.x()*normal.z()*t;
    t2 = normal.y()*s;

    m_rotation_matrix[2][0] = t1 - t2;
    m_rotation_matrix[0][2] = t1 + t2;

    t1 = normal.y()*normal.z()*t;
    t2 = normal.x()*s;

    m_rotation_matrix[2][1] = t1 + t2;
    m_rotation_matrix[1][2] = t1 - t2;

    m_is_new_axis = false;
    m_old_radians = a_radians;

  }

  Coords::Cartesian tmp(m_rotation_matrix[0][0]*a_heading.x() +
			m_rotation_matrix[0][1]*a_heading.y() +
			m_rotation_matrix[0][2]*a_heading.z(),
			m_rotation_matrix[1][0]*a_heading.x() +
			m_rotation_matrix[1][1]*a_heading.y() +
			m_rotation_matrix[1][2]*a_heading.z(),
			m_rotation_matrix[2][0]*a_heading.x() +
			m_rotation_matrix[2][1]*a_heading.y() +
			m_rotation_matrix[2][2]*a_heading.z());

  return tmp;

}


// =============================
// ===== CartesianRecorder =====
// =============================

const unsigned int Coords::CartesianRecorder::default_size(1024);

Coords::CartesianRecorder::CartesianRecorder(const unsigned int& a_size_limit) :
  m_size_limit(a_size_limit),
  m_data(std::deque<Coords::Cartesian>(m_size_limit))
{}

Coords::CartesianRecorder::CartesianRecorder(const Coords::CartesianRecorder& a):
  m_size_limit(a.sizeLimit()),
  m_data(a.m_data)
{}

Coords::CartesianRecorder&
Coords::CartesianRecorder::operator=(const Coords::CartesianRecorder& rhs) {
  if (this == &rhs) return *this;
  sizeLimit(rhs.sizeLimit());
  m_data = rhs.m_data;
  return *this;
}

void Coords::CartesianRecorder::push(Coords::Cartesian a) {
  // even up the sizes to just under the limit.
  while (m_data.size() > sizeLimit() - 1)
    m_data.pop_front();
  m_data.push_back(a);
}

// output compatible for R frames <- read.table(flnm)
void Coords::CartesianRecorder::write2R(const std::string& flnm, bool skip_Uo) {

  std::ofstream ssfile(flnm.c_str());

  if (!ssfile.is_open()) {
    std::stringstream err;
    err << "Error: unable to open file \"" << flnm << "\"";
    throw Coords::CartesianRecorderIOError(err.str());
  }

  ssfile << "# Formated for R frames <- read.table(" << flnm << ")"
	 << std::endl;
  ssfile << "x y z" << std::endl;

  for (unsigned int k = 0; k < m_data.size(); ++k) {

    // skip zero points from partially filled buffer.
    if (skip_Uo and m_data[k] == Coords::Cartesian::Uo)
      continue;

    ssfile << k << " "
	   << m_data[k].x() << " "
	   << m_data[k].y() << " "
	   << m_data[k].z() << std::endl;
  }

  ssfile.close();

}
