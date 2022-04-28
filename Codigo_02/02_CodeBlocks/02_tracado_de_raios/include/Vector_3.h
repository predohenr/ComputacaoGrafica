#ifndef VECTOR_3_H
#define VECTOR_3_H

#include <iostream>

class Vector_3
{
public:
  // Construtores
  Vector_3(double x = 0.0, double y = 0.0, double z = 0.0);
  Vector_3(const Vector_3& v);

  // Acesso
  double x() const;
  double y() const;
  double z() const;

  // Operators
  Vector_3& operator=(const Vector_3& v);
  Vector_3 operator+(const Vector_3& v);
  Vector_3& operator/=(double t);
  Vector_3 operator/(double t) const;

  // Others
  double squared_length() const;
  Vector_3 unit_vector() const;

private:
  double _x, _y, _z;
};

Vector_3
operator*(const Vector_3& v, double t);

Vector_3
operator*(double t, const Vector_3& v);

Vector_3
operator-(const Vector_3& v);

// IO operators
std::ostream&
operator<<(std::ostream& os, const Vector_3& v);

#endif // VECTOR_3_H
