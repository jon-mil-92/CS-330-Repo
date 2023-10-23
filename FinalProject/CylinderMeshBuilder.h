#pragma once

#define _USE_MATH_DEFINES

#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifndef CYLINDER_MESH_BUILDER_H
#define CYLINDER_MESH_BUILDER_H

using namespace std;

class CylinderMeshBuilder {
    public:
        void buildSideMesh(vector<GLfloat>& vertices, int slices, float radius, float height);
        void buildFaceMesh(vector<GLfloat>& vertices, bool isTopFace, int slices, float radius);
};

#endif