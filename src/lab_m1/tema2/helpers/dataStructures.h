#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "lab_m1/tema2/transform3D.h"
#include "lab_m1/tema2/helpers/maze.cpp"
#include <vector>
#include "core/engine.h"
#include "utils/gl_utils.h"
#include "lab_m1/tema2/transform3D.h"

#define PLAYGROUND_BOX_HITBOX_RADIUS 0.5f
#define PLAYER_HITBOX_RADIUS 0.6f
#define BULLETS_LIFE_DISTANCE 20.f
#define ENEMY_SIZE 1.f
#define BULLET_RADIUS 0.5f
#define PLAYGROUND_MATRIX_OFFSET 0.5f
#define ENEMY_ROUTES_DEPTH 3

#define ENEMY_PATH_MARKER 3
#define EMPTY_SPACE_MARKER 0
#define WALL_MARKER 1
#define ENEMY_MARKER 2

typedef struct dimensionsTriplet {
	float x, y, z;
};


namespace custom_structs {
	typedef struct sphere {
		float x, y, z, radius;
	};
}
#endif