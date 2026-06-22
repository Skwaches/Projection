#include "update.h"
#include "init.h"

/*
 * Kindly note that this function adds the point of a circle to the GLOBAL_POINTS array, every time it is called.
 * It modifies the value of GLOBAL_FREE_INDEX!
*/
void initCirclePoints(Vector3 center, float radius, int accuracy){
	float angle = 2*SDL_PI_F/accuracy;
	float currentAngle = 0;
	for(int i = 0; i < accuracy; i++){
		Vector3 translation = {SDL_cosf(currentAngle), 0, SDL_sinf(currentAngle)};
		translation = scale3(translation, radius);
		GLOBAL_POINTS[i + GLOBAL_FREE_INDEX] = translate3(center,translation);
		GLOBAL_OBJECT_MAPPING[ i + GLOBAL_FREE_INDEX ] = GLOBAL_FREE_OBJECT_INDEX;
		currentAngle += angle;
	}
	GLOBAL_FREE_INDEX+= accuracy;
}
	
void initCircle(Circle* circle){
	if(!circle)
		SDL_Log("Error: Circle object passed is NULL!");
	SPACE_CHECK(circle->accuracy);
	if(!RUNNING)
		return;

	circle->points = &GLOBAL_POINTS[GLOBAL_FREE_INDEX];
	circle->projections = &GLOBAL_PROJECTIONS[GLOBAL_FREE_INDEX];
	circle->id = GLOBAL_FREE_OBJECT_INDEX;

	GLOBAL_OBJECTS[GLOBAL_FREE_OBJECT_INDEX]= (ObjectProperties){
		.color = circle->color,
		.translation = {0, 0, 0},
		.rotation = circle->rotation,
		.origin = circle->origin,
	};

	circle->points[0] = circle->center;
	GLOBAL_OBJECT_MAPPING[GLOBAL_FREE_INDEX++] = GLOBAL_FREE_OBJECT_INDEX;
	initCirclePoints(circle->center, circle->radius, circle->accuracy -1);
	GLOBAL_FREE_OBJECT_INDEX++;

}
/* Highest point and Lowest point are not counted in accuracy.
 * Concentric circles will be drawn top to bottom.
 * Equator is forced
 * accuracy.x are the concentric circles in one hemisphere
 * Concentric circle start from the highest y value downwards
*/
void initSphere(Sphere* sphere){
	if(!sphere){
		SDL_Log("Error: Sphere object passed is NULL!");
		return;
	}
	int concentricMAX =  sphere->accuracy.x * 2 + 1;
 	//Number of point the sphere will have
 	sphere->count = concentricMAX * sphere->accuracy.y + 2;
	SPACE_CHECK(sphere->count);
	if(!RUNNING)
		return;

	GLOBAL_OBJECTS[GLOBAL_FREE_OBJECT_INDEX]= (ObjectProperties){
		.color = sphere->color,
		.translation = {0, 0, 0},
		.rotation = {0, 0},
		.origin = sphere->center
	};
	
	//Assign the sphere's pointer the current end point of the list.
	sphere->points = &GLOBAL_POINTS[GLOBAL_FREE_INDEX];
	sphere->projections = &GLOBAL_PROJECTIONS[GLOBAL_FREE_INDEX];

	for(int i = 0; i < sphere->count; i++){
		sphere->projections[i].color = sphere->color;
	}

	sphere->points[0] = (Vector3){sphere->center.x, sphere->center.y + sphere->radius, sphere->center.z}; //Top pole
	GLOBAL_OBJECT_MAPPING[GLOBAL_FREE_INDEX++] = 0;

	float angle_Delta = (SDL_PI_F/2) / (sphere->accuracy.x + 1); //Uniform angle difference
	float angle = SDL_PI_F/2 - angle_Delta;

	//This counts the concentric circles
	//Keeps track of index. 0 is the highest concentric circle. 
	int concentricCircleIndex = 0;  

	while( concentricCircleIndex < concentricMAX){
		float height = sphere->radius * SDL_sinf(angle);
	    float radius = sphere->radius * SDL_cosf(angle);

		Vector3 translation = {0, height, 0};
		Vector3 center = translate3(sphere->center, translation);
		initCirclePoints(center, radius, sphere->accuracy.y);
		angle-=angle_Delta;
		concentricCircleIndex++;
	}

	sphere->points[GLOBAL_FREE_INDEX] = (Vector3){sphere->center.x, sphere->center.y - sphere->radius, sphere->center.z}; //Bottom pole
	GLOBAL_OBJECT_MAPPING[GLOBAL_FREE_INDEX++] = GLOBAL_FREE_OBJECT_INDEX;
	++GLOBAL_FREE_OBJECT_INDEX;
}


