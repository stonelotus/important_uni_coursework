#ifndef BULLET_H
#define BULLET_H
#include "lab_m1/tema2/helpers/dataStructures.h"
#include "lab_m1/tema2/entities/sphere/sphere.h"
class Bullet : public Sphere {
	float speed, remaining_distance;
public: 
	Bullet();
	Bullet(dimensionsTriplet, float);
	~Bullet();
	float getRemainingDistance();
	void ModifyRemainingDistance(float);
};
#endif