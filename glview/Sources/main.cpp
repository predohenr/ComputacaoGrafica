#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <cmath>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

std::string program_name;
GLsizei width, height;

// Read a shader source from a file
// store the shader source in a std::vector<char>
void
read_shader_src(const char *fname, std::vector<char> &buffer);

// Compile a shader
GLuint
load_and_compile_shader(const char *fname, GLenum shaderType);

// Create a program from two shaders
GLuint
create_program(const char *path_vert_shader, const char *path_frag_shader);

// Called when the window is resized
void
window_resized(GLFWwindow *window, int width, int height);

// Called for keyboard events
void
keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);

// Render scene
void
display(GLFWwindow* window, GLuint &vao);

// Initialize the data to be rendered
void
initialize(GLuint &vao);

static void
glfwError(int id, const char* description)
{
  std::cout << description << std::endl;
}

int
main(int argc, char *argv[])
{
  GLFWwindow* window;
  program_name = std::string(argv[0]);

  /* Initialize the library */
  if (!glfwInit())
    return -1;
    
  // Use OpenGL 3.2 core profile
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwSetErrorCallback(&glfwError);

  /* Create a windowed mode window and its OpenGL context */
  width = 400;
  height = 400;
  window = glfwCreateWindow(width, height, "OpenGL Viewer", NULL, NULL);
  if (!window) {
    std::cerr << "Failed to open a window." << std::endl;
    glfwTerminate();
    return -1;
  }
    
  // Register a callback function for window resize events
  glfwSetWindowSizeCallback(window, &window_resized);
  
    // Register a callback function for keyboard pressed events
  glfwSetKeyCallback(window, &keyboard);

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  
  // Print the OpenGL version
  std::cout << "OpenGL - " << glGetString(GL_VERSION) << std::endl;
  
  // Initialize GLEW
  glewExperimental = GL_TRUE;
  if(glewInit() != GLEW_OK) {
    std::cerr << "Failed to initialize GLEW! I'm out!" << std::endl;
    glfwTerminate();
    exit(-1);
  }

  // Create a vertex array object
  GLuint vao;
  
  // Initialize the data to be rendered
  initialize(vao);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    // Display scene
    display(window, vao);

    /* Poll for and process events */
    glfwPollEvents();
  }
  
  // Terminate GLFW
  glfwTerminate();
  
  return 0;
}

// Render scene
void
display(GLFWwindow* window, GLuint &vao)
{
  glClear(GL_COLOR_BUFFER_BIT);

  glBindVertexArray(vao);
  glDrawElements(GL_TRIANGLES, 64*3, GL_UNSIGNED_INT, 0);

  // Swap front and back buffers
  glfwSwapBuffers(window);
}

float* rota(float x, float y, float ang){
  float *resp = (float*)malloc(sizeof(float) * 2);
  float cos = std::cos(ang*M_PI/180);
  float sen = std::sin(ang*M_PI/180);
  if (cos < 1.0e-3 && cos > 0){
    cos = 0.0;
  }
  if (sen < 1.0e-3 && sen > 0){
    sen = 0.0;
  }
  resp[0] = cos*x - sen*y;
  resp[1] = sen*x + cos*y;
  return resp;
}

void
initialize(GLuint &vao)
{
  // Use a Vertex Array Object
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // 1 square (made by 2 triangles) to be rendered
  int qtd = 64;
  float* resp;
  float angulo = -360/(float)qtd;
  GLfloat vertices_position[qtd * 3];
  vertices_position[0]=0.0;
  vertices_position[1]=0.0;
  vertices_position[2]=0.0;
  vertices_position[3]=0.5;
  for(int i = 4, l=1; i<qtd*3 && l<qtd;i++, l++){
    resp = rota(vertices_position[i-2], vertices_position[i-1], angulo);
     vertices_position[i] = resp[0];
     vertices_position[++i] = resp[1];
    //  std::cout << angulo*l << std::endl;
    //  std::cout << vertices_position[i-1] << std::endl;
    //  std::cout << vertices_position[i] << std::endl;
  }

  GLuint indices[qtd*3];
  for(int i = 0, j=0;i<(qtd*3)-1; i=i+3, j++){
      indices[i] = 0;
      indices[i+1] = j+1;
      indices[i+2] = j+2;
  }
  indices[(qtd*3)-3] = 0;
  indices[(qtd*3)-2] = qtd;
  indices[(qtd*3)-1] = 1;

  GLfloat colors[(qtd*9)+1];

  // Fill colors with random numbers from 0 to 1, use continuous polynomials for r,g,b:
  int k = 0;
  for(int i = 0; i < sizeof(colors)/sizeof(float)/3; ++i) {
    colors[k++] = 1.0;
    colors[k++] = 1.0;
    colors[k++] = 1.0;
  }
  
  colors[0] = 1.0;
  colors[1] = 0.0;
  colors[2] = 0.0;

  // Create a Vector Buffer Object that will store the vertices on video memory
  GLuint vbo;
  glGenBuffers(1, &vbo);

  // Allocate space for vertex positions and colors
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_position) + sizeof(colors), NULL, GL_STATIC_DRAW);

  // Transfer the vertex positions:
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices_position), vertices_position);

  // Transfer the vertex colors:
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices_position), sizeof(colors), colors);

  // Create an Element Array Buffer that will store the indices array:
  GLuint eab;
  glGenBuffers(1, &eab);

  // Transfer the data from indices to eab
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  GLuint shaderProgram = create_program("Sources/shaders/vert.shader", "Sources/shaders/frag.shader");

  // Get the location of the attributes that enters in the vertex shader
  GLint position_attribute = glGetAttribLocation(shaderProgram, "position");

  // Specify how the data for position can be accessed
  glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

  // Enable the attribute
  glEnableVertexAttribArray(position_attribute);

  // Color attribute
  GLint color_attribute = glGetAttribLocation(shaderProgram, "color");
  glVertexAttribPointer(color_attribute, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(vertices_position));
  glEnableVertexAttribArray(color_attribute);
}

