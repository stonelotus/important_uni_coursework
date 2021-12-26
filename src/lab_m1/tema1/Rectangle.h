#ifndef RECTANGLE_H
#define RECTANGLE_H
#include <vector>
#include "core/engine.h"
#include "utils/gl_utils.h"
#include <iostream>
#include <lab_m1/tema1/object2D.h>


#include <lab_m1/tema1/transform2D.h>
class Rectangle
{
private:
	float x, y;
	float sizeX, sizeY;
	glm::mat3 modelMatrix;
public:

	Rectangle(float, float, float, float);
	~Rectangle();

	float getSizeX();
	float getSizeY();
	float getX();
	float getY();
	void setX(float);
	void setY(float);
	void setModelMatrix(float, float, float, float, float);
	void setModelMatrix(glm::mat3);
	glm::mat3 Rectangle::getModelMatrix();

};

#endif