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


