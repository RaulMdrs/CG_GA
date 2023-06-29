#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Points.h"
#include "Shader.h"

class CurveControl
{
private:
	GLuint vao, vbo, vaoBSpline, vboBSpline, internalVAO, internalVBO, externalVAO, externalVBO;
	std::vector<float> vertices;
	std::vector<float> bSplineCurvePoints, internalCurvePoints, externalCurvePoints;
	public:
	CurveControl();
	~CurveControl();

	GLuint CreateLines(Points points);

	void CreateBSPline(Points points);
	void CreateInternalPoints(Points points);
	void CreateExternalPoints(Points points);

	GLuint BsplineToVBO();
	GLuint InternalToVBO();
	GLuint ExternalToVBO();
	void InitializeVAOVBO();
};

