#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

std::string program_name;

int maxval;
GLsizei width, height;
GLubyte *image;
GLubyte *gimage;
GLubyte *dimage;

void msg(const char *message)
{
  std::cerr << program_name << ": " << message << ".\n";
}

void read_pgm_file(std::istream& is)
{
  std::string line;
  // read magic number and check if it is 'P2'
  std::getline(is, line);
  if (line.compare("P2") != 0) {
    msg("wrong input format");
    std::exit(EXIT_FAILURE);
  }

  // skip comments
  do {
    line.clear();
    std::getline(is, line);
  } while (line.compare(0, 1, "#") == 0);

  std::istringstream isline(line);
  isline >> width >> height;
  is >> maxval;
  image = new GLubyte[width * height];

  unsigned int t;
  GLubyte c;
  for (int j = height - 1; j >= 0; --j) {
    for (int i = 0; i < width; ++i) {
      is >> t;
      c = t;
      image[j*width + i] = c;
    }
  }
}

int clamp(int c, int min, int max)
{
  if (c < min) c = min;
  if (c > max) c = max;
  return c;
}

void rgb_to_grayscale()
{
  gimage = new GLubyte[width * height];

  int r, g, b;
  for (int j = height - 1; j >= 0; --j) {
    for (int i = 0; i < 3*width; i += 3) {
      r = image[3*j*width + i];
      g = image[3*j*width + i + 1];
      b = image[3*j*width + i + 2];
      gimage[j*width + i/3] = clamp(0.299*r + 0.587*g + 0.114*b, 0, 255);
    }
  }
}

void read_ppm_file(std::istream& is)
{
  std::string line;
  // read magic number and check if it is 'P2'
  std::getline(is, line);
  if (line.compare("P3") != 0) {
    msg("wrong input format");
    std::exit(EXIT_FAILURE);
  }

  // skip comments
  do {
    line.clear();
    std::getline(is, line);
  } while (line.compare(0, 1, "#") == 0);

  std::istringstream isline(line);
  isline >> width >> height;
  is >> maxval;
  image = new GLubyte[3 * width * height];

  unsigned int R, G, B;
  GLubyte r, g, b;
  std::cout << "Resolucao espacial: " << width << " x " << height << std::endl;
  std::cout << "Intensidade máxima: " << maxval << std::endl;
  for (int j = height - 1; j >= 0; --j) {
    for (int i = 0; i < 3*width; i += 3) {
      is >> R;
      is >> G;
      is >> B;
      r = R;
      g = G;
      b = B;
      image[3*j*width + i] = r;
      image[3*j*width + i + 1] = g;
      image[3*j*width + i + 2] = b;
    }
  }
}

void usage(bool status)
{
  if (status) {
    std::cout << "Usage: " << program_name << " -[cl] K INPUT OUTPUT\n";
    std::exit(EXIT_SUCCESS);
  } else {
    std::cerr << "Try `" << program_name << " -h' for more information\n";
    std::exit(EXIT_FAILURE);
  }
}

// Contornos de quantizaçao
void dith00()
{
    dimage = new GLubyte[width * height];
    for (int l = 0; l < height; l++){
        for (int c = 0; c < width; c++){
            if (image[l + c*width] < 127)
                dimage[l + c*width] = 0;
            else
                dimage[l + c*width] = 255;
        }
    }
}

//  Modulação aleatória
void dith01()
{
    dimage = new GLubyte[width * height];
    unsigned seed = time(0);
    srand(seed);
    for (int l = 0; l < height; l++){
        for (int c = 0; c < width; c++){
            int random = rand()%127;
            int negative = rand()%2;
            if (negative)
                random * -1;
            if (image[l + c*width] + random < 127)
                dimage[l + c*width] = 0;
            else
                dimage[l + c*width] = 255;
        }
    }
}

// Ordenado Periódico (Com aglomeração)
void dith02()
{
    dimage = new GLubyte[width * height];
    float x, y;
    int m[6][6];
    m[0][0] = 34;
    m[0][1] = 29;
    m[0][2] = 17;
    m[0][3] = 21;
    m[0][4] = 30;
    m[0][5] = 35;
    m[1][0] = 28;
    m[1][1] = 14;
    m[1][2] = 9;
    m[1][3] = 16;
    m[1][4] = 20;
    m[1][5] = 31;
    m[2][0] = 13;
    m[2][1] = 8;
    m[2][2] = 4;
    m[2][3] = 5;
    m[2][4] = 15;
    m[2][5] = 19;
    m[3][0] = 12;
    m[3][1] = 3;
    m[3][2] = 0;
    m[3][3] = 1;
    m[3][4] = 10;
    m[3][5] = 18;
    m[4][0] = 27;
    m[4][1] = 7;
    m[4][2] = 2;
    m[4][3] = 6;
    m[4][4] = 23;
    m[4][5] = 24;
    m[5][0] = 33;
    m[5][1] = 26;
    m[5][2] = 11;
    m[5][3] = 22;
    m[5][4] = 25;
    m[5][5] = 32;

    for (int l = 0; l < height; l++){
        for (int c = 0; c < width; c++){
            x = image[l + c*width]/255.0;
            y = m[l%6][(c*width)%6]/35.0;
            if (x < y)
                dimage[l + c*width] = 0;
            else
                dimage[l + c*width] = 255;
        }
    }
}


// Ordenado com dispersão de pontos
void dith03()
{
    dimage = new GLubyte[width * height];
    float x, y;
    int m[4][4];
    m[0][0] = 1;
    m[0][1] = 15;
    m[0][2] = 2;
    m[0][3] = 12;
    m[1][0] = 9;
    m[1][1] = 5;
    m[1][2] = 10;
    m[1][3] = 6;
    m[2][0] = 3;
    m[2][1] = 13;
    m[2][2] = 0;
    m[2][3] = 14;
    m[3][0] = 11;
    m[3][1] = 7;
    m[3][2] = 8;
    m[3][3] = 4;

    for (int l = 0; l < height; l++){
        for (int c = 0; c < width; c++){
            x = image[l + c*width]/255.0;
            y = m[l%4][(c*width)%4]/15.0;
            if (x < y)
                dimage[l + c*width] = 0;
            else
                dimage[l + c*width] = 255;
        }
    }
}


int main(int argc, char *argv[])
{
    GLFWwindow* window;
    program_name = std::string(argv[0]);

    // Process command line arguments
    if (argc == 2 && argv[1][0] == '-' && argv[1][1] == 'h') {
        usage(true);
    }

    if (argc != 2) {
      msg("wrong number parameters");
      usage(false);
    }

    // read input file into 'image' vector
    std::ifstream fin(argv[1]);
    if (fin.is_open()) {
      read_ppm_file(fin);
    } else {
      msg("cannot open input file");
      std::exit(EXIT_FAILURE);
    }

    //dith00();

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Image Viewer", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glRasterPos2i(-1, -1);
        glDrawBuffer(GL_BACK);
        glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, image);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
