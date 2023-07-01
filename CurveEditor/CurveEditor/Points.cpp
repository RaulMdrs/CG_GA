#include "Points.h"

Points::Points()
{
	controlPoints = std::vector<glm::vec3>();
}

Points::~Points()
{
}

std::vector<glm::vec3> Points::GetControlPoints()
{
	return controlPoints;
}

void Points::AddControlPoint(glm::vec3 point)
{
	if (controlPoints.size() == 0)
	{
		controlPoints.push_back(point);
	}
	else if(point != controlPoints[controlPoints.size() - 1])
	{
		controlPoints.push_back(point);		
	}
}

void Points::Reset()
{
	controlPoints.clear();
}
