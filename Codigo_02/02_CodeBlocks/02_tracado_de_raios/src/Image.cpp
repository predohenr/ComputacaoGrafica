#include "../include/Image.h"

Image::Image()
{ }

Image::Image(int w, int h) : _w(w), _h(h)
{
  _pixels = new Color[w * h];
}

Image::~Image()
{
  if (_pixels) delete[] _pixels;
}

int
Image::width() const
{
  return _w;
}

int
Image::height() const
{
  return _h;
}

Color&
Image::pixel(int i, int j)
{
  return _pixels[i * width() + j];
}

void
Image::set_pixel(int i, int j, const Color& c)
{
  _pixels[i * width() + j] = c;
}

void
Image::write_ppm(std::ostream& os) const
{
  os << "P3\n";
  os << width() << ' ' << height() << std::endl;
  os << "255\n";

  for (int i = height() - 1; i >= 0; --i) {
    for (int j = 0; j < width(); ++j) {
      os << _pixels[i * width() + j] << ' ';
    }
    os << std::endl;
  }
}

void
Image::create_image(GLubyte *imgrtx) const
{
   for (int i = height() - 1; i >= 0; --i) {
    for (int j = 0, indc = 0; j < width(); j++, indc += 3) {
        imgrtx[3 * i * width() + indc] = _pixels[i * width() + j].red();
        imgrtx[3 * i * width() + indc + 1] = _pixels[i * width() + j].green();
        imgrtx[3 * i * width() + indc + 2] = _pixels[i * width() + j].blue();
    }
  }
}

