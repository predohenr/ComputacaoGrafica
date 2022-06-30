#ifndef TRACKBALL_H
#define TRACKBALL_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

class TrackBall
{
public:
	
	TrackBall(int width, int height, float speed = 0.03125)
	    : _dragging(false), _matrix(1.0f)
	{
	    _width  = width;
	    _height = height;
    	_speed  = speed;
	}
	
	void start(int x, int y) {
	    _dragging = true;
	    _update_previous(x, y);
    }
	void stop() {
        _update_previous(0.0f, 0.0f);
	    _dragging = false;
    }
        	
	void drag(int x, int y) {
        glm::vec3 v0 = _map_to_hyperbolic_sheet(_previous);
        glm::vec3 v1 = _map_to_hyperbolic_sheet(x, y); // current position
        glm::vec3 v2 = glm::cross(v0, v1); // calculate rotation axis

        float theta = glm::length(v2); // assuming sin(theta) \approx theta
        
        if (std::abs(theta) >= 2e-5) { // prevent from null cross products
            glm::vec3 axis = glm::normalize(v2);

            std::cout << "  previous      : " << glm::to_string(v0) << std::endl;
            std::cout << "  current       : " << glm::to_string(v1) << std::endl;                
            std::cout << "  rotation angle: " << theta*180.0/M_PI << std::endl;
            std::cout << "  rotation axis : " << glm::to_string(axis) << std::endl;

            glm::mat4 _current = glm::rotate(glm::mat4(1.0f), theta, axis);
            _matrix  = _current*_matrix;
            _update_previous(x,y);
        }
    }
    
	bool dragging() const { return _dragging; }
	
	glm::mat4& matrix() {
	    return _matrix;
	}
	
    void reset() {
	    _matrix = glm::mat4(1.0f);
	    _update_previous(0.0f, 0.0f);
	}

private:
	glm::vec3 _map_to_sphere(const glm::vec2 &p) {
        float d, x, y, z;
        
        // transforma as coordenadas para o intervalo [-1,1]
        x = (2.0*p.x - _width)/_width;
        y = (_height - 2.0*p.y)/_height;
        
        // mapeia para a esfera de raio 1
        d = x*x + y*y;
        if (d >= 1.0) {
            float theta = std::atan2(p.y, p.x);
            x = std::cos(theta);
            y = std::sin(theta);
            z = 0.0;
        } else {
            z = std::sqrt(1.0 - d);
        }

        return glm::normalize(glm::vec3(x,y,z));
    }
    
    glm::vec3 _map_to_sphere(float x, float y) {
        return _map_to_sphere(glm::vec2(x,y));
    }
    
    // Source: https://www.khronos.org/opengl/wiki/Object_Mouse_Trackball
  	glm::vec3 _map_to_hyperbolic_sheet(const glm::vec2 &p) {
        float d, x, y, z;
        
        // transforma as coordenadas para o intervalo [-1,1]
        x = (2.0*p.x - _width)/_width;
        y = (_height - 2.0*p.y)/_height;
        
        // mapeia para a esfera de raio 1
        d = x*x + y*y;
        if (d > 0.5) {
            z = 0.5/std::sqrt(d);
        } else {
            z = std::sqrt(1.0 - d);
        }

        return glm::normalize(glm::vec3(x,y,z));
    }
    
    glm::vec3 _map_to_hyperbolic_sheet(float x, float y) {
        return _map_to_hyperbolic_sheet(glm::vec2(x,y));
    }
    
   	void _update_previous(int x, int y) {
        _previous = glm::vec2(x, y);
    }
	
	int   _width, _height;
	bool  _dragging;
	float _speed;	
	glm::vec2 _previous;
	glm::mat4 _matrix;
};

#endif // TRACKBALL_H
