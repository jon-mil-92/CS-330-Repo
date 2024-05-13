#pragma once

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifndef CUBOID_MESH_BUILDER_H
#define CUBOID_MESH_BUILDER_H

using namespace std;

class CuboidMeshBuilder {
public:
    void buildMesh(vector<GLfloat>& vertices, float width, float height, float length);
};

#endif