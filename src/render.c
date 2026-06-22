#include "init.h"

void Err(){
		SDL_Log("I suggest you start debugging  -> %s:%i\n", __FILE__,__LINE__);
		RUNNING = false;
};

bool setColor(SDL_Color color){
	return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

bool eventTimer(int interval){
	static Uint64 elapsedTime = 0;
	static Uint64 lastFrameTime = 0;
	elapsedTime += lastFrameTime ? SDL_GetTicks() - lastFrameTime: 0;
	lastFrameTime = SDL_GetTicks();
	if(interval < elapsedTime){
		elapsedTime = 0;
		return true;
	}
	return false;
}

//The points should be in order.
int connect_LP(SDL_Vertex* points, int poleIndex, int* indices, int length){
	if(!points || !indices){
		Err();
		return 0;
	}
	int count = 0;
	for(int i = 0; i < length ;i++){
		int i_next = (i + 1)%length ;
		int i_use = (i)%length ;
		if(i == poleIndex)
			continue;
		if(i_next == poleIndex)
			i_next++;
		indices[count++] = i_use;
		indices[count++] = poleIndex;
		indices[count++] = i_next;
	}
	return count;
}
//Points should contain both lines
//The length is the length of each line.
int connect_LL(SDL_Vertex* points, int* indices, int length){
	if(!points){
		Err();
		return 0;
	}

	int count = 0;
	for(int i = 0; i < length; i++){

		int i_next = (i+1) % length;
		int j = i + length;
		int j_next = i_next + length;

		int facePoints[4] = {i, j, j_next, i_next};
		
		//My lovely magic numbers
		//I group them into quadrilaterals the to triangles.
		for(int i =0; i < 6; i++){
			int newOrder = FACE_ORDER[i];
			indices[count++] = facePoints[newOrder];
		}
	}
	return count;
}

void renderSphere(Sphere* sphere, int* indices){
	if(!sphere)
		Err();
}

bool render(){
	static int cursor = 0;
	SDL_CHECK(setColor(BACKGROUND));
	SDL_CHECK(SDL_RenderClear(renderer));
	SDL_CHECK(setColor(lineColor));

	if(eventTimer(0)){
		cursor++;
		cursor%= testSPHERE.accuracy.x * 2;
	}

	//Render;
	for(int i = 0; i < testSPHERE.count; i++)
		SDL_RenderPoint(renderer, testSPHERE.projections[i].position.x , testSPHERE.projections[i].position.y);

	int* indices = SDL_malloc(sizeof(int) * testSPHERE.accuracy.y * 3);
	int count =  connect_LP(testSPHERE.projections, 0, indices, testSPHERE.accuracy.y + 1);
	SDL_RenderGeometry(renderer, NULL, testSPHERE.projections, testSPHERE.accuracy.y + 1, indices, count);
	SDL_free(indices);
	indices = NULL;

	//int cursor = 0; cursor < testSPHERE.accuracy.x * 2 + 1 - 1; cursor++
	indices = SDL_malloc(sizeof(int) * testSPHERE.accuracy.y * 6);
	count = connect_LL(&testSPHERE.projections[1 + cursor * testSPHERE.accuracy.y], indices, testSPHERE.accuracy.y);
	SDL_RenderGeometry(renderer, NULL, &testSPHERE.projections[1 + cursor * testSPHERE.accuracy.y], testSPHERE.accuracy.y * 2, indices, count);
	SDL_free(indices);

	SDL_RenderPresent(renderer);
	return RUNNING; 
}
