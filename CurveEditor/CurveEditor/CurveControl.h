#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Points.h"
#include "Shader.h"

class CurveControl
{
private:
	GLuint vao, vbo, vaoBSpline, vboBSpline, vaoInternal, vboInternal, vaoExternal, vboExternal;
	std::vector<float> vertices;
	std::vector<float> bSplineCurvePoints, internalCurvePoints, externalCurvePoints;
	public:
	CurveControl();
	~CurveControl();

	GLuint CreateLines(Points points);

	void CreateBSPline(Points points);
	void CreateInternalPoints(float thickness);
	void CreateExternalPoints(float thickness);

	GLuint BsplineToVBO();
	GLuint InternalToVBO();
	GLuint ExternalToVBO();
	void InitializeVAOVBO();

	std::vector<float> GetBSplinePoints();
	std::vector<float> GetInternalPoints();
	std::vector<float> GetExternalPoints();

	void Reset();
};

