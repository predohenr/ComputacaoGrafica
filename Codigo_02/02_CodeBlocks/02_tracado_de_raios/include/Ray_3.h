#ifndef RAY_3_H
#define RAY_3_H

#include "../include/Point_3.h"
#include "../include/Vector_3.h"

class Ray_3
{
public:
  // Construtores
  Ray_3(const Point_3& e = Point_3(), const Vector_3& d = Vector_3());
  Ray_3(const Ray_3& r);

  // Acesso
  const Point_3& origin() const;
  const Vector_3& direction() const;

private:
  Point_3 _origin;
  Vector_3 _direction;
};

// IO operators
std::ostream&
operator<<(std::ostream& os, const Ray_3& r);

#endif // RAY_3_H
