#include "init.h"

bool setColor(SDL_Color color){ return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

#define magic 6
/* Sorts a list
 * Largest to smallest
 * Returns the indices of the sorted list with reference to the original.
*/
void sortInts(float* numbers, int count, int* result){
	if ( !numbers || !result )
		return;
	//Init indices
	for(int i = 0; i < count; i++){
		result[i] = i;
	}

	for(;;){
		int success = 1;
		for(int i = 0; i + 1 < count; i++ ){
			if (numbers[result[i]] < numbers[result[i+1]]){
				//Swap
				success = 0;
				result[i] ^= result[i+1];
				result[i+1] ^= result[i];
				result[i] ^= result[i+1];
			}
		}
		if (success){
			return;
		}
	}
}

void cuboidFaceBuffer(Vector3 points[CUBOID_VERTICES_NO], int order[magic]){
	/* Give points weight and sum the weight for each face then order them according to the weights
	 * Heavier points are rendered first
	 * */
	float weights[magic];
	for(int i = 0; i < magic; i++)
		weights[i] = 0;
	for(int i = 0; i < FACE_POINTS_NO; i++){
		weights[0] += points[CUBOID_FACE_0[i]].z;
		weights[1] += points[CUBOID_FACE_1[i]].z;
		weights[2] += points[CUBOID_FACE_2[i]].z;
		weights[3] += points[CUBOID_FACE_3[i]].z;
		weights[4] += points[CUBOID_FACE_4[i]].z;
		weights[5] += points[CUBOID_FACE_5[i]].z;
	}
	sortInts(weights, magic, order);
};

bool renderCuboidFaces ( SDL_FPoint vertices[CUBOID_VERTICES_NO], int* order, SDL_FColor colors[6]){
	SDL_Vertex face0[magic];
	SDL_Vertex face1[magic];
	SDL_Vertex face2[magic];
	SDL_Vertex face3[magic];
	SDL_Vertex face4[magic];
	SDL_Vertex face5[magic];

	//FIXME This for loop just builds objects....feel like this can be optimised somehow.
	for (int i = 0; i < magic; i++){
		int triangleID = FACE_ORDER[i];
		face0[i] = (SDL_Vertex){ 
			.position = vertices[CUBOID_FACE_0[triangleID]],
			.color = colors[0]
		};
		face1[i] =  (SDL_Vertex){ 
			.position = vertices[CUBOID_FACE_1[triangleID]],
			.color = colors[1]
		};
		face2[i] =  (SDL_Vertex){ 
			.position = vertices[CUBOID_FACE_2[triangleID]],
			.color = colors[2]
		};
		face3[i] =  (SDL_Vertex){ 
			.position = vertices[CUBOID_FACE_3[triangleID]],
			.color = colors[3]
		};
		face4[i] =  (SDL_Vertex){ 
			.position = vertices[CUBOID_FACE_4[triangleID]],
			.color = colors[4]
		};
		face5[i] = (SDL_Vertex){ 
			.position = vertices[CUBOID_FACE_5[triangleID]],
			.color = colors[5]
		};
	}
	/*
	 *	Faces must be renderered in order.
	 *	Higher z values get rendered first
	 *	It looks shit otherwise
	 *	I know the code is shit.
	 *	I don't want to hear about it.
	 * */  
	for(int i = 0; i < magic; i++){
		switch(order[i])
		{
			case 0: 
				SDL_RenderGeometry(renderer, NULL, face0, magic, NULL, 0);
				break;
			case 1:
				SDL_RenderGeometry(renderer, NULL, face1, magic, NULL, 0);
				break;
			case 2:
				SDL_RenderGeometry(renderer, NULL, face2, magic, NULL, 0);
				break;
			case 3:
				SDL_RenderGeometry(renderer, NULL, face3, magic, NULL, 0);
				break;
			case 4:
				SDL_RenderGeometry(renderer, NULL, face4, magic, NULL, 0);
				break;
			case 5: 
				SDL_RenderGeometry(renderer, NULL, face5, magic, NULL, 0);
				break;
		}
	}
	return true;
} 

int cylinderConnect(Circle* circle1, Circle* circle2, SDL_Vertex* vertices, SDL_FColor baseColor){	
	int count = 0;
	int index = 0;
	if(!circle1 || !circle2 || !vertices){
		SDL_Log("Null occurence!");
		return 0;
	}
	if(circle1->accuracy != circle2->accuracy){
		SDL_Log("Circles must match");
		return 0;
	}

	while(index < circle1->accuracy){
		int next = (index + 1) %circle2->accuracy;
		SDL_FPoint face[4] = {
			circle1->projections[index].position,
			circle2->projections[index].position,
			circle2->projections[next].position,
			circle1->projections[next].position,
		};
		index++;
		for(int i = 0; i < magic; i++){
			int id = FACE_ORDER[i];
			vertices[count++] = (SDL_Vertex){
				.position = face[id],
				.color = baseColor
			};
		}
	}

	return count;
}

//Connect the points of a circle to a point in space.
//This doesn't return a list of indices because the point is assumed to not be part of the list.
int connectPia(Circle* circle, SDL_FPoint* point, SDL_Vertex* vertex, SDL_FColor color){
	int count = 0;
	if(!circle || !point || !vertex){
		SDL_Log("Null occurence : connectPia");
		return 0;
	}
	for(int i = 0; i < circle->accuracy; i++){
		int next = i + 1 % circle->accuracy;
		SDL_FPoint points[3] = {
			*point,
			circle->projections[i].position,
			circle->projections[next].position,
		};
		for(int j = 0; j < 3; j++)
			vertex[count++] = (SDL_Vertex) {.position = points[j], .color = color};
	}
	return count;
}

//Converts a sphere to rectangles.
//Then to triangles
int fillSphere(Sphere *sphere, SDL_Vertex* vertexes, SDL_FColor baseColor){
	int count = 0;
	//Top pole and first concentric circle.
	//And bottom pole with last concentric circle.

	//Connect the other concentric circles
	for( int o = 0; o < sphere->count - 1; o++){
		int number = cylinderConnect(&sphere->circles[o], &sphere->circles[o+1], &vertexes[count], baseColor);
		count+=number;
	}
	return count;
}

bool render(){
	SDL_CHECK(setColor(BACKGROUND));
	SDL_CHECK(SDL_RenderClear(renderer));
	SDL_CHECK(setColor(lineColor));
	SDL_FColor faceColors[6] = {
		{1, 0, 0, 1},
		{0, 1, 0, 1},
		{0, 0, 1, 1},
		{1, 1, 0, 1},
		{1, 0, 1, 1},
		{0, 1, 1, 1},
	};
	
	static Uint64 accumulated = 0, lastFrameTime = 0;
	Uint64 maxTime = 100;
	Uint64 delay = 1000;
	static bool wait =false;
	Uint64 currentFrameTime = SDL_GetTicks();

	accumulated += lastFrameTime ? currentFrameTime - lastFrameTime: 0;
	lastFrameTime = currentFrameTime;

	if(wait){
		wait = accumulated < delay;
		if(!wait)
			accumulated = 0;
	}

	static int index = 0;
	if( !wait && accumulated >= maxTime ){
		accumulated = 0;
		index ++;
		if( index == testSPHERE.count - 1){
			wait = true;
			goto end;	
		}
		index %= testSPHERE.count - 1;
	}
	for(int i = 0; i < testSPHERE.count; i++){
		for(int j = 0; j < testSPHERE.accuracy.y; j++){
			int next_j = (j+1) % testSPHERE.accuracy.y;
			SDL_RenderLine(
					renderer, 
					testSPHERE.circles[i].projections[j].position.x,      testSPHERE.circles[i].projections[j].position.y,
					testSPHERE.circles[i].projections[next_j].position.x, testSPHERE.circles[i].projections[next_j].position.y
					);
		}
	}
end:;
	SDL_CHECK(SDL_RenderPresent(renderer));
	return true;
}

