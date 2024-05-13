#pragma once

#define _USE_MATH_DEFINES

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifndef SPHERE_MESH_BUILDER_H
#define SPHERE_MESH_BUILDER_H

using namespace std;

class SphereMeshBuilder {
public:
    void buildMesh(vector<GLfloat>& vertices, vector<GLushort>& indices, int segments);
};

#endif