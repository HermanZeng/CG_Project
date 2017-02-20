#ifndef SPHERE_H
#define SPHERE_H

#define _USE_MATH_DEFINES
#include <iostream>
#include <vector>

#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/gtc/constants.hpp> // glm::pi

class Sphere
{
public:
	std::vector<glm::vec3> vertices;
	std::vector<GLuint> indices;
	Sphere(GLuint rings, GLuint sectors);
};

#endif