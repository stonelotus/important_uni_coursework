#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "lab_m1/tema2/transform3D.h"
#include "lab_m1/tema2/helpers/maze.cpp"
#include <vector>
#include "core/engine.h"
#include "utils/gl_utils.h"
#include "lab_m1/tema2/transform3D.h"

#define PLAYGROUND_BOX_HITBOX_RADIUS 0.5f
#define BULLETS_LIFE_DISTANCE 10.f
#define ENEMY_SIZE 1.5f
#define BULLET_RADIUS 0.5f
typedef struct dimensionsTriplet {
	float x, y, z;
};


namespace custom_structs {
	typedef struct sphere {
		float x, y, z, radius;
	};
}
#endif