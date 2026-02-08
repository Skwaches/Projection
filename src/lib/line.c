#include "update.h"

//Returns a unit vector perpendicular to the vector provided.
SDL_FPoint perpendicularVECTOR (SDL_FPoint vector){
	if ( vector.x == 0)
		return (SDL_FPoint){1, 0};
	float ratio = vector.y/vector.x;
	float b = SDL_powf(SDL_powf(ratio, 2) + 1, -1);
	float a = -b * ratio;
	return (SDL_FPoint){a,b};
}

void initLINE (Line* line){
	SDL_FPoint vector = translate2(line->a, scale2(line->b, -1));
	SDL_FPoint positive = scale2(perpendicularVECTOR(vector), line->thickness/2);
	SDL_FPoint negative = scale2(positive, -1);

	line->vertexes[0].position = translate2(line->a, positive);
	line->vertexes[1].position = translate2(line->a, negative); 
	line->vertexes[2].position = translate2(line->b, negative);
	line->vertexes[3].position = translate2(line->b, positive);

	line->vertexes[0].color = line->color;
    line->vertexes[1].color = line->color;
	line->vertexes[2].color = line->color;
    line->vertexes[3].color = line->color;
}
