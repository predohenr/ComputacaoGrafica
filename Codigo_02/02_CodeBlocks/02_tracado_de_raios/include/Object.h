#ifndef OBJECT_H
#define OBJECT_H

#include "../include/Point_3.h"
#include "../include/Ray_3.h"
#include "../include/Vector_3.h"
#include "../include/Color.h"
#include "../include/number_utils.h"

class Object
{
public:
  typedef std::pair<const Object*,double> Hit_pair;

  virtual Hit_pair hit(const Ray_3& r, double tmin = 0.0, double tmax = INF) const
  { return Hit_pair(NULL,0); };

  virtual Vector_3 normal(const Point_3& p) const
  { return Vector_3(p - Point_3(0,0,0)); };

  virtual Vector_3 unit_normal(const Point_3& p) const
    { return Vector_3(p - Point_3(1,0,0)); };
};

#endif // OBJECT_H
