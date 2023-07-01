#include "CurveToOBJ.h"

CurveToOBJ::CurveToOBJ()
{
}

CurveToOBJ::~CurveToOBJ()
{
}

void CurveToOBJ::createObj(CurveControl* curveControl)
{
	InternalPoints.clear();
	ExternalPoints.clear();
	InternalPoints = curveControl->GetInternalPoints();
	ExternalPoints = curveControl->GetExternalPoints();
	int size = InternalPoints.size();

	std::ofstream arq("track.obj");

	arq << "mtllib track.mtl" << std::endl;

	//v
	for (int i = 0; i < size; i += 3) {//loop through each point: x, y, z
		arq << "v " << InternalPoints[i] << " " //x
			<< InternalPoints[i + 2] * 10 << " " //z
			<< InternalPoints[i + 1] << std::endl; //y
	}

	for (int i = 0; i < ExternalPoints.size(); i += 3) {
		arq << "v " << ExternalPoints[i] << " " //x
			<< ExternalPoints[i + 2] * 10 << " " //z
			<< ExternalPoints[i + 1] << std::endl; //y
	}
	//vt
	arq << "vt " << 0 << " " << 0 << std::endl;
	arq << "vt " << 1 << " " << 0 << std::endl;
	arq << "vt " << 1 << " " << 1 << std::endl;
	arq << "vt " << 0 << " " << 1 << std::endl;

	//vn
	for (int i = 0; i < size - 5; i += 3) { //loop through each point: x, y, z

		glm::vec3 A = glm::vec3(InternalPoints[i], InternalPoints[(i + 2)], InternalPoints[(i + 1)]);
		glm::vec3 B = glm::vec3(InternalPoints[(i + 3)], InternalPoints[(i + 5)], InternalPoints[(i + 4)]);
		glm::vec3 C = glm::vec3(ExternalPoints[i], ExternalPoints[(i + 2)], ExternalPoints[(i + 1)]);
		glm::vec3 D = glm::vec3(ExternalPoints[(i + 3)], ExternalPoints[(i + 5)], ExternalPoints[(i + 4)]);

		glm::vec3 AB = createVectorFrom2Points(A, B);
		glm::vec3 AC = createVectorFrom2Points(A, C);
		glm::vec3 BC = createVectorFrom2Points(B, C);
		glm::vec3 BD = createVectorFrom2Points(B, D);

		glm::vec3 vn1 = glm::cross(AB, AC);
		glm::vec3 vn2 = glm::cross(BC, BD);

		if (vn1.y < 0) {
			vn1 = glm::vec3(vn1.x * -1, vn1.y * -1, vn1.z * -1);
		}

		if (vn2.y < 0) {
			vn2 = glm::vec3(vn2.x * -1, vn2.y * -1, vn2.z * -1);
		}

		arq << "vn " << (unsigned int)vn1.x << " " << vn1.y << " " << (unsigned int)vn1.z << std::endl;
		arq << "vn " << (unsigned int)vn2.x << " " << vn2.y << " " << (unsigned int)vn2.z << std::endl;
	}

	arq << " " << std::endl;

	//g and usemtl
	arq << "g track" << std::endl;
	arq << "usemtl track" << std::endl;

	//f

	int vnNum = 1;
	bool isTex1 = true;
	glm::vec3 tex1 = glm::vec3(1, 2, 4);
	glm::vec3 tex2 = glm::vec3(4, 2, 3);

	for (int i = 1; i < size / 3; i++) {
		//v1/t1/n1

		arq << "f " << i << "/" << tex1.x << "/" << vnNum << " "
			<< i + 1 << "/" << tex1.y << "/" << vnNum << " "
			<< i + (size / 3) << "/" << tex1.z << "/" << vnNum << std::endl;

		arq << "f " << i + (size / 3) << "/" << tex2.x << "/" << vnNum << " "
			<< i + 1 << "/" << tex2.y << "/" << vnNum << " "
			<< i + (size / 3) + 1 << "/" << tex2.z << "/" << vnNum << std::endl;

		vnNum++;
		if (vnNum >= 4) {
			vnNum = 1;
		}
	}

	arq.close();
}

glm::vec3 CurveToOBJ::createVectorFrom2Points(glm::vec3 a, glm::vec3 b)
{
	return glm::vec3(b.x - a.x, b.y - a.y, b.z - a.z);
}

void CurveToOBJ::createBSplinePointsTxt(CurveControl* curveControl)
{
	std::ofstream arq("bspline.txt");

	BSplinePoints.clear();
	BSplinePoints = curveControl->GetBSplinePoints();
	int size = BSplinePoints.size() / 3;

	arq << size << " " << std::endl;

	for (int i = 0; i < size; i += 3) {
		arq << BSplinePoints[i] << " " << BSplinePoints[i + 2] << " " << BSplinePoints[i + 1] << std::endl;
	}

	arq.close();
}