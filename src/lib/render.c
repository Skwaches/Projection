#include "init.h"
#include "update.h"

bool setColor(SDL_Color color){
	return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

SDL_FColor convertToFColor(SDL_Color color){
	return (SDL_FColor){(float)color.r/255.0, (float)color.g/255.0,  (float)color.g/255.0,  (float)color.a/255.0};
}
bool renderCuboidEdges( SDL_FPoint vertices[CUBOID_VERTICES_NO] ){
	Line lines[CUBOID_EDGES_NO];
	for(int i = 0; i < CUBOID_EDGES_NO; i++){
		lines[i] = (Line){
			.a = vertices[ CUBOID_EDGES[i][0] ], 
			.b = vertices[ CUBOID_EDGES[i][1] ], 
			.color = (SDL_FColor){0,0,1,1},
			.thickness = 20
		};
		initLINE(&lines[i]);
		if(!SDL_RenderGeometry(renderer, NULL, lines[i].vertexes, LINE_VERTICES_NO, LINE_PATH_TRIANGLE_INDICES, LINE_PATH_TRIANGLE_NO)){
			SDL_Log("\n Rendering Error: %s", SDL_GetError());
			return false;
		}
	}
	return true;
}

bool renderCuboidFaces ( SDL_FPoint vertices[CUBOID_VERTICES_NO], SDL_FColor color){
	SDL_Vertex vertexes[CUBOID_VERTICES_NO];
	for( int i = 0; i < CUBOID_VERTICES_NO; i++){
		vertexes[i] = (SDL_Vertex){
			.position = vertices[i],
			.color = color
		};
	}
	SDL_RenderGeometry(renderer, NULL, vertexes, CUBOID_VERTICES_NO, CUBOID_PATH_TRIANGLE_INDICES, CUBOID_PATH_TRIANGLE_NO);
	return true;
} 

bool render(){
	if(!setColor(BACKGROUND))
		return false;
	if(!SDL_RenderClear(renderer))
		return false;

	renderCuboidEdges(testCUBOID.projection);
	renderCuboidFaces(testCUBOID.projection, (SDL_FColor){0.8, 0.4, 0.3, 1});

	if(!SDL_RenderPresent(renderer))
		return false;
	return true;
}

