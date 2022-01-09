#ifndef PLAYER_HEADER
#define PLAYER_HEADER
#include <vector>
#include "core/engine.h"
#include "utils/gl_utils.h"
#include "lab_m1/tema2/transform3D.h"
#include "lab_m1/tema2/entities/box/box.h"

class Player {
	
private:
	float x, y, z;
	dimensionsTriplet last_position;
	float health;
	float angle;

public: 
	Player(float, float, float);
	Player();
	~Player();
	void Move(float, float, float);
	void setPosition(dimensionsTriplet);
	void setLastPosition();
	float getHealth();
	void modifyHealth(float);
	//void SaveCurrentPosition();
	dimensionsTriplet getPosition();
	dimensionsTriplet getLastMove();
	Box body, head, left_leg, right_leg, right_arm, left_arm, left_palm, right_palm;
	void setAngle(float);
};
#endif