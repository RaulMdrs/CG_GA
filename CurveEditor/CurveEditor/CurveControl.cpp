#include "CurveControl.h"

CurveControl::CurveControl()
{
    vertices = std::vector<float>();
}

CurveControl::~CurveControl()
{

}

GLuint CurveControl::CreateLines(Points points)
{
    std::vector<float> vertices; // Flat array to hold the vertex data
  
    if (points.controlPoints.size() > 0)
    {
        for (const auto& point : points.controlPoints) {
            vertices.push_back(point.x);
            vertices.push_back(point.y);
            vertices.push_back(point.z);
        }

        vertices.push_back(points.controlPoints[0].x);
        vertices.push_back(points.controlPoints[0].y);
        vertices.push_back(points.controlPoints[0].z);
    }
    

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    return vao;
}

void CurveControl::CreateBSPline(Points points)
{
    float inc = 1.0f / 100.0f;
    float x, y, z;
    int size = points.GetControlPoints().size();
    bSplineCurvePoints.clear();

    for (int i = 0; i < size; i++) {
        for (float t = 0; t <= 1; t += inc) {
            x = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1) * points.controlPoints[i].x +
                (3 * pow(t, 3) - 6 * pow(t, 2) + 4) * points.controlPoints[(i + 1) % size].x +
                (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) * points.controlPoints[(i + 2) % size].x +
                (1 * pow(t, 3)) * points.controlPoints[(i + 3) % size].x) / 6);

            y = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1) * points.controlPoints[i].y +
                (3 * pow(t, 3) - 6 * pow(t, 2) + 4) * points.controlPoints[(i + 1) % size].y +
                (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) * points.controlPoints[(i + 2) % size].y +
                (1 * pow(t, 3)) * points.controlPoints[(i + 3) % size].y) / 6);

            z = (((-1 * pow(t, 3) + 3 * pow(t, 2) - 3 * t + 1) * points.controlPoints[i].z +
                (3 * pow(t, 3) - 6 * pow(t, 2) + 4) * points.controlPoints[(i + 1) % size].z +
                (-3 * pow(t, 3) + 3 * pow(t, 2) + 3 * t + 1) * points.controlPoints[(i + 2) % size].z +
                (1 * pow(t, 3)) * points.controlPoints[(i + 3) % size].z) / 6);

            bSplineCurvePoints.push_back(x);
            bSplineCurvePoints.push_back(y);
            bSplineCurvePoints.push_back(z);
        }
    }


    /*x = (((-1 * pow(0, 3) + 3 * pow(0, 2) - 3 * 0 + 1) * points.controlPoints[0].x +
        (3 * pow(0, 3) - 6 * pow(0, 2) + 4) * points.controlPoints[(0 + 1) % size].x +
        (-3 * pow(0, 3) + 3 * pow(0, 2) + 3 * 0 + 1) * points.controlPoints[(0 + 2) % size].x +
        (1 * pow(0, 3)) * points.controlPoints[(0 + 3) % size].x) / 6);

    y = (((-1 * pow(0, 3) + 3 * pow(0, 2) - 3 * 0 + 1) * points.controlPoints[0].y +
        (3 * pow(0, 3) - 6 * pow(0, 2) + 4) * points.controlPoints[(0 + 1) % size].y +
        (-3 * pow(0, 3) + 3 * pow(0, 2) + 3 * 0 + 1) * points.controlPoints[(0 + 2) % size].y +
        (1 * pow(0, 3)) * points.controlPoints[(0 + 3) % size].y) / 6);

    z = (((-1 * pow(0, 3) + 3 * pow(0, 2) - 3 * 0 + 1) * points.controlPoints[0].z +
        (3 * pow(0, 3) - 6 * pow(0, 2) + 4) * points.controlPoints[(0 + 1) % size].z +
        (-3 * pow(0, 3) + 3 * pow(0, 2) + 3 * 0 + 1) * points.controlPoints[(0 + 2) % size].z +
        (1 * pow(0, 3)) * points.controlPoints[(0 + 3) % size].z) / 6);

    bSplineCurvePoints.push_back(x);
    bSplineCurvePoints.push_back(y);
    bSplineCurvePoints.push_back(z);*/
}

void CurveControl::CreateInternalPoints(float thickness)
{
    float x, y, z, width, height, alpha, teta;
    float halfPi = glm::half_pi<float>();
    int size = bSplineCurvePoints.size();
    internalCurvePoints.clear();

    for (int i = 0; i < size; i += 3) { //loop through each point: x, y, z

        glm::vec3 p1 = glm::vec3(bSplineCurvePoints[i],
            bSplineCurvePoints[(i + 1) % size],
            bSplineCurvePoints[(i + 2) % size]);
        glm::vec3 p2 = glm::vec3(bSplineCurvePoints[(i + 3) % size],
            bSplineCurvePoints[(i + 4) % size],
            bSplineCurvePoints[(i + 5) % size]);

        width = p2.x - p1.x;
        height = p2.y - p1.y;
        teta = glm::atan(height / width);

        if (width > 0) {
            alpha = teta - halfPi;
        }
        else {
            alpha = teta + halfPi;
        }

        x = glm::cos(alpha) * thickness + p1.x;
        y = glm::sin(alpha) * thickness + p1.y;
        z = bSplineCurvePoints[(i + 2) % size];

        internalCurvePoints.push_back(x);
        internalCurvePoints.push_back(y);
        internalCurvePoints.push_back(z);
    }

    /*glm::vec3 p1 = glm::vec3(bSplineCurvePoints[0],
        bSplineCurvePoints[(0 + 1) % size],
        bSplineCurvePoints[(0 + 2) % size]);
    glm::vec3 p2 = glm::vec3(bSplineCurvePoints[(0 + 3) % size],
        bSplineCurvePoints[(0 + 4) % size],
        bSplineCurvePoints[(0 + 5) % size]);

    width = p2.x - p1.x;
    height = p2.y - p1.y;
    teta = glm::atan(height / width);

    if (width > 0) {
        alpha = teta - halfPi;
    }
    else {
        alpha = teta + halfPi;
    }

    x = glm::cos(alpha) * thickness + p1.x;
    y = glm::sin(alpha) * thickness + p1.y;
    z = bSplineCurvePoints[(0 + 2) % size];

    internalCurvePoints.push_back(x);
    internalCurvePoints.push_back(y);
    internalCurvePoints.push_back(z);*/
}

