#pragma once

#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Points
{


public:

	Points();
	~Points();


	std::vector<glm::vec3> GetControlPoints();

	void AddControlPoint(glm::vec3 point);

	std::vector<glm::vec3> controlPoints;

	void Reset();
};

