#ifndef MESH_HPP
#define MESH_HPP

#include <ebb/math/vector.hpp>
#include <ebb/core/shaders/shader.hpp>

using namespace Ebb::Math;

namespace Ebb {
namespace Core {

class Mesh {
public:
    /**
     * Construct a new empty mesh with the given shader
    */
    Mesh(Ebb::Core::ShaderProgram* shader);

    /**
     * Construct a new mesh with the specified vertices, edges, and shader. Texcoords are set to 0. [TODO: auto texture mapping (per-axis?)]
    */
    Mesh(unsigned int vertexCount, float3 vertexTable[], 
         unsigned int triCount, unsigned int edgeTable[],
         Ebb::Core::ShaderProgram *shader);

    /**
     * Construct a new mesh with the specified vertices, edges, texcoords, and shader. Each texcoord corresponds to a vertex.
    */
    Mesh(unsigned int vertexCount, float3 vertexTable[],
         unsigned int triCount, unsigned int edgeTable[],
         float2 texCoords[], Ebb::Core::ShaderProgram *shader);

    /**
     * Draw the mesh to the current context.
    */
    void draw();

private:
    unsigned int VBO;
    unsigned int VAO;
    unsigned int EBO;

    unsigned int numTriangles;

    Ebb::Core::ShaderProgram *shader;
};

}; };

#endif