#ifndef BOX_HEADER
#define BOX_HEADER
#include <vector>
#include "core/engine.h"
#include "utils/gl_utils.h"
#include "lab_m1/tema2/transform3D.h"

typedef struct dimensionsTriplet{
	float x, y, z;
};


class Box {
	float x, y, z;
	float sizeX, sizeY, sizeZ;
	glm::mat4 modelMatrix;
public:
	Box();
	Box(dimensionsTriplet, dimensionsTriplet);
	~Box();
	glm::mat4 getModelMatrix();
	float getX();
	float getY();
	float getZ();
};

#endif