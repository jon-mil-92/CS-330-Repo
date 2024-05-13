#include "CylinderMeshBuilder.h"

// Builds the vertices for the sides of an n-prism mesh (without the bottom and top faces)
// Example: 4 slices results in a rectangular prism, 6 slices results in a hexagonal prism, 60 slices will virtually result in a cylinder
void CylinderMeshBuilder::buildSideMesh(vector<GLfloat>& vertices, int slices, float radius, float height)
{
    // The vertex angle for each triangle slice in radians
    float theta = (360.0 / slices) * (M_PI / 180.0f);

    // Divide the texture into vertical sections corresponding to the number of slices
    float textureSectionLength = 1.0f / slices;

    // Build a rectangle for each side of the prism
    for (int i = 0; i < slices; i++) {
        // Vertex: Position (X, Y, Z) - Normal (nX, nY, nZ) - Texture Coordinate (tX, tY)

        // Side Triangle One
        // -----------------

        // Vertex i: Position - Top
        vertices.push_back(radius * cos(i * theta)), vertices.push_back(height), vertices.push_back(radius * sin(i * theta));

        // Vertex i: Normal - Top
        vertices.push_back(cos(i * theta)), vertices.push_back(0.0f), vertices.push_back(sin(i * theta));

        // Vertex i: Texture Coordinate - Top
        vertices.push_back(i * textureSectionLength), vertices.push_back(1.0f);

        // Vertex i: Position - Bottom
        vertices.push_back(radius * cos(i * theta)), vertices.push_back(0.0f), vertices.push_back(radius * sin(i * theta));

        // Vertex i: Normal - Bottom
        vertices.push_back(cos(i * theta)), vertices.push_back(0.0f), vertices.push_back(sin(i * theta));

        // Vertex i: Texture Coordinate - Bottom
        vertices.push_back(i * textureSectionLength), vertices.push_back(0.0f);

        if (i < slices)
        {
            // Vertex (i + 1): Position - Top
            vertices.push_back(radius * cos((i + 1) * theta)), vertices.push_back(height), vertices.push_back(radius * sin((i + 1) * theta));

            // Vertex (i + 1): Normal - Top
            vertices.push_back(cos((i + 1) * theta)), vertices.push_back(0.0f), vertices.push_back(sin((i + 1) * theta));

            // Vertex (i + 1): Texture Coordinate - Top
            vertices.push_back((i + 1) * textureSectionLength), vertices.push_back(1.0f);
        }
        else
        {
            // Vertex 0: Position - Top
            vertices.push_back(radius * cos(0.0f)), vertices.push_back(height), vertices.push_back(radius * sin(0.0f));

            // Vertex 0: Normal - Top
            vertices.push_back(cos(0.0f)), vertices.push_back(0.0f), vertices.push_back(sin(0.0f));

            // Vertex 0: Texture Coordinate - Top
            vertices.push_back(0.0f), vertices.push_back(1.0f);
        }

        // Side Triangle Two
        // -----------------

        // Vertex i: Position - Bottom
        vertices.push_back(radius * cos(i * theta)), vertices.push_back(0.0f), vertices.push_back(radius * sin(i * theta));

        // Vertex i: Normal - Bottom
        vertices.push_back(cos(i * theta)), vertices.push_back(0.0f), vertices.push_back(sin(i * theta));

        // Vertex i: Texture Coordinate - Bottom
        vertices.push_back(i * textureSectionLength), vertices.push_back(0.0f);

        if (i < slices)
        {
            // Vertex (i + 1): Position - Top
            vertices.push_back(radius * cos((i + 1) * theta)), vertices.push_back(height), vertices.push_back(radius * sin((i + 1) * theta));

            // Vertex (i + 1): Normal - Top
            vertices.push_back(cos((i + 1) * theta)), vertices.push_back(0.0f), vertices.push_back(sin((i + 1) * theta));

            // Vertex (i + 1): Texture Coordinate - Top
            vertices.push_back((i + 1) * textureSectionLength), vertices.push_back(1.0f);
        }
        else
        {
            // Vertex 0: Position - Top
            vertices.push_back(radius * cos(0.0f)), vertices.push_back(height), vertices.push_back(radius * sin(0.0f));

            // Vertex 0: Normal - Top
            vertices.push_back(cos(0.0f)), vertices.push_back(0.0f), vertices.push_back(sin(0.0f));

            // Vertex 0: Texture Coordinate - Top
            vertices.push_back(0.0f), vertices.push_back(1.0f);
        }

        if (i < slices)
        {
            // Vertex (i + 1): Position - Bottom
            vertices.push_back(radius * cos((i + 1) * theta)), vertices.push_back(0.0f), vertices.push_back(radius * sin((i + 1) * theta));

            // Vertex (i + 1): Normal - Bottom
            vertices.push_back(cos((i + 1) * theta)), vertices.push_back(0.0f), vertices.push_back(sin((i + 1) * theta));

            // Vertex (i + 1): Texture Coordinate - Bottom
            vertices.push_back((i + 1) * textureSectionLength), vertices.push_back(0.0f);
        }
        else
        {
            // Vertex 0: Position - Bottom
            vertices.push_back(radius * cos(0.0f)), vertices.push_back(0.0f), vertices.push_back(radius * sin(0.0f));

            // Vertex 0: Tint - Bottom
            vertices.push_back(cos(0.0f)), vertices.push_back(0.0f), vertices.push_back(sin(0.0f));

            // Vertex 0: Texture Coordinate - Bottom
            vertices.push_back(0.0f), vertices.push_back(0.0f);
        }
    }
}

// Builds the vertices for an n-sided polygon mesh; the number of sides corresponds to the number of slices
// For example: 4 slices is a square, 6 slices is a pentagon, and 60 slices is virtually a circle
void CylinderMeshBuilder::buildFaceMesh(vector<GLfloat>& vertices, bool isTopFace, int slices, float radius)
{
    // The vertex angle for each triangle slice in radians
    float theta = (360.0 / slices) * (M_PI / 180.0f);

    // The middle of the texture is at (0.5, 0.5), and the radius of the circle cutout from the texture is 0.5
    float textureRadius = 0.5f;

    // Build a rectangle for each side of the prism
    for (int i = 0; i < slices; i++) {
        // Vertex: Position (X, Y, Z) - Normal (nX, nY, nZ) - Texture Coordinate (tX, tY)

        // Vertex i: Position
        vertices.push_back(radius * cos(i * theta)), vertices.push_back(0.0f), vertices.push_back(radius * sin(i * theta));

        // Vertex i: Normal
        if (isTopFace)
            vertices.push_back(0.0f), vertices.push_back(1.0f), vertices.push_back(0.0f);  // +Y
        else
            vertices.push_back(0.0f), vertices.push_back(-1.0f), vertices.push_back(0.0f); // -Y

        // Vertex i: Texture Coordinate
        vertices.push_back(textureRadius * cos(theta * i) + textureRadius), vertices.push_back(textureRadius * sin(theta * i) + textureRadius);

        if (i < slices)
        {
            // Vertex (i + 1): Position
            vertices.push_back(radius * cos((i + 1) * theta)), vertices.push_back(0.0f), vertices.push_back(radius * sin((i + 1) * theta));

            // Vertex (i + 1): Normal
            if (isTopFace)
                vertices.push_back(0.0f), vertices.push_back(1.0f), vertices.push_back(0.0f);  // +Y
            else
                vertices.push_back(0.0f), vertices.push_back(-1.0f), vertices.push_back(0.0f); // -Y

            // Vertex (i + 1): Texture Coordinate
            vertices.push_back(textureRadius * cos(theta * (i + 1)) + textureRadius), vertices.push_back(textureRadius * sin(theta * (i + 1)) + textureRadius);
        }
        else
        {
            // Vertex 0: Position
            vertices.push_back(radius * cos(0.0f)), vertices.push_back(0.0f), vertices.push_back(radius * sin(0.0f));

            // Vertex 0: Normal
            if (isTopFace)
                vertices.push_back(0.0f), vertices.push_back(1.0f), vertices.push_back(0.0f);  // +Y
            else
                vertices.push_back(0.0f), vertices.push_back(-1.0f), vertices.push_back(0.0f); // -Y

            // Vertex 0: Texture Coordinate
            vertices.push_back(0.0f), vertices.push_back(1.0f);
        }

        // Center Vertex: Postion - Origin
        vertices.push_back(0.0f), vertices.push_back(0.0f), vertices.push_back(0.0f);

        // Center Vertex: Normal
        if (isTopFace)
            vertices.push_back(0.0f), vertices.push_back(1.0f), vertices.push_back(0.0f);  // +Y
        else
            vertices.push_back(0.0f), vertices.push_back(-1.0f), vertices.push_back(0.0f); // -Y

        // Center Vertex: Texture Coordinate - Center of texture
        vertices.push_back(0.5f), vertices.push_back(0.5f);
    }
}