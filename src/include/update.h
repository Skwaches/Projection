#ifndef UPDATE
#define UPDATE
#include "init.h"

void vertexes(Vector3* vertices, Vector3 bak, Vector3 dimensions);
Vector3 translate(Vector3 point, Vector3 translation);
void linePath(Vector3 path[CUBOID_PATH],Vector3 vertices[CUBOID_VERTICES]);
SDL_FPoint projection(Vector3 point);
SDL_FPoint convertScreen(SDL_FPoint point);
float timer(void);
void update(void);
void initCuboid(Cuboid* cube);
void updateCuboid(Cuboid* cube);
#endif
