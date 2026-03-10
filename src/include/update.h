#ifndef UPDATE 
#define UPDATE
#include "classes.h"

void vertexes(Vector3* vertices, Vector3 bak, Vector3 dimensions);

//Transformations
Vector3 translate3(Vector3 point, Vector3 translation);
SDL_FPoint translate2(SDL_FPoint point, SDL_FPoint translation);
SDL_FPoint scale2(SDL_FPoint point, float factor);
Vector3 scale3(Vector3 point, float factor);
SDL_FPoint projection(Vector3 point);
SDL_FPoint convertScreen(SDL_FPoint point);
SDL_FPoint rotate2(SDL_FPoint point, SDL_FPoint origin, float angle);
Vector3 rotate3(Vector3 point, Vector3 origin, SDL_FPoint angle);
SDL_FPoint perpendicular2(SDL_FPoint vector);

float timer(void);
void update(void);

//Cuboids
void initCuboid(Cuboid* cube);
void updateCuboid(Cuboid* cube, Vector3 *translation, Vector3* origin, SDL_FPoint* angle);
Vector3 centerCuboid(Cuboid cube);

//Circle
void initCircle(Circle* circle);

//Spheres
void initSphere(Sphere* sphere);
void updateSphere(Sphere* sphere, Vector3* translation, SDL_FPoint* angle);
void destroySphere(Sphere* sphere);

//GPU calculations

#ifdef __cplusplus
extern "C" {
#endif
	

//Transformations
void translate3_Shader(Vector3* points, Vector3* translation, Vector3* results, int size, bool unique);
void translate2_Shader(SDL_FPoint* points, SDL_FPoint translation, int size);

//Spheres
void updateSphere_Shader(Sphere* sphere, Vector3 translation, SDL_FPoint angle);


#ifdef __cplusplus
}
#endif 



#endif
