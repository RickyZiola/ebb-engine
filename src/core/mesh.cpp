#include <ebb/core/geometry/mesh.hpp>
#include <stdio.h>
#include <GL/glu.h>

Ebb::Core::Mesh::Mesh(Ebb::Core::ShaderProgram *shader) {
    this->shader = shader;
    glGenVertexArrays(1, &VAO);

    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    this->numTriangles = 0;
}

Ebb::Core::Mesh::Mesh(unsigned int vertexCount, float3 vertexTable[],
                      unsigned int triCount, unsigned int edgeTable[],
                      Ebb::Core::ShaderProgram *shader) {
    this->shader = shader;


    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        // Handle the error here, e.g., print an error message or log it.
        fprintf(stderr, "OpenGL Error before mesh setup: %s\n", gluErrorString(error));
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    this->numTriangles = triCount;

    /*
    INFO LAYOUT:
        |vX| |vY| |vZ|  | tX || tY |
        | vertex pos |  | texcoord |
    */
    float meshInfo[vertexCount * 5];

    for (int i = 0; i < vertexCount; ++i) {
        meshInfo[i * 5 + 0] = vertexTable[i].x;
        meshInfo[i * 5 + 1] = vertexTable[i].y;
        meshInfo[i * 5 + 2] = vertexTable[i].z;

        /*
            Unused unless texcoords are specified
        */
        meshInfo[i * 5 + 3] = 0.0;
        meshInfo[i * 5 + 4] = 0.0;
    }

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(meshInfo), meshInfo, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triCount * 3 * sizeof(unsigned int), edgeTable, GL_STATIC_DRAW);

    /*
        Index 0 is vertex position, index 1 is texcoord.
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while ((error = glGetError()) != GL_NO_ERROR) {
        // Handle the error here, e.g., print an error message or log it.
        fprintf(stderr, "OpenGL Error in mesh setup: %s\n", gluErrorString(error));
    }
}

Ebb::Core::Mesh::Mesh(unsigned int vertexCount, float3 vertexTable[],
                      unsigned int triCount, unsigned int edgeTable[],
                      float2 texCoords[], Ebb::Core::ShaderProgram *shader) {
    this->shader = shader;


    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        // Handle the error here, e.g., print an error message or log it.
        fprintf(stderr, "OpenGL Error before mesh setup: %s\n", gluErrorString(error));
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    this->numTriangles = triCount;

    /*
    INFO LAYOUT:
        |vX| |vY| |vZ|  | tY || tY |
        | vertex pos |  | texcoord |
    */
    float meshInfo[vertexCount * 5];

    for (int i = 0; i < vertexCount; ++i) {
        meshInfo[i * 5 + 0] = vertexTable[i].x;
        meshInfo[i * 5 + 1] = vertexTable[i].y;
        meshInfo[i * 5 + 2] = vertexTable[i].z;
        
        meshInfo[i * 5 + 3] = texCoords[i].x;
        meshInfo[i * 5 + 4] = texCoords[i].y;
    }
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(meshInfo), meshInfo, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triCount * 3 * sizeof(unsigned int), edgeTable, GL_STATIC_DRAW);

    /*
        Index 0 is vertex position, index 1 is texcoord.
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    while ((error = glGetError()) != GL_NO_ERROR) {
        // Handle the error here, e.g., print an error message or log it.
        fprintf(stderr, "OpenGL Error in mesh setup: %s\n", gluErrorString(error));
    }
}

void Ebb::Core::Mesh::draw() {
    this->shader->use();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3 * this->numTriangles, GL_UNSIGNED_INT, 0);
}