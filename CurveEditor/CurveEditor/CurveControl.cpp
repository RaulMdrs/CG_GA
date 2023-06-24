#include "CurveControl.h"

CurveControl::CurveControl()
{
    vertices = std::vector<float>();
}

CurveControl::~CurveControl()
{

}

GLuint CurveControl::CreateLines(Points points, Shader* ourShader)
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

void CurveControl::InitializeVAOVBO()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Set up the vertex attribute bindings  
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind the VAO and VBO

    glBindVertexArray(0);
}
