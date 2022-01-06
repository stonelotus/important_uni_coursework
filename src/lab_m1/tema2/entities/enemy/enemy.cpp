#include "enemy.h"

Enemy::Enemy(){}
Enemy::Enemy(dimensionsTriplet position, dimensionsTriplet size):x(position.x), y(position.y), z(position.z),
																 sizeX(position.x), sizeY(size.y), sizeZ(size.z) {
	body = Sphere(position, ENEMY_SIZE);
	left_eye = Sphere({ position.x + 0.4f,position.y + 0.36f,position.z - 0.4f }, ENEMY_SIZE/5);
	right_eye = Sphere({ position.x + 0.4f,position.y + 0.36f,position.z + 0.4f }, ENEMY_SIZE/5);

};
Enemy::~Enemy() {};