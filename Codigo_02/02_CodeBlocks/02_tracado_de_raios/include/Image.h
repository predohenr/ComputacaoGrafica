#ifndef IMAGE_H
#define IMAGE_H

#include <vector>

#include "../include/Color.h"
#define GLEW_STATIC
#include <GL/glew.h>


class Image
{
public:
  Image();
  Image(int w, int h);
  ~Image();

  int width() const;
  int height() const;

  Color& pixel(int i, int j);
  void set_pixel(int i, int j, const Color& c);

  void write_ppm(std::ostream& os) const;

  void create_image(GLubyte *imgrtx) const;

private:
  int _w, _h;
  Color *_pixels;
};

#endif // IMAGE_H
