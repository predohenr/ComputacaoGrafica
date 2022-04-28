#ifndef COLOR_H
#define COLOR_H

#include <iostream>

class Color
{
public:
  // Constructors
  Color(double r = 0.0, double g = 0.0, double b = 0.0);
  Color(const Color& c);

  // Access
  double red();
  double green();
  double blue();

  // Modifiers
  void set_red(double r);
  void set_green(double g);
  void set_blue(double b);

private:
  double _r, _g, _b;
};

std::ostream&
operator<<(std::ostream& os, Color& c);

#endif // COLOR_H
