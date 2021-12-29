#ifndef BOX_HEADER
#define BOX_HEADER

#include "lab_m1/tema2/helpers/dataStructures.h"
class Box {
	float x, y, z;
	float sizeX, sizeY, sizeZ;
	glm::mat4 modelMatrix;
public:
	Box();
	Box(dimensionsTriplet, dimensionsTriplet);
	~Box();
	glm::mat4 getModelMatrix();
	dimensionsTriplet getPosition();
	void setPosition(float, float, float);
};

#endif