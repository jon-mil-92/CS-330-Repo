#include "PlaneMeshBuilder.h"

// Builds the vertices for a plane mesh centered around the origin on the XZ coordinate plane
void PlaneMeshBuilder::buildMesh(vector<GLfloat>& vertices, float length, float width)
{
    // Vertex: Position (X, Y, Z) - Tint (R, G, B, A) - Texture Coordinate (tX, tY)

    // Vertex 0: Position - Back left
    vertices.push_back(-width / 2.0f), vertices.push_back(0.0f), vertices.push_back(-length / 2.0f);

    // Vertex 0: Normal (+Y)
    vertices.push_back(0.0f), vertices.push_back(1.0f), vertices.push_back(0.0f);

    // Vertex 0: Texture Coordinate - Top left of image
    vertices.push_back(0.0f), vertices.push_back(1.0f);

    // Vertex 1: Position - Back right
    vertices.push_back(width / 2.0f), vertices.push_back(0.0f), vertices.push_back(-length / 2.0f);

    // Vertex 1: Normal (+Y)
    vertices.push_back(0.0f), vertices.push_back(1.0f), vertices.push_back(0.0f);

    // Vertex 1: Texture Coordinate - Top right of image
    vertices.push_back(1.0f), vertices.push_back(1.0f);

    // Vertex 2: Position - Front left
    vertices.push_back(-width / 2.0f), vertices.push_back(0.0f), vertices.push_back(length / 2.0f);

    // Vertex 2: Normal (+Y)
    vertices.push_back(0.0f), vertices.push_back(1.0f), vertices.push_back(0.0f);

    // Vertex 2: Texture Coordinate - Bottom left of image
    vertices.push_back(0.0f), vertices.push_back(0.0f);

    // Vertex 3: Position - Back right
    vertices.push_back(width / 2.0f), vertices.push_back(0.0f), vertices.push_back(-length / 2.0f);

    // Vertex 3: Normal (+Y)
    vertices.push_back(0.0f), vertices.push_back(1.0f), vertices.push_back(0.0f);

    // Vertex 3: Texture Coordinate - Top right of image
    vertices.push_back(1.0f), vertices.push_back(1.0f);

    // Vertex 4: Position - Front left
    vertices.push_back(-width / 2.0f), vertices.push_back(0.0f), vertices.push_back(length / 2.0f);

    // Vertex 4: Normal (+Y)
    vertices.push_back(0.0f), vertices.push_back(1.0f), vertices.push_back(0.0f);

    // Vertex 4: Texture Coordinate - Bottom left of image
    vertices.push_back(0.0f), vertices.push_back(0.0f);

    // Vertex 5: Position - Front right
    vertices.push_back(width / 2.0f), vertices.push_back(0.0f), vertices.push_back(length / 2.0f);

    // Vertex 5: Normal (+Y)
    vertices.push_back(0.0f), vertices.push_back(1.0f), vertices.push_back(0.0f);

    // Vertex 5: Texture Coordinate - Bottom right of image
    vertices.push_back(1.0f), vertices.push_back(0.0f);
}