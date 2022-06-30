#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <cmath>
#include <cassert>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <Mesh.h>
#include <Shader.h>
#include <Scene.h>
#include <TrackBall.h>

enum {
    MODE_IDLE = 0,
    MODE_ORBITING,
    MODE_ZOOMING,
    MODE_PANNING
};

std::string program_name;
GLsizei width, height, status; // window size
int direction;
float theta = 0;
float head = 0;
float rotate = 0;
glm::vec3 head_RotationAxis;
glm::mat4 matrixLegL;
glm::mat4 matrixLegR;
glm::mat4 matrixArmL;
glm::mat4 matrixArmR;
glm::mat4 matrixHead;
glm::mat4 matrixBody;

class MyScene : public Scene
{
public:
    MyScene()
        : _theta(0.0) { }
        
public:
    void rotate_body(){
        head_RotationAxis = leg_top_center(0);

        matrixLegL = glm::translate(glm::mat4(1.0f), head_RotationAxis);

        matrixLegL = glm::rotate(matrixLegL, 0.1f*direction, glm::vec3(0.0,1.0,0.0));

        matrixLegL = glm::translate(matrixLegL, -head_RotationAxis);
        
        glm::mat4 matrix = model(0).matrix();
        glm::mat4 update = matrix*matrixLegL;
        model(0).set_matrix(update);
    }
public:
    void move_head(){
              glm::vec3 head_RotationAxis = leg_top_center(0);
              glm::mat4 matrixHead = glm::translate(glm::mat4(1.0f), head_RotationAxis);
              matrixHead = glm::rotate(matrixHead, head, glm::vec3(0.0,1.0,0.0));
              matrixHead = glm::translate(matrixHead, -head_RotationAxis);
              model(0).mesh(0).set_matrix(matrixHead);
    }
public:
    void walk()
    {
        glm::vec3 legL_RotationAxis = leg_top_center(2);
        glm::vec3 legR_RotationAxis = leg_top_center(5);
        glm::vec3 armL_RotationAxis = leg_top_center(3);
        glm::vec3 armR_RotationAxis = leg_top_center(4);
        head_RotationAxis = leg_top_center(0);
        glm::vec3 body_RotationAxis = leg_top_center(1);

        // we know meshes #2 and #5 are left and right legs, respectively
        //right arm is #4 and left arm is #3
        glm::mat4 matrixLegL = glm::translate(glm::mat4(1.0f), legL_RotationAxis);
        glm::mat4 matrixLegR = glm::translate(glm::mat4(1.0f), legR_RotationAxis);
        glm::mat4 matrixArmL = glm::translate(glm::mat4(1.0f), armL_RotationAxis);
        glm::mat4 matrixArmR = glm::translate(glm::mat4(1.0f), armR_RotationAxis);

        matrixLegL = glm::rotate(matrixLegL, std::sin(theta), glm::vec3(0.0,0.0,1.0));
        matrixLegR = glm::rotate(matrixLegR, std::sin(theta), glm::vec3(0.0,0.0,-1.0));
        matrixArmL = glm::rotate(matrixArmL, std::sin(theta), glm::vec3(0.0,0.0,-1.0));
        matrixArmR = glm::rotate(matrixArmR, std::sin(theta), glm::vec3(0.0,0.0,1.0));
        theta+=2*M_PI/100;
        if (theta>=2*M_PI) {
          theta=0;
        }

        matrixLegL = glm::translate(matrixLegL, -legL_RotationAxis);
        matrixLegR = glm::translate(matrixLegR, -legR_RotationAxis);
        matrixArmL = glm::translate(matrixArmL, -armL_RotationAxis);
        matrixArmR = glm::translate(matrixArmR, -armR_RotationAxis);

        model(0).mesh(2).set_matrix(matrixLegL);
        model(0).mesh(5).set_matrix(matrixLegR);
        model(0).mesh(3).set_matrix(matrixArmL);
        model(0).mesh(4).set_matrix(matrixArmR);

        glm::mat4 walking = model(0).matrix();
        walking = glm::translate(walking, glm::vec3(0.2,0.0,0.0));
        model(0).set_matrix(walking);
    }
    
