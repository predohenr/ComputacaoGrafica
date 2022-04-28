#ifndef SPHERE_3_H
#define SPHERE_3_H

#include <cmath>

#include "../include/Point_3.h"
#include "../include/Ray_3.h"
#include "../include/Vector_3.h"
#include "../include/Object.h"
#include "../include/euclidean_constructions_3.h"
#include "../include/number_utils.h"
#include "../include/Color.h"

class Sphere_3 : public Object
{
public:
  // Constructors
  Sphere_3();
  Sphere_3(const Point_3& p, const double r);

  double radius() const;
  const Point_3& center() const;

  Hit_pair hit(const Ray_3& r, double tmin = 0.0, double tmax = INF) const;
  Vector_3 normal(const Point_3& p) const;
  Vector_3 unit_normal(const Point_3& p) const;

private:
  Point_3 _center;
  double _radius;
};

#endif // SPHERE_3_H
