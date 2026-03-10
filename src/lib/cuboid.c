#include "update.h"

// Creates the front and back faces of a cube when given the "back-left" point 
// Anti-clockwise back then front. Starts from top-left
void vertexes(Vector3* vertices, Vector3 bak, Vector3 dimensions){
	//Back Face
	vertices[0] = bak;
	vertices[1] = (Vector3){bak.x, 				  bak.y - dimensions.y, bak.z};
	vertices[2] = (Vector3){bak.x + dimensions.x, bak.y - dimensions.y, bak.z};
	vertices[3] = (Vector3){bak.x + dimensions.x, bak.y,				bak.z};
	
	//Front Face
	vertices[4] = (Vector3){bak.x, 				  bak.y, 				bak.z + dimensions.z};
	vertices[5] = (Vector3){bak.x, 				  bak.y - dimensions.y, bak.z + dimensions.z};
	vertices[6] = (Vector3){bak.x + dimensions.x, bak.y - dimensions.y, bak.z + dimensions.z};
	vertices[7] = (Vector3){bak.x + dimensions.x, bak.y,				bak.z + dimensions.z};
}

//Returns the center of the cube provided 
Vector3 centerCuboid(Cuboid cube){
	Vector3 center = { 
	(cube.points[6].x + cube.points[0].x)/2 , 
	(cube.points[6].y + cube.points[0].y)/2 ,
	(cube.points[6].z + cube.points[0].z)/2 , 
		};
	return center;
}

void updateCuboid(Cuboid* cube, Vector3 *translation, Vector3 *origin, SDL_FPoint *angle){
	cube->center = centerCuboid(*cube);

	for(int i = 0; i < CUBOID_VERTICES_NO; i++){
		if(translation)
			cube->points[i] = translate3(cube->points[i], *translation);
		if(angle && origin)
			cube->points[i] = rotate3(cube->points[i], *origin, *angle);
		SDL_FPoint projected = projection(cube->points[i]);
		cube->projection[i].position = convertScreen(projected);
	}
}
void initCuboid(Cuboid* cube){
	vertexes(cube->points, cube->topBackLeft, cube->dimensions);
	updateCuboid(cube, NULL, NULL, NULL);
}

