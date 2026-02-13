#include "update.h"

void updateCircle(Circle* circle){
	for(int i = 0; i < circle->accuracy; i++){
		circle->projection[i] = convertScreen(projection(circle->points[i]));
	}
	return;
}

//Mallocs memory. Remember to free!
void initCircle(Circle* circle){
	if(!circle){
		SDL_Log("Circle object passed to be initialised is NULL!");
		return;
	}
	circle->points = SDL_malloc(sizeof(Vector3) * circle->accuracy);
	circle->projection = SDL_malloc(sizeof(Vector3) * circle->accuracy);
	float angle = 2*SDL_PI_F/circle->accuracy;
	float currentAngle = 0;
	for(int i =0; i<circle->accuracy; i++){
		Vector3 translation = {SDL_cosf(currentAngle), SDL_sinf(currentAngle), 0};
		translation = scale3(translation, circle->radius);
		circle->points[i]= translate3(circle->center,translation);
		currentAngle += angle;
	}
	updateCircle(circle);
}

//Plane starts with standing surface moving Anti-clockwise from the right
//Points start form the left center anti-clockwise
//It will rotate about the x-axis.
void initSphere(Sphere* sphere){
	if(!sphere){
		SDL_Log("Error: Sphere object passed is NULL!");
		return;
	}
	sphere->circles = SDL_malloc(sizeof(Circle) * sphere->accuracy.x);
	float angle = SDL_PI_F/sphere->accuracy.x;
	float currentAngle = angle;

	sphere->circles[0].radius = sphere->radius;
	sphere->circles[0].center = sphere->center;
	sphere->circles[0].accuracy = sphere->accuracy.y;
	initCircle(&sphere->circles[0]);
	for(int i = 1; i < sphere->accuracy.x; i++){

		//These are duplicates. I'm filling them out for safety. Maybe get rid of them?
		sphere->circles[i].radius = sphere->radius;
		sphere->circles[i].center = sphere->center;
		sphere->circles[i].accuracy = sphere->accuracy.y;

		//Remember to free!
		sphere->circles[i].points = SDL_malloc(sizeof(Circle) * sphere->accuracy.y);
		sphere->circles[i].projection = SDL_malloc(sizeof(Vector3) * sphere->accuracy.y);

		for(int k = 0; k < sphere->accuracy.y; k++){
			sphere->circles[i].points[k] = rotate3(sphere->circles[0].points[k], sphere->center, (Vector3){ 0, 0, currentAngle});
		}
		updateCircle(&sphere->circles[i]);
		currentAngle += angle;
	}
}

void updateSphere(Sphere* sphere){
	for( int i = 0; i < sphere->accuracy.x; i++)
		updateCircle(&sphere->circles[i]);
}

void destroySphere(Sphere* sphere){
	for ( int i = 0; i < sphere->accuracy.x; i++){
		SDL_free(sphere->circles[i].points);
		SDL_free(sphere->circles[i].projection);
	}
	SDL_free(sphere->circles);
}
