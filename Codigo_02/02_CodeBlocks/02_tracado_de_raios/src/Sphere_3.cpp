#include "../include/Sphere_3.h"
#include "../include/euclidean_constructions_3.h"

Sphere_3::Sphere_3()
  : _center(), _radius(0.0)
{ }

Sphere_3::Sphere_3(const Point_3& p, const double r)
  : _center(p), _radius(r)
{ }

double
Sphere_3::radius() const
{
  return _radius;
}

const Point_3&
Sphere_3::center() const
{
  return _center;
}

Sphere_3::Hit_pair
Sphere_3::hit(const Ray_3& r, double tmin, double tmax) const
{
    Vector_3 d = r.direction(), emc = (r.origin() - this->center());
    double delta, raiz_1, raiz_2, a, b, c;

    a = dot_product(d, d);
    b = dot_product(d, emc) * 2;
    c = dot_product(emc, emc) - pow(this->radius(), 2);

    delta = pow(b, 2) - (4*a*c);

    if (delta == 0){
        raiz_1 = (-b)/(2*a);
        return Hit_pair (this, raiz_1);
    }
    else if (delta>0){
        raiz_1 = (-b + sqrt(delta))/(2*a);
        raiz_2 = (-b - sqrt(delta))/(2*a);
        if (raiz_1 < raiz_2 && raiz_1>tmin && raiz_1<tmax)
            return Hit_pair (this, raiz_1);
        else if (raiz_1 > raiz_2 && raiz_2>tmin && raiz_2<tmax)
            return Hit_pair (this, raiz_2);
        else
            return Hit_pair (NULL, 1);
    }
    return Hit_pair (NULL, 1);
}

Vector_3
Sphere_3::normal(const Point_3& p) const
{
  return 2.0 * Vector_3(p - center());
}

Vector_3
Sphere_3::unit_normal(const Point_3& p) const
{
  Vector_3 n = Vector_3(p - center());
  return (n / radius());
}