void CurveControl::CreateExternalPoints(float thickness)
{
    float x, y, z, width, height, alpha, teta;
    float halfPi = glm::half_pi<float>();
    int size = bSplineCurvePoints.size();
    externalCurvePoints.clear();

    for (int i = 0; i < size; i += 3) { //loop through each point: x, y, z

        glm::vec3 p1 = glm::vec3(bSplineCurvePoints[i],
            bSplineCurvePoints[(i + 1) % size],
            bSplineCurvePoints[(i + 2) % size]);
        glm::vec3 p2 = glm::vec3(bSplineCurvePoints[(i + 3) % size],
            bSplineCurvePoints[(i + 4) % size],
            bSplineCurvePoints[(i + 5) % size]);

        width = p2.x - p1.x;
        height = p2.y - p1.y;
        teta = glm::atan(height / width);

        if (width < 0) {
            alpha = teta - halfPi;
        }
        else {
            alpha = teta + halfPi;
        }

        x = glm::cos(alpha) * thickness + p1.x;
        y = glm::sin(alpha) * thickness + p1.y;
        z = bSplineCurvePoints[(i + 2) % size];

        externalCurvePoints.push_back(x);
        externalCurvePoints.push_back(y);
        externalCurvePoints.push_back(z);
    }

    /*glm::vec3 p1 = glm::vec3(bSplineCurvePoints[0],
        bSplineCurvePoints[(0 + 1) % size],
        bSplineCurvePoints[(0 + 2) % size]);
    glm::vec3 p2 = glm::vec3(bSplineCurvePoints[(0 + 3) % size],
        bSplineCurvePoints[(0 + 4) % size],
        bSplineCurvePoints[(0 + 5) % size]);

    width = p2.x - p1.x;
    height = p2.y - p1.y;
    teta = glm::atan(height / width);

    if (width < 0) {
        alpha = teta - halfPi;
    }
    else {
        alpha = teta + halfPi;
    }

    x = glm::cos(alpha) * thickness + p1.x;
    y = glm::sin(alpha) * thickness + p1.y;
    z = bSplineCurvePoints[(0 + 2) % size];

    externalCurvePoints.push_back(x);
    externalCurvePoints.push_back(y);
    externalCurvePoints.push_back(z);*/
}

GLuint CurveControl::BsplineToVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, vboBSpline);

    glBufferData(GL_ARRAY_BUFFER, bSplineCurvePoints.size() * sizeof(GLfloat), bSplineCurvePoints.data(), GL_STATIC_DRAW);

    return vaoBSpline;
}

GLuint CurveControl::InternalToVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, vboInternal);

    glBufferData(GL_ARRAY_BUFFER, internalCurvePoints.size() * sizeof(GLfloat), internalCurvePoints.data(), GL_STATIC_DRAW);

    return vboInternal;
}

GLuint CurveControl::ExternalToVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, vboExternal);

    glBufferData(GL_ARRAY_BUFFER, externalCurvePoints.size() * sizeof(GLfloat), externalCurvePoints.data(), GL_STATIC_DRAW);

    return vboExternal;
}

void CurveControl::InitializeVAOVBO()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Set up the vertex attribute bindings  
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(0);

    // Unbind the VAO and VBO

    glBindVertexArray(0);

    // B-Spline curve
    glGenVertexArrays(1, &vaoBSpline);
    glGenBuffers(1, &vboBSpline);

    glBindVertexArray(vaoBSpline);
    glBindBuffer(GL_ARRAY_BUFFER, vboBSpline);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // Internal curve
    glGenVertexArrays(1, &vaoInternal);
    glGenBuffers(1, &vboInternal);

    glBindVertexArray(vaoInternal);
    glBindBuffer(GL_ARRAY_BUFFER, vboInternal);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // External curve
    glGenVertexArrays(1, &vaoExternal);
    glGenBuffers(1, &vboExternal);

    glBindVertexArray(vaoExternal);
    glBindBuffer(GL_ARRAY_BUFFER, vboExternal);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), NULL);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

std::vector<float> CurveControl::GetBSplinePoints()
{
    return bSplineCurvePoints;
}

std::vector<float> CurveControl::GetInternalPoints()
{
    return internalCurvePoints;
}

std::vector<float> CurveControl::GetExternalPoints()
{
    return externalCurvePoints;
}

void CurveControl::Reset()
{
    bSplineCurvePoints.clear();
	internalCurvePoints.clear();
	externalCurvePoints.clear();
    vertices.clear();
}
