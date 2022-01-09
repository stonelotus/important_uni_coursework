#ifndef BOX_HEADER
#define BOX_HEADER

#include "lab_m1/tema2/helpers/dataStructures.h"
#include <string>
class Box {
	float x, y, z;
	float sizeX, sizeY, sizeZ;
	glm::vec3 color;
	glm::mat4 modelMatrix;
	float angle;
public:
	Box();
	Box(dimensionsTriplet, dimensionsTriplet);
	~Box();
	glm::mat4 getModelMatrix();
	dimensionsTriplet getPosition();
	void setPosition(float, float, float);
	void Move(float, float, float);
	glm::vec3 getShaderColor();
	void setShaderColor(glm::vec3);
	void setAngle(float);
	void Rotate(float);
};

#endif