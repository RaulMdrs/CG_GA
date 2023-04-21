#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <GL\glew.h>

class Obj {
public:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat>  normals;
    std::vector<GLfloat>  textures;
    std::vector<GLint> faces;   

    LeitorOBJ lerObj;

    void render() {
        // Render the object using the data in the class's member variables
        // ...
    }

    void translate(float x, float y, float z) {
        // Apply a translation to the object's vertices
        // ...
    }

    // Other methods for manipulating the object's data can be added as needed
};