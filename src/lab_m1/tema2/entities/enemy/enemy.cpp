#include "enemy.h"

Enemy::Enemy(){}
Enemy::Enemy(dimensionsTriplet position, dimensionsTriplet size):x(position.x), y(position.y), z(position.z),
																 sizeX(position.x), sizeY(size.y), sizeZ(size.z) {
	body = Sphere(position, ENEMY_SIZE);
	left_eye = Sphere({ position.x + ENEMY_SIZE * 0.42f,position.y + ENEMY_SIZE*0.2f,position.z - ENEMY_SIZE*0.3f }, ENEMY_SIZE/5);
	right_eye = Sphere({ position.x + ENEMY_SIZE * 0.42f,position.y + ENEMY_SIZE*0.2f,position.z + ENEMY_SIZE*0.3f }, ENEMY_SIZE/5);

};
Enemy::~Enemy() {};