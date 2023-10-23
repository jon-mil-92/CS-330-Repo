#include "SphereMeshBuilder.h"

// Builds the vertices and indices for a sphere mesh; the higher the segments, the more round the sphere is
// Algorithm from the following source: https://learnopengl.com/code_viewer_gh.php?code=src/6.pbr/1.2.lighting_textured/lighting_textured.cpp
void SphereMeshBuilder::buildMesh(vector<GLfloat>& vertices, vector<GLushort>& indices, int segments)
{
    const int X_SEGMENTS = segments;
    const int Y_SEGMENTS = segments;

    // Generate vertices, normals, and texture coordinates for a sphere with the given amount of segments
    for (int x = 0; x <= X_SEGMENTS; ++x)
    {
        for (int y = 0; y <= Y_SEGMENTS; ++y)
        {
            float xSegment = (float)x / (float)X_SEGMENTS;
            float ySegment = (float)y / (float)Y_SEGMENTS;
            float xPos = cos(xSegment * 2.0f * M_PI) * sin(ySegment * M_PI);
            float yPos = cos(ySegment * M_PI);
            float zPos = sin(xSegment * 2.0f * M_PI) * sin(ySegment * M_PI);

            // Vertex: Position (X, Y, Z) - Normal (nX, nY, nZ) - Texture Coordinate (tX, tY)

            // Position coordinates
            vertices.push_back(xPos), vertices.push_back(yPos), vertices.push_back(zPos);

            // Normal vector
            vertices.push_back(xPos), vertices.push_back(yPos), vertices.push_back(zPos);

            // Texture coordinates
            vertices.push_back(xSegment), vertices.push_back(ySegment);
        }
    }

    bool oddRow = false;

    // Generate indices for the vertices of the sphere with the given amount of segments
    for (int y = 0; y < Y_SEGMENTS; ++y)
    {
        if (!oddRow)
        {
            for (int x = 0; x <= X_SEGMENTS; ++x)
            {
                indices.push_back(y * (X_SEGMENTS + 1) + x);
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
            }
        }
        else
        {
            for (int x = X_SEGMENTS; x >= 0; --x)
            {
                indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                indices.push_back(y * (X_SEGMENTS + 1) + x);
            }
        }

        oddRow = !oddRow;
    }
}