#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Points.h"
#include "Shader.h"

class CurveControl
{
private:
	GLuint vao, vbo;
	std::vector<float> vertices;

	public:
	CurveControl();
	~CurveControl();

	GLuint CreateLines(Points points, Shader* ourShader);
	void InitializeVAOVBO();
};

