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
    float inc = 5.0f / 100.0f;
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
}

void CurveControl::CreateInternalPoints(Points points)
{

}

void CurveControl::CreateExternalPoints(Points points)
{

}

GLuint CurveControl::BsplineToVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, vboBSpline);

    glBufferData(GL_ARRAY_BUFFER, bSplineCurvePoints.size() * sizeof(float), bSplineCurvePoints.data(), GL_STATIC_DRAW);

    return vaoBSpline;
}

GLuint CurveControl::InternalToVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, internalVBO);

    glBufferData(GL_ARRAY_BUFFER, internalCurvePoints.size() * sizeof(GLfloat), internalCurvePoints.data(), GL_STATIC_DRAW);

    return internalVAO;
}

GLuint CurveControl::ExternalToVBO()
{
    glBindBuffer(GL_ARRAY_BUFFER, externalVBO);

    glBufferData(GL_ARRAY_BUFFER, externalCurvePoints.size() * sizeof(GLfloat), externalCurvePoints.data(), GL_STATIC_DRAW);

    return externalVAO;
}

void CurveControl::InitializeVAOVBO()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Set up the vertex attribute bindings  
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VAO and VBO

    glBindVertexArray(0);

    glGenVertexArrays(1, &vaoBSpline);
    glGenBuffers(1, &vboBSpline);

    glBindVertexArray(vaoBSpline);
    glBindBuffer(GL_ARRAY_BUFFER, vboBSpline);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}
