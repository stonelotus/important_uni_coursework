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
	glm::mat4 modelMatrix;
public: 
	Player(float, float, float);
	Player();
	~Player();
	glm::mat4 getModelMatrix();
	Box body,head,left_leg,right_leg;
};
#endif