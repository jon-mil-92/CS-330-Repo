#include "CuboidMeshBuilder.h"

// Builds the vertices for a cuboid mesh
// Constraints: length + width <= 1 and (2 * width) + (2 * height) <= 1
void CuboidMeshBuilder::buildMesh(vector<GLfloat>& vertices, float width, float height, float length)
{
    // Front Face: Bottom Triangle
    // ------------------------

    // Vertex - Position - Bottom left
    vertices.push_back(0.0f), vertices.push_back(0.0f), vertices.push_back(0.0f);
    // Vertex - Normal (+Z)
    vertices.push_back(0.0f), vertices.push_back(0.0f), vertices.push_back(1.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length), vertices.push_back(0.0f);

    // Vertex - Position - Top Left
    vertices.push_back(0.0f), vertices.push_back(height), vertices.push_back(0.0f);
    // Vertex - Normal (+Z)
    vertices.push_back(0.0f), vertices.push_back(0.0f), vertices.push_back(1.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length), vertices.push_back(height);

    // Vertex - Position - Bottom right
    vertices.push_back(width), vertices.push_back(0.0f), vertices.push_back(0.0f);
    // Vertex - Normal (+Z)
    vertices.push_back(0.0f), vertices.push_back(0.0f), vertices.push_back(1.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length + width), vertices.push_back(0.0f);

    // Front Face: Top Triangle
    // ---------------------------

    // Vertex - Position - Bottom right
    vertices.push_back(width), vertices.push_back(0.0f), vertices.push_back(0.0f);
    // Vertex - Normal (-Z)
    vertices.push_back(0.0f), vertices.push_back(0.0f), vertices.push_back(1.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length + width), vertices.push_back(0.0f);

    // Vertex - Position -  Top right
    vertices.push_back(width), vertices.push_back(height), vertices.push_back(0.0f);
    // Vertex - Normal (-Z)
    vertices.push_back(0.0f), vertices.push_back(0.0f), vertices.push_back(1.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length + width), vertices.push_back(height);

    // Vertex - Position - Top Left
    vertices.push_back(0.0f), vertices.push_back(height), vertices.push_back(0.0f);
    // Vertex - Normal (+Z)
    vertices.push_back(0.0f), vertices.push_back(0.0f), vertices.push_back(1.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length), vertices.push_back(height);

    // Back Face: Bottom Triangle
    // -----------------------

    // Vertex - Position - Bottom left
    vertices.push_back(0.0f), vertices.push_back(0.0f), vertices.push_back(-length);
    // Vertex - Normal (-Z)
    vertices.push_back(0.0f), vertices.push_back(0.0f), vertices.push_back(-1.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length + width), vertices.push_back(height);

    // Vertex - Position - Top Left
    vertices.push_back(0.0f), vertices.push_back(height), vertices.push_back(-length);
    // Vertex - Normal (-Z)
    vertices.push_back(0.0f), vertices.push_back(0.0f), vertices.push_back(-1.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length + width), vertices.push_back(2 * height);

    // Vertex - Position - Bottom right
    vertices.push_back(width), vertices.push_back(0.0f), vertices.push_back(-length);
    // Vertex - Normal (-Z)
    vertices.push_back(0.0f), vertices.push_back(0.0f), vertices.push_back(-1.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length), vertices.push_back(height);

    // Back Face: Top Triangle
    // --------------------------

    // Vertex - Position - Bottom right
    vertices.push_back(width), vertices.push_back(0.0f), vertices.push_back(-length);
    // Vertex - Normal (-Z)
    vertices.push_back(0.0f), vertices.push_back(0.0f), vertices.push_back(-1.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length), vertices.push_back(height);

    // Vertex - Position -  Top right
    vertices.push_back(width), vertices.push_back(height), vertices.push_back(-length);
    // Vertex - Normal (-Z)
    vertices.push_back(0.0f), vertices.push_back(0.0f), vertices.push_back(-1.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length), vertices.push_back(2 * height);

    // Vertex - Position - Top Left
    vertices.push_back(0.0f), vertices.push_back(height), vertices.push_back(-length);
    // Vertex - Normal (-Z)
    vertices.push_back(0.0f), vertices.push_back(0.0f), vertices.push_back(-1.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length + width), vertices.push_back(2 * height);

    // Left Face: Bottom Triangle
    // --------------------------

    // Vertex - Position - Front bottom
    vertices.push_back(0.0f), vertices.push_back(0.0f), vertices.push_back(0.0f);
    // Vertex - Normal (-X)
    vertices.push_back(-1.0f), vertices.push_back(0.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length), vertices.push_back(0.0f);

    // Vertex - Position - Front top
    vertices.push_back(0.0f), vertices.push_back(height), vertices.push_back(0.0f);
    // Vertex - Normal (-X)
    vertices.push_back(-1.0f), vertices.push_back(0.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length), vertices.push_back(height);

    // Vertex - Position - Back bottom
    vertices.push_back(0.0f), vertices.push_back(0.0f), vertices.push_back(-length);
    // Vertex - Normal (-X)
    vertices.push_back(-1.0f), vertices.push_back(0.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(0.0f), vertices.push_back(0.0f);

    // Left Face: Top Triangle
    // -----------------------

    // Vertex - Position - Back bottom
    vertices.push_back(0.0f), vertices.push_back(0.0f), vertices.push_back(-length);
    // Vertex - Normal (-X)
    vertices.push_back(-1.0f), vertices.push_back(0.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(0.0f), vertices.push_back(0.0f);

    // Vertex - Position - Back top
    vertices.push_back(0.0f), vertices.push_back(height), vertices.push_back(-length);
    // Vertex - Normal (-X)
    vertices.push_back(-1.0f), vertices.push_back(0.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(0.0f), vertices.push_back(height);

    // Vertex - Position - Front top
    vertices.push_back(0.0f), vertices.push_back(height), vertices.push_back(0.0f);
    // Vertex - Normal (-X)
    vertices.push_back(-1.0f), vertices.push_back(0.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length), vertices.push_back(height);

    // Right Face: Bottom Triangle
    // ---------------------------

    // Vertex - Position - Front bottom
    vertices.push_back(width), vertices.push_back(0.0f), vertices.push_back(0.0f);
    // Vertex - Normal (+X)
    vertices.push_back(1.0f), vertices.push_back(0.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(0.0f), vertices.push_back(height);

    // Vertex - Position - Front top
    vertices.push_back(width), vertices.push_back(height), vertices.push_back(0.0f);
    /// Vertex - Normal (+X)
    vertices.push_back(1.0f), vertices.push_back(0.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(0.0f), vertices.push_back(2 * height);

    // Vertex - Position - Back bottom
    vertices.push_back(width), vertices.push_back(0.0f), vertices.push_back(-length);
    // Vertex - Normal (+X)
    vertices.push_back(1.0f), vertices.push_back(0.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length), vertices.push_back(height);

    // Right Face: Top Triangle
    // ------------------------

    // Vertex - Position - Back bottom
    vertices.push_back(width), vertices.push_back(0.0f), vertices.push_back(-length);
    // Vertex - Normal (+X)
    vertices.push_back(1.0f), vertices.push_back(0.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length), vertices.push_back(height);

    // Vertex - Position - Back top
    vertices.push_back(width), vertices.push_back(height), vertices.push_back(-length);
    // Vertex - Normal (+X)
    vertices.push_back(1.0f), vertices.push_back(0.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length), vertices.push_back(2 * height);

    // Vertex - Position - Front top
    vertices.push_back(width), vertices.push_back(height), vertices.push_back(0.0f);
    /// Vertex - Normal (+X)
    vertices.push_back(1.0f), vertices.push_back(0.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(0.0f), vertices.push_back(2 * height);

    // Top Face: Front Triangle
    // ----------------------------

    // Vertex - Position - Front left
    vertices.push_back(0.0f), vertices.push_back(height), vertices.push_back(0.0f);
    // Vertex - Normal (+Y)
    vertices.push_back(0.0f), vertices.push_back(1.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length), vertices.push_back(2 * height);

    // Vertex - Position - Front right
    vertices.push_back(width), vertices.push_back(height), vertices.push_back(0.0f);
    // Vertex - Normal (+Y)
    vertices.push_back(0.0f), vertices.push_back(1.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length), vertices.push_back(2 * height + width);

    // Vertex - Position - Back left
    vertices.push_back(0.0f), vertices.push_back(height), vertices.push_back(-length);
    // Vertex - Normal (+Y)
    vertices.push_back(0.0f), vertices.push_back(1.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(0.0f), vertices.push_back(2 * height);

    // Top Face: Back Triangle
    // -------------------------

    // Vertex - Position - Back left
    vertices.push_back(0.0f), vertices.push_back(height), vertices.push_back(-length);
    // Vertex - Normal (+Y)
    vertices.push_back(0.0f), vertices.push_back(1.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(0.0f), vertices.push_back(2 * height);

    // Vertex - Position - Back right
    vertices.push_back(width), vertices.push_back(height), vertices.push_back(-length);
    // Vertex - Normal (+Y)
    vertices.push_back(0.0f), vertices.push_back(1.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(0.0f), vertices.push_back(2 * height + width);

    // Vertex - Position - Front right
    vertices.push_back(width), vertices.push_back(height), vertices.push_back(0.0f);
    // Vertex - Normal (+Y)
    vertices.push_back(0.0f), vertices.push_back(1.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length), vertices.push_back(2 * height + width);

    // Bottom Face: Front Triangle
    // ----------------------------

    // Vertex - Position - Front left
    vertices.push_back(0.0f), vertices.push_back(0.0f), vertices.push_back(0.0f);
    // Vertex - Normal (-Y)
    vertices.push_back(0.0f), vertices.push_back(-1.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length), vertices.push_back(2 * height + 2 * width);

    // Vertex - Position - Front right
    vertices.push_back(width), vertices.push_back(0.0f), vertices.push_back(0.0f);
    // Vertex - Normal (-Y)
    vertices.push_back(0.0f), vertices.push_back(-1.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length), vertices.push_back(2 * height + width);

    // Vertex - Position - Back left
    vertices.push_back(0.0f), vertices.push_back(0.0f), vertices.push_back(-length);
    // Vertex - Normal (-Y)
    vertices.push_back(0.0f), vertices.push_back(-1.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(0.0f), vertices.push_back(2 * height + 2 * width);

    // Bottom Face: Back Triangle
    // -------------------------

    // Vertex - Position - Back left
    vertices.push_back(0.0f), vertices.push_back(0.0f), vertices.push_back(-length);
    // Vertex - Normal (-Y)
    vertices.push_back(0.0f), vertices.push_back(-1.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(0.0f), vertices.push_back(2 * height + 2 * width);

    // Vertex - Position - Back right
    vertices.push_back(width), vertices.push_back(0.0f), vertices.push_back(-length);
    // Vertex - Normal (-Y)
    vertices.push_back(0.0f), vertices.push_back(-1.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(0.0f), vertices.push_back(2 * height + width);

    // Vertex - Position - Front right
    vertices.push_back(width), vertices.push_back(0.0f), vertices.push_back(0.0f);
    // Vertex - Normal (-Y)
    vertices.push_back(0.0f), vertices.push_back(-1.0f), vertices.push_back(0.0f);
    // Vertex - Texture Coordinate
    vertices.push_back(length), vertices.push_back(2 * height + width);
}