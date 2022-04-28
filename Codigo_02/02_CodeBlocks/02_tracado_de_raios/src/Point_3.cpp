#include "../include/Point_3.h"

// Constructors
Point_3::Point_3(double x, double y, double z) : _x(x), _y(y), _z(z)
{ }

Point_3::Point_3(const Point_3& p) : _x(p.x()), _y(p.y()), _z(p.z())
{ }

// Acesso
double
Point_3::x() const
{
  return _x;
}

double
Point_3::y() const
{
  return _y;
}

double
Point_3::z() const
{
  return _z;
}

// Operadores
Point_3&
Point_3::operator=(const Point_3& p)
{
  _x = p.x();
  _y = p.y();
  _z = p.z();
  return *this;
}

Point_3&
Point_3::operator+=(const Vector_3& v)
{
  _x += v.x();
  _y += v.y();
  _z += v.z();
  return *this;
}

Point_3
Point_3::operator+(const Vector_3& v)
{
  return Point_3(x() + v.x(), y() + v.y(), z() + v.z());
}

Vector_3
Point_3::operator-(const Point_3& p) const
{
  return Vector_3(x() - p.x(), y() - p.y(), z() - p.z());
}

std::ostream&
operator<<(std::ostream& os, const Point_3& p)
{
  os << p.x() << ' ' << p.y() << ' ' << p.z();
  return os;
}
