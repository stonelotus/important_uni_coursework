#ifndef SPHERE_H
#define SPHERE_H

#include "lab_m1/tema2/helpers/dataStructures.h"

class Sphere {
	float x_center, y_center, z_center,radius;
	glm::mat4 modelMatrix;
public:
	Sphere();
	Sphere(dimensionsTriplet, float);
	~Sphere();
	glm::mat4 getModelMatrix();
	float getRadius();
	void Move(dimensionsTriplet);
	dimensionsTriplet getPosition();
};

#endif