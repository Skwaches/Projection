#ifndef UPDATE
#define UPDATE
#include "init.h"

void vertexes(Vector3* vertices, Vector3 bak, Vector3 dimensions);
Vector3 translate3(Vector3 point, Vector3 translation);
SDL_FPoint translate2(SDL_FPoint point, SDL_FPoint translation);
SDL_FPoint scale2(SDL_FPoint point, float factor);
Vector3 scale3(Vector3 point, float factor);
SDL_FPoint projection(Vector3 point);
SDL_FPoint convertScreen(SDL_FPoint point);
SDL_FPoint rotate2(SDL_FPoint point, SDL_FPoint origin, float angle);
Vector3 rotate3(Vector3 point, Vector3 origin, Vector3 angle);
float timer(void);
void update(void);

void initCuboid(Cuboid* cube);
void updateCuboid(Cuboid* cube);
Vector3 centerCuboid(Cuboid cube);

void initCircle(Circle* circle);
void updateCircle(Circle* circle);

void initSphere(Sphere* sphere);
void updateSphere(Sphere* sphere);
void destroySphere(Sphere* sphere);

void initLINE (Line* line);
SDL_FPoint perpendicularVECTOR (SDL_FPoint vector);

#endif
