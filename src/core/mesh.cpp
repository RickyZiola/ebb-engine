#include <ebb/core/geometry/mesh.hpp>
#include <vector>
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
                      unsigned int triCount, unsigned int triTable[],
                      Ebb::Core::ShaderProgram *shader) {
    this->shader = shader;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    this->numTriangles = triCount;

    /*
    INFO LAYOUT:
        |vX| |vY| |vZ|  |nX| |nY| |nZ|  | tX || tY |
        | vertex pos |  |   normal   |  | texcoord |
        |  3 floats  |  |  3 floats  |  | 2 floats |
        |                  8 floats                |
    */
    float meshInfo[vertexCount * 8];

    for (int i = 0; i < vertexCount; ++i) {
        meshInfo[i * 8 + 0] = vertexTable[i].x;
        meshInfo[i * 8 + 1] = vertexTable[i].y;
        meshInfo[i * 8 + 2] = vertexTable[i].z;

        meshInfo[i * 8 + 3] = 1.0; // TODO: normals
        meshInfo[i * 8 + 4] = 0.0; // TODO: normals
        meshInfo[i * 8 + 5] = 0.0; // TODO: normals

        /*
            Unused unless texcoords are specified
        */
        meshInfo[i * 8 + 6] = 0.0;
        meshInfo[i * 8 + 7] = 0.0;
    }

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(meshInfo), meshInfo, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triCount * 3 * sizeof(unsigned int), triTable, GL_STATIC_DRAW);

    /*
        Index 0 is vertex position, index 1 is normal, index 2 is texcoord.
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

Ebb::Core::Mesh::Mesh(unsigned int vertexCount, float3 vertexTable[],
                      unsigned int triCount, unsigned int triTable[],
                      float2 texCoords[], float3 normals[], Ebb::Core::ShaderProgram *shader) {
    this->shader = shader;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);


    this->numTriangles = triCount;

    /*
    INFO LAYOUT:
        |vX| |vY| |vZ|  |nX| |nY| |nZ|  | tX || tY |
        | vertex pos |  |   normal   |  | texcoord |
        |  3 floats  |  |  3 floats  |  | 2 floats |
        |                  8 floats                |
    */
    float meshInfo[vertexCount * 8];

    for (int i = 0; i < vertexCount; ++i) {
        meshInfo[i * 8 + 0] = vertexTable[i].x;
        meshInfo[i * 8 + 1] = vertexTable[i].y;
        meshInfo[i * 8 + 2] = vertexTable[i].z;

        meshInfo[i * 8 + 3] = normals[i].x;
        meshInfo[i * 8 + 4] = normals[i].y;
        meshInfo[i * 8 + 5] = normals[i].z;

        /*
            Unused unless texcoords are specified
        */
        meshInfo[i * 8 + 6] = texCoords[i].x;
        meshInfo[i * 8 + 7] = texCoords[i].y;
    }

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(meshInfo), meshInfo, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triCount * 3 * sizeof(unsigned int), triTable, GL_STATIC_DRAW);

    /*
        Index 0 is vertex position, index 1 is normal, index 2 is texcoord.
    */
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(0 * sizeof(float)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void Ebb::Core::Mesh::draw() {
    this->shader->use();

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 3 * this->numTriangles, GL_UNSIGNED_INT, 0);
}

Ebb::Core::Mesh *Ebb::Core::Mesh::load_ebb_mesh(const char *filename, ShaderProgram *shader) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Failed to open ebb mesh file \"%s\".\n", filename);
        exit(2);
    }
    std::vector<float3> vertices;
    std::vector<float2> texcoords;
    std::vector<float3> normals;

    std::vector<unsigned int> indices;

    char byte;
    bool parsingFaces = false;
    while ((byte = fgetc(file)) != EOF) {
        if (!parsingFaces && (byte == 'f')) {
            parsingFaces = true;
        }
        if (!parsingFaces) {
            if (byte != 'v') {
                fprintf(stderr, "Invalid ebb mesh file \"%s\": expected 'v', got 0x%02x\n", filename, byte);
                exit(2);
            }
            float posX, posY, posZ;
            float texX, texY;
            float normX, normY, normZ;

            size_t status = 0;

            status += fread(&posX, sizeof(posX), 1, file);
            status += fread(&posY, sizeof(posY), 1, file);
            status += fread(&posZ, sizeof(posZ), 1, file);

            status += fread(&texX, sizeof(texX), 1, file);
            status += fread(&texY, sizeof(texY), 1, file);
            
            status += fread(&normX, sizeof(normX), 1, file);
            status += fread(&normY, sizeof(normY), 1, file);
            status += fread(&normZ, sizeof(normZ), 1, file);

            if (status != 8) {
                fprintf(stderr, "Failed reading vertex data from ebb mesh file \"%s\".", filename);
                exit(2);
            }

            vertices.push_back(float3(posX, posY, posZ));
            texcoords.push_back(float2(texX, texY));
            normals.push_back(float3(normX, normY, normZ));
        } else {
            if (byte != 'f') {
                fprintf(stderr, "Invalid ebb mesh file \"%s\": expected 'f', got 0x%02x\n", filename, byte);
                exit(2);
            }
            unsigned int v0, v1, v2;

            size_t status = 0;

            status += fread(&v0, sizeof(v0), 1, file);
            status += fread(&v1, sizeof(v1), 1, file);
            status += fread(&v2, sizeof(v2), 1, file);

            if (status != 3) {
                fprintf(stderr, "Failed reading face data from ebb mesh file \"%s\".", filename);
                exit(2);
            }
            indices.push_back(v0);
            indices.push_back(v1);
            indices.push_back(v2);
        }
    }
    return new Mesh(vertices.size(), &vertices[0], (indices.size() / 3), &indices[0], &texcoords[0], &normals[0], shader);
}