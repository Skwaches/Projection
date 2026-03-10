#include "update.h"
#include "init.h"

//Allocates memory. Remember to free!
void initCircle(Circle* circle){
	if(!circle){
		SDL_Log("Circle object passed to be initialised is NULL!");
		return;
	}
	circle->points = &GLOBAL_POINTS[GLOBAL_FREE_ID];
	float angle = 2*SDL_PI_F/circle->accuracy;
	float currentAngle = 0;

	for(int i =0; i < circle->accuracy; i++){
		Vector3 translation = {SDL_cosf(currentAngle), 0, SDL_sinf(currentAngle)};
		translation = scale3(translation, circle->radius);
		circle->points[i]= translate3(circle->center,translation);
		currentAngle += angle;
		++GLOBAL_FREE_ID;
	}
}

//Highest point and Lowest point are not counted in accuracy.
//Concentric circles will be drawn top to bottom.
//Equator is forced
//accuracy.x are the concentric circles in one hemisphere
//Concentric circle start from the highest y value downwards
void initSphere(Sphere* sphere){
	if(!sphere){
		SDL_Log("Error: Sphere object passed is NULL!");
		return;
	}
	Vector3 topPole = {0, sphere->center.y + sphere->radius, 0};
	Vector3 bottomPole =  {0, sphere->center.y - sphere->radius, 0};
	float rise = sphere->radius/(float)(sphere->accuracy.x + 1); //To have _equidistant_ concentric circles.
	float height = sphere->radius - rise;
	float R_squared = SDL_pow(sphere->radius, 2);
	sphere->points[0] = topPole;
	sphere->points[1] = bottomPole;
														
	int count = 0; 
	//Keeps track of index. 0 is the highest concentric circle. 
	//Closest to poles[0]. It moves downwards.
	//Max is expected to be sphere->accuracy.x * 2 + 1
	//This part does the top half and the equator.
	while( count <= sphere->accuracy.x ){
		Circle* circle = &sphere->circles[count];
		Vector3 translation = {0, height, 0};
		float h_squared = SDL_pow(height, 2);

		circle->center = translate3(sphere->center, translation);
	    circle->radius = SDL_pow(R_squared - h_squared, 0.5);
		circle->accuracy = sphere->accuracy.y;
		initCircle(circle);
		count++;
		height-=rise;
	}
	int reflected = sphere->accuracy.x - 1;
	height = rise;
	while( reflected >= 0 ){
		Circle* originalCircle = &sphere->circles[reflected--];
		Circle* reflectedCircle = &sphere->circles[count++];

		Vector3 translation = {0, -height * 2, 0};
		reflectedCircle->center = translate3(originalCircle->center, translation);
		reflectedCircle->radius = originalCircle->radius;
		reflectedCircle->accuracy = sphere->accuracy.y;

		reflectedCircle->points = SDL_malloc( sizeof(Vector3) * sphere->accuracy.y);
		reflectedCircle->projections = SDL_malloc( sizeof(SDL_Vertex) * sphere->accuracy.y);

		for(int i = 0; i < sphere->accuracy.y; i++){
			reflectedCircle->points[i] = translate3(originalCircle->points[i], translation);
		}
		height+=rise;
	}

	sphere->count = count;
}

void destroySphere(Sphere* sphere){
	for ( int i = 0; i < sphere->count; i++){
		SDL_free(sphere->circles[i].points);
		SDL_free(sphere->circles[i].projections);
	}
	SDL_free(sphere->circles);
}

