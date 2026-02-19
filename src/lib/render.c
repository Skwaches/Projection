#include "init.h"
#include "update.h"

bool setColor(SDL_Color color){
	return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

bool renderCuboidEdges( SDL_FPoint vertices[CUBOID_VERTICES_NO] ){
	Line lines[CUBOID_EDGES_NO];
	for(int i = 0; i < CUBOID_EDGES_NO; i++){
		lines[i] = (Line){
			.a = vertices[ CUBOID_EDGES[i][0] ], 
				.b = vertices[ CUBOID_EDGES[i][1] ], 
				.color = (SDL_FColor){0,0,1,1},
				.thickness = 2
		};
		initLINE(&lines[i]);
		if(!SDL_RenderGeometry(renderer, NULL, lines[i].vertexes, LINE_VERTICES_NO, LINE_PATH_TRIANGLE_INDICES, LINE_PATH_TRIANGLE_NO)){
			SDL_Log("\n Rendering Error: %s", SDL_GetError());
			return false;
		}
	}
	return true;
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

//Converts a sphere to rectangles.
//Then to triangles
int squaresSphere(Sphere sphere, SDL_Vertex** vertexes,SDL_FColor baseColor, float gradient){
	int count = 0;
	//This misses one pair of circles
	for(int i = 0; i < sphere.accuracy.x; i++){
		int i_next    = (i + 1) % sphere.accuracy.x,
			i_current = i % sphere.accuracy.x ; 
		//These 2 circles' point will be paired together to form rectangles, which can easily be broken into triangles
		Circle circle1 = sphere.circles[i_current],
			   circle2 = sphere.circles[i_next];

		// One circle pair seems to be missing
		for(int j = 0; j < sphere.accuracy.y; j++)
		{
			SDL_FColor color = baseColor;
			float depthUNIT = 0;
			bool visible = true;

			int j_next = ( j+1 ) % sphere.accuracy.y,
				j_current = j % sphere.accuracy.y;

			Vector3 points[4] =  {
 				circle2.points[j_next],
 				circle1.points[j_next],
 				circle1.points[j_current],
 				circle2.points[j_current],
			};

			SDL_FPoint face[4] =  {
				circle2.projection[j_next],
				circle1.projection[j_next],
				circle1.projection[j_current],
				circle2.projection[j_current],
			};

			for (int i = 0; i < 4; i++){
				if( sphere.center.z - points[i].z  < 0){
					// Point is on the other half of the sphere!
					// This assumes camera is at (0,0,0)
					visible = false;
					break;
				}
				depthUNIT += points[i].z ;
			}

			if (!visible)
				continue;
			
			depthUNIT/=4.0f;
			depthUNIT=(sphere.center.z-depthUNIT)/sphere.radius;
			depthUNIT=SDL_pow(depthUNIT, gradient);
			color.r = depthUNIT;
			color.b += depthUNIT;
			color.g += depthUNIT;

			for( int k = 0; k < magic; k++){
				vertexes[count][k] = (SDL_Vertex){
					.position = face[FACE_ORDER[k]],
					.color = color
				};
			}
			count++;
		}
	}
	return count;
}

bool render(){
	if(!setColor(BACKGROUND))
		return false;
	if(!SDL_RenderClear(renderer))
		return false;

	setColor(lineColor);
	SDL_FColor faceColors[6] = {
		{1, 0, 0, 1},
		{0, 1, 0, 1},
		{0, 0, 1, 1},
		{1, 1, 0, 1},
		{1, 0, 1, 1},
		{0, 1, 1, 1},
	};
	/*
	int depthBuffer[magic];
	cuboidFaceBuffer(testCUBOID.vertices, depthBuffer);
	renderCuboidFaces(testCUBOID.projection, depthBuffer, faceColors);
	*/
	
	int rectsNO = (testSPHERE.accuracy.y-1) * (testSPHERE.accuracy.x-1) + 1000;
	SDL_Vertex** vertexes = SDL_malloc(sizeof(SDL_Vertex*) * rectsNO);
	for(int i = 0; i < rectsNO; i++)
		vertexes[i] = SDL_malloc( sizeof(SDL_Vertex) * magic );
	SDL_FColor baseColor= {1.00, 0.80, 0.45, 1}; 

	int count = squaresSphere(testSPHERE, vertexes, baseColor, 9 );
	for(int i = 0; i < count; i++)
		SDL_RenderGeometry(renderer, NULL, vertexes[i], magic, NULL, 0);

	for(int i = 0; i < rectsNO; i++)
		 SDL_free(vertexes[i]);
	SDL_free(vertexes);
	
	if(!SDL_RenderPresent(renderer))
		return false;
	return true;
}

