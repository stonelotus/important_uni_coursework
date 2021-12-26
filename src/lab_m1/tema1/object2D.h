#ifndef OBJ_2D_H
#define OBJ_2D_H

#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D
{
    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
    Mesh* createCircle(float radiusX, float radiusY, const char* name, glm::vec3 color,bool fill=false);
}

#endif