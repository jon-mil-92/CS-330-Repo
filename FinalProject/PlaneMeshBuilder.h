#pragma once

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifndef PLANE_MESH_BUILDER_H
#define PLANE_MESH_BUILDER_H

using namespace std;

class PlaneMeshBuilder {
public:
    void buildMesh(vector<GLfloat>& vertices, float length, float width);
};

#endif