    glm::vec3 leg_top_center(int leg)
    {
       // compute bounding box
       GLfloat xmin(0), xmax(0), ymin(0), ymax(0), zmin(0), zmax(0);
       
       Mesh& mesh = model(0).mesh(leg);
       for (size_t i = 0; i < mesh.number_of_vertices(); ++i) {
           glm::vec3 p = mesh.vertex(i).Position;
           
           std::cout << glm::to_string(p) << std::endl;
           
           xmax = std::max(xmax, p.x);
           xmin = std::min(xmin, p.x);
           ymax = std::max(ymax, p.y);
           ymin = std::min(ymin, p.y);
           zmax = std::max(zmax, p.z);
           zmin = std::min(zmin, p.z);
       }
       
       return glm::vec3(0.5*(xmin + xmax),
                        ymax,
                        0.5*(zmin + zmax));
    }
    
private:
    GLfloat _theta;
};

MyScene scene;
TrackBall* trackball;

// Called when the window is resized
void
window_resized(GLFWwindow *window, int width, int height);

// Called for keyboard events
void
keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);

// Called for mouse events
void
mouse_button(GLFWwindow* window, int button, int action, int mods);

void mouse_motion(GLFWwindow* window, double x, double y);

// Render scene
void
display(GLFWwindow* window);

// Initialize the data to be rendered
void
initialize();

static void
error(int id, const char* description);

int
main(int argc, char *argv[])
{
  GLFWwindow* window;
  program_name = std::string(argv[0]);
  
  // Initialize the library
  if (!glfwInit())
    return EXIT_FAILURE;
    
  // Use OpenGL 3.2 core profile
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwSetErrorCallback(&error);

  // Create a windowed mode window and its OpenGL context
  width = 1000;
  height = 1000;
  window = glfwCreateWindow(width, height, "OpenGL Viewer", NULL, NULL);
  if (!window) {
    error(-1, "Failed to open a window.");
    glfwTerminate();
    return EXIT_FAILURE;
  }
    
  // Register a callback function for window resize events
  glfwSetWindowSizeCallback(window, &window_resized);
  
  // Register a callback function for keyboard pressed events
  glfwSetKeyCallback(window, &keyboard);
  
  // Register a callback function for mouse buttons events
  glfwSetMouseButtonCallback(window, mouse_button);

  // Register a callback function for mouse move events
  glfwSetCursorPosCallback(window, mouse_motion);

  // Make the window's context current
  glfwMakeContextCurrent(window);
  
  // Print the OpenGL version
  std::cout << "OpenGL - " << glGetString(GL_VERSION) << std::endl;
  
  // Initialize GLEW
  glewExperimental = GL_TRUE;
  if(glewInit() != GLEW_OK) {
    error(EXIT_FAILURE, "Failed to initialize GLEW! I'm out!");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  // Initialize the scene to be rendered
  initialize();
  
  // Loop until the user closes the window
  while (!glfwWindowShouldClose(window))
  {
    // Display scene
    display(window);

    // Poll for and process events
    glfwPollEvents();
  }
  
  // Terminate GLFW
  glfwTerminate();
  
  return EXIT_SUCCESS;
}

// Render scene
void
display(GLFWwindow* window)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
    scene.set_orbit(trackball->matrix());

    scene.render();

    // Swap front and back buffers
    glfwSwapBuffers(window);
}

void
initialize()
{
    glEnable(GL_DEPTH_TEST);

    // set projection
    scene.set_projection(45.0, (float)width/(float)height, 1.0, 100.0);

    // set view
    glm::vec3 eye(0.0,0.0,10.0);
    glm::vec3 at(0.0,0.0,-1.0);
    glm::vec3 up(0.0,1.0,0.0);
    scene.set_view(eye,at,up);

    // setting up a virtual trackball
    trackball = new TrackBall(width,height);

    // set scene light
    Light light = {
        glm::vec3(1.2f, 1.0f, 2.0f), // position
        glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), // ambient
        glm::vec4(0.7f, 0.7f, 0.7f, 1.0f), // diffuse
        glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), // specular
    };
    scene.set_light(light);

    scene.set_shader("Sources/shaders/vertex.glsl", "Sources/shaders/fragment.glsl");
    
    // add model from OBJ    
    scene.add_model("Data/Steve.obj");
    scene.add_model("Data/untitled.obj");
    // set model matrix
    glm::mat4 matrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, -4.0f, -8.0f));
    scene.model(0).set_matrix(matrix);
    scene.model(1).set_matrix(matrix);
    

    std::cout << "Number of models: " << scene.number_of_models() << std::endl;
}

// Called when the window is resized
void
window_resized(GLFWwindow* window, int width, int height)
{
  // Use  black to clear the screen
  glClearColor(0, 0, 0, 1);

  // Set the viewport
  glViewport(0, 0, width, height);
  
  scene.set_projection(45.0, (float)width/(float)height, 1.0, 100.0);
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glfwSwapBuffers(window);
}

// Called for keyboard events
void
keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  switch (key) {
    case GLFW_KEY_W:
      if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        std::cout << "caminha para frente\n";
        scene.walk();        
      }
      break;
    case GLFW_KEY_D:
      if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        std::cout << "roda o corpo\n";
        direction = 1;
        rotate+=0.1;
        scene.rotate_body();        
      }
      break;
    case GLFW_KEY_A:
      if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        std::cout << "roda o corpo\n";
        direction = -1;
        rotate-=0.1;
        scene.rotate_body();        
      }
      break;
    case GLFW_KEY_E:
      if(action == GLFW_PRESS || action == GLFW_REPEAT){
        std::cout << "mexe a cabeça\n";
        head-=0.1;
        scene.move_head();
      }
      break;
    case GLFW_KEY_Q:
      if(action == GLFW_PRESS || action == GLFW_REPEAT){
        std::cout << "mexe a cabeça\n";
        head+=0.1;
        scene.move_head();
      }
      break;
    case GLFW_KEY_R:
      if (action == GLFW_PRESS) {
        std::cout << "reset view.\n";   
        trackball->reset();
      }
      break;
    case GLFW_KEY_O:
      if (action == GLFW_PRESS) {
        status = MODE_ORBITING;
        std::cout << "entering ORBIT mode.\n";
      }
      break;
    case GLFW_KEY_ESCAPE:
      if (action == GLFW_PRESS) {
        glfwTerminate();
        exit(0);
      }
    case GLFW_KEY_I:
      if (action == GLFW_PRESS) {
        status = MODE_IDLE;
        std::cout << "entering IDLE mode.\n";
      }
    default:
      break;
  }
}

void mouse_button(GLFWwindow* window, int button, int action, int mods)
{
	if (button != GLFW_MOUSE_BUTTON_LEFT)
		return;
	
	switch (action) {
		case GLFW_PRESS:
			double x, y;
         	glfwGetCursorPos(window, &x, &y);
         	std::cout << "cursor position: " << x << ", " << y << std::endl;
         	if (status == MODE_ORBITING)
         	    trackball->start(x, y);
			break;
		case GLFW_RELEASE:
       		if (status == MODE_ORBITING)
		    	trackball->stop();
			break;
   }
}

void mouse_motion(GLFWwindow* window, double x, double y)
{
    if (trackball->dragging())
        trackball->drag(x, y);
}

static void
error(int id, const char* description)
{
  std::cerr << program_name << ": " << description << std::endl;
}

