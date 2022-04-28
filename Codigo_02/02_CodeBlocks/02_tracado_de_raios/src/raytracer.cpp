#include <iostream>
#include <vector>
#include <fstream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../include/Sphere_3.h"
#include "../include/Ray_3.h"
#include "../include/Color.h"
#include "../include/Image.h"

using namespace std;

std::string program_name;


int maxval;
GLsizei width, height;
GLubyte *imgrtx;

Object::Hit_pair
hit(const Ray_3& r, std::vector<Object*>& object,
    double tmin, double tmax)
{
  typedef Object::Hit_pair Hit_pair;

  Hit_pair closest(NULL, tmax);
  for (int i = 0; i < object.size(); ++i) {
    Hit_pair res = object[i]->hit(r, tmin, closest.second);
    if (res.first != NULL) { // hit found
      closest = res;
    }
  }

  return closest;
}

double maxvalue(double a, double b){
    if (a >= b)
        return a;
    else
        return b;
}

double clamp(double a){
    if (a > 255)
        return 255.0;
    else if (a < 0)
        return 0.0;
    else
        return a;
}

Color trace(const Ray_3& r, std::vector<Object*>& object,
    double tmin = 0.0, double tmax = INF)
{
  Object::Hit_pair res = hit(r, object, tmin, tmax);
  if (res.first != NULL) {

        double Id = 100, rd = 0.5, Ia = 100, ra = 0.3, Ie = 100, re = 1.0, ns = 1.0, cos, cos2;
        Point_3 p = find_point(r.direction(), r.origin(), res.second);
        Point_3 c = Point_3(0.5,0.5,-1.0);
        Vector_3 n = res.first->unit_normal(p);
        Vector_3 l = -r.direction();
        Vector_3 d = (2 * dot_product(l, n) * n);
        Vector_3 r = Vector_3(d.x()-l.x(), d.y()-l.y(), d.z()-l.z());
        Vector_3 v = Vector_3(0, 1, 1);
        cos = maxvalue(cos0(l, n), 0.0);
        cos2 = maxvalue(cos0(r, v), 0.0);
        return Color(clamp((Id*rd*cos + Ia*ra + Ie*re*pow(cos2, ns)) * 1.0),
                     clamp((Id*rd*cos + Ia*ra + Ie*re*pow(cos2, ns)) * 1.0) ,
                     clamp((Id*rd*cos + Ia*ra + Ie*re*pow(cos2, ns)) * 1.0));

        //Colorindo apartir da Normal
        /*Point_3 p = find_point(r.direction(), r.origin(), res.second);
        Point_3 c = (0.5, 0.5, -1.0);
        Vector_3 N = (p-c).unit_vector();
        return Color(127*(N.x()+1), 127*(N.y()+1), 127*(N.z()+1));*/

        //Colorindo apartir de uma cor sólida
        //return Color(255, 255, 255);
  } else{
        //Fundo degrade de azul
        /*double fn = sqrt(r.direction().squared_length());
        double f = 0.5*((r.direction().y()/fn) + 1.0);
        double cor[3] = {(1.0-f)*255 + f*127,
                         ((1.0-f)*255 + f*178.5),
                         (1.0-f)*255 + f*255};
        return Color(cor[0], cor[1], cor[2]);*/

        //Fundo Preto
        return Color(0, 0, 0);
  }
}

Ray_3 construct_ray(int nx, int ny, int i, int j)
{
  // Configurações de câmera
  Vector_3 u(1, 0, 0);
  Vector_3 v(0, 1, 0);
  Vector_3 w(0, 0, 1);
  Point_3  e(0, 0, 1);
  double t = 1.0;
  double r = 1.0;
  double l = -r, b = -t;

  double lambda_u = l + (r - l)*(i + 0.5)/nx;
  double lambda_v = b + (t - b)*(j + 0.5)/ny;

  Point_3 origin;
  Vector_3 direction;
  origin = e + (lambda_u * u + lambda_v * v);
  direction = -w;

  return Ray_3(origin, direction);
}

int main(int argc, char *argv[])
{
  //Object::Hit_pair res;
  std::vector<Object*> object;
  Image image(400, 400);

  // Cena contém apenas uma esfera

  object.push_back(new Sphere_3(Point_3(0.5,-0.5,-1.0), 0.6));
  object.push_back(new Sphere_3(Point_3(0.5,0.5,-1.0), 0.6));
  // object.push_back(new Sphere_3(Point_3(0.1,0.0,-1.0), 0.6));
  

  for (int i = 0; i < image.height(); ++i) {
    for (int j = 0; j < image.width(); ++j) {
      Ray_3 r = construct_ray(image.width(), image.height(), i, j);
      Color c = trace(r, object);
      image.set_pixel(i, j, c);
    }
  }

  // Carregando a imagem
  width = GLint(image.width());
  height = GLint(image.height());
  imgrtx = new GLubyte[3 * width * height];
  image.create_image(imgrtx);

  // Criando a Janela
  GLFWwindow* window;

  if (!glfwInit())
        return -1;

  window = glfwCreateWindow(width, height, "Image Viewer", NULL, NULL);

  if (!window){
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  // Loop da janela
  while (!glfwWindowShouldClose(window)){
    glClear(GL_COLOR_BUFFER_BIT);

    glRasterPos2i(-1, -1);
    glDrawBuffer(GL_BACK);
    glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, imgrtx);

    glfwSwapBuffers(window);

    glfwPollEvents();
  }

  glfwTerminate();


  std::ofstream file("image.ppm");
  image.write_ppm(file);
  file.close();

  return 0;
}
