#pragma once
#include "CurveControl.h"

class CurveToOBJ
{

public:
	void createObj(CurveControl* curveControl);
	glm::vec3 createVectorFrom2Points(glm::vec3 a, glm::vec3 b);
	void createBSplinePointsTxt(CurveControl* curveControl);

	CurveToOBJ();
	~CurveToOBJ();
private:

	

	std::vector <float> BSplinePoints, InternalPoints, ExternalPoints;
};