// Called when the window is resized
void
window_resized(GLFWwindow* window, int width, int height)
{
  // Use  black to clear the screen
  glClearColor(0, 0, 0, 1);

  // Set the viewport
  glViewport(0, 0, width, height);

  glClear(GL_COLOR_BUFFER_BIT);
  glfwSwapBuffers(window);
}

// Called for keyboard events
void
keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if(key == 'Q' && action == GLFW_PRESS) {
    glfwTerminate();
    exit(0);
  }
}

// Read a shader source from a file
// store the shader source in a std::vector<char>
void
read_shader_src(const char *fname, std::vector<char> &buffer)
{
  std::ifstream in;
  in.open(fname, std::ios::binary);

  if(in.is_open()) {
    // Get the number of bytes stored in this file
    in.seekg(0, std::ios::end);
    size_t length = (size_t)in.tellg();

    // Go to start of the file
    in.seekg(0, std::ios::beg);

    // Read the content of the file in a buffer
    buffer.resize(length + 1);
    in.read(&buffer[0], length);
    in.close();
    // Add a valid C - string end
    buffer[length] = '\0';
  }
  else {
    std::cerr << "Unable to open " << fname << " I'm out!" << std::endl;
    exit(-1);
  }
}

// Compile a shader
GLuint
load_and_compile_shader(const char *fname, GLenum shaderType)
{
  // Load a shader from an external file
  std::vector<char> buffer;
  read_shader_src(fname, buffer);
  const char *src = &buffer[0];

  // Compile the shader
  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &src, NULL);
  glCompileShader(shader);
  // Check the result of the compilation
  GLint test;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &test);
  if(!test) {
    std::cerr << "Shader compilation failed with this message:" << std::endl;
    std::vector<char> compilation_log(512);
    glGetShaderInfoLog(shader, compilation_log.size(), NULL, &compilation_log[0]);
    std::cerr << &compilation_log[0] << std::endl;
    glfwTerminate();
    exit(-1);
  }
  return shader;
}

// Create a program from two shaders
GLuint
create_program(const char *path_vert_shader, const char *path_frag_shader)
{
  // Load and compile the vertex and fragment shaders
  GLuint vertexShader = load_and_compile_shader(path_vert_shader, GL_VERTEX_SHADER);
  GLuint fragmentShader = load_and_compile_shader(path_frag_shader, GL_FRAGMENT_SHADER);

  // Attach the above shader to a program
  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);

  // Flag the shaders for deletion
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Link and use the program
  glLinkProgram(shaderProgram);
  glUseProgram(shaderProgram);

  return shaderProgram;
}



int main2(){
  int qtd = 8;
  float* resp;
  float angulo = -360/(float)qtd;
  GLfloat vertices_position[qtd * 3];
  vertices_position[0]=0.0;
  vertices_position[1]=0.0;
  vertices_position[2]=0.0;
  vertices_position[3]=0.5;
  for(int i = 4, l=1; i<qtd*3 && l<qtd;i++, l++){
    resp = rota(vertices_position[i-2], vertices_position[i-1], angulo);
     vertices_position[i] = resp[0];
     vertices_position[++i] = resp[1];
    //  std::cout << angulo*l << std::endl;
    //  std::cout << vertices_position[i-1] << std::endl;
    //  std::cout << vertices_position[i] << std::endl;
  }
  GLuint indices[qtd*3];
  for(int i = 0, j=0;i<(qtd*3)-1; i=i+3, j++){
      indices[i] = 0;
      indices[i+1] = j+1;
      indices[i+2] = j+2;
  }
  indices[(qtd*3)-3] = 0;
  indices[(qtd*3)-2] = qtd;
  indices[(qtd*3)-1] = 1;

  return 0;
}
