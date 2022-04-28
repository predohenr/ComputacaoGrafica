#include "../include/Vector_3.h"
#include <math.h>

// Constructors
Vector_3::Vector_3(double x, double y, double z)
  : _x(x), _y(y), _z(z) { }

Vector_3::Vector_3(const Vector_3& v)
  : _x(v.x()), _y(v.y()), _z(v.z()) { }

// Acesso
double
Vector_3::x() const
{
  return _x;
}

double
Vector_3::y() const
{
  return _y;
}

double
Vector_3::z() const
{
  return _z;
}

// Operadores
Vector_3&
Vector_3::operator=(const Vector_3& v)
{
  _x = v.x();
  _y = v.y();
  _z = v.z();
  return *this;
}

Vector_3
Vector_3::operator+(const Vector_3& v)
{
  return Vector_3(_x + v.x(), _y + v.y(), _z + v.z());
}

Vector_3&
Vector_3::operator/=(double t)
{
  _x /= t;
  _y /= t;
  _z /= t;
  return *this;
}

Vector_3
Vector_3::operator/(double t) const
{
  return Vector_3(x()/t, y()/t, z()/t);
}

double
Vector_3::squared_length() const
{
  return x()*x() + y()*y() + z()*z();
}

Vector_3
Vector_3::unit_vector() const{
    double n = sqrt(squared_length());
    return Vector_3(x()/n, y()/n, z()/n);
}

Vector_3
operator*(const Vector_3& v, double t)
{
  return Vector_3(t * v.x(), t * v.y(), t * v.z());
}

Vector_3
operator*(double t, const Vector_3& v)
{
  return Vector_3(t * v.x(), t * v.y(), t * v.z());
}

Vector_3
operator-(const Vector_3& v)
{
  return Vector_3(-v.x(), -v.y(), -v.z());
}

std::ostream&
operator<<(std::ostream& os, const Vector_3& v)
{
  os << v.x() << ' ' << v.y() << ' ' << v.z();
  return os;
}
