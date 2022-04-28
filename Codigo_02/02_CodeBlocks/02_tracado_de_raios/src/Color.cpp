#include "../include/Color.h"

Color::Color(double r, double g, double b)
    : _r(r), _g(g), _b(b)
{ }

double
Color::red()
{
  return _r;
}

double
Color::green()
{
  return _g;
}

double
Color::blue()
{
  return _b;
}

void
Color::set_red(double r)
{
  _r = r;
}

void
Color::set_green(double g)
{
  _g = g;
}

void
Color::set_blue(double b)
{
  _b = b;
}

std::ostream&
operator<<(std::ostream& os, Color& c)
{
  os << c.red() << ' ' << c.green() << ' ' << c.blue();
  return os;
}
