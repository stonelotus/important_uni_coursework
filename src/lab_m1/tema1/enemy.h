#ifndef ENEMY_H
#define ENEMY_H
#include <vector>
#include "core/engine.h"
#include "utils/gl_utils.h"
#include <iostream>
#include <lab_m1/tema1/object2D.h>
#include <lab_m1/tema1/transform2D.h>
#include "Rectangle.h"

#define ENEMY_BODY_LENGTH 130.f
#define ENEMY_EYE_LENGTH 50.f
class Enemy {
public:
	Rectangle body;
	Rectangle eye1, eye2;
	float x, y;
	float scale;
	float rotationAngle;
	glm::mat3 modelMatrix;
	float speed;

	Enemy(float, float, float);
	~Enemy();

	void setX(float);
	void setY(float);
	void setRotationAngle(float);
	float getX();
	float getY();
	float getRotationAngle();
	float getSpeed();
	//
	void setModelMatrix(float, float, float);
	glm::mat3 getModelMatrix();
}; 

#endif