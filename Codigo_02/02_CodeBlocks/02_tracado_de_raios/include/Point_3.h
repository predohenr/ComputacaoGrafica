#ifndef POINT_3_H
#define POINT_3_H

#include <iostream>

#include "../include/Vector_3.h"

class Point_3
{
public:
  // Constructors
  Point_3(double x = 0.0, double y = 0.0, double z = 0.0);
  Point_3(const Point_3& p);

  // Access
  double x() const;
  double y() const;
  double z() const;

  // Operators
  Point_3& operator=(const Point_3& p);
  Point_3& operator+=(const Vector_3& v);
  Point_3 operator+(const Vector_3& v);
  Vector_3 operator-(const Point_3& p) const;

private:
  double _x, _y, _z;
};

// IO operators
std::ostream&
operator<<(std::ostream& os, const Point_3& p);

#endif // POINT_3_H
