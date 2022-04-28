#include "../include/Ray_3.h"

// Construtores
Ray_3::Ray_3(const Point_3& p, const Vector_3& v)
  : _origin(p), _direction(v) { }

Ray_3::Ray_3(const Ray_3& r)
  : _origin(r.origin()), _direction(r.direction()) { }

// Acesso
const Point_3&
Ray_3::origin() const
{
  return _origin;
}

const Vector_3&
Ray_3::direction() const
{
  return _direction;
}

std::ostream&
operator<<(std::ostream& os, const Ray_3& r)
{
  os << r.origin() << ' ' << r.direction();
  return os;
}
