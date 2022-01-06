#ifndef ENEMEY_HEADER
#define ENEMY_HEADER

#include "lab_m1/tema2/helpers/dataStructures.h"
#include "lab_m1/tema2/entities/sphere/sphere.h"

class Enemy {
	float x, y, z, sizeX, sizeY, sizeZ;
public:
	Sphere body, left_eye, right_eye;
	
	Enemy();
	Enemy(dimensionsTriplet, dimensionsTriplet);
	~Enemy();
	
};
#endif