#include "init.h"
#include "update.h"

bool RUNNING = true;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

Vector3* GLOBAL_POINTS = NULL;
SDL_Vertex* GLOBAL_PROJECTIONS = NULL;
ObjectProperties *GLOBAL_OBJECTS = NULL;
int* GLOBAL_OBJECT_MAPPING = NULL;
int GLOBAL_FREE_INDEX = 0; //This is the index of the first available free spot. No item is at this index
int GLOBAL_FREE_OBJECT_INDEX = 0;

inline void SDL_CHECK(bool success){
	if(!success){
		SDL_Log("Runtime Error! , %s:%i %s\n", __FILE__, __LINE__, SDL_GetError());
		RUNNING = false;
	};
};

inline void SPACE_CHECK(int spots){
	if(GLOBAL_FREE_INDEX + spots >= GLOBAL_MAX_POINTS){
		SDL_Log("Runtime Error!\n%s:%i\nThe %d points requested exceed the slots available(%d)!", __FILE__, __LINE__, spots, GLOBAL_MAX_POINTS - GLOBAL_FREE_INDEX);
		RUNNING = false;
	}
}

void init(void){
	SDL_CHECK(SDL_Init(SDL_INIT_VIDEO));
	SDL_CHECK(SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_SIZE.x, WINDOW_SIZE.y, WINDOW_FLAGS, &window, &renderer));
	GLOBAL_POINTS = SDL_malloc(GLOBAL_MAX_POINTS * sizeof(Vector3));
	GLOBAL_PROJECTIONS = SDL_malloc(GLOBAL_MAX_POINTS * sizeof(SDL_Vertex));
	GLOBAL_OBJECTS = SDL_malloc(GLOBAL_MAX_OBJECTS * sizeof(ObjectProperties));
	GLOBAL_OBJECT_MAPPING = SDL_malloc(GLOBAL_MAX_POINTS * sizeof(int));
	if(!GLOBAL_POINTS||!GLOBAL_PROJECTIONS ||!GLOBAL_OBJECTS){
		SDL_Log("You might wanna check this out... %s:%i", __FILE__, __LINE__);
		RUNNING = false;
	}
	SDL_memset(GLOBAL_POINTS, 0, GLOBAL_MAX_POINTS * sizeof(Vector3));
	SDL_memset(GLOBAL_PROJECTIONS, 0, GLOBAL_MAX_POINTS * sizeof(SDL_Vertex));
	frameOne();
	if(!init_Shader(GLOBAL_FREE_INDEX, GLOBAL_FREE_OBJECT_INDEX))
		RUNNING = false;
}

void quit(){
	frameLast();
	quit_Shader();
	SDL_free(GLOBAL_POINTS);
	SDL_free(GLOBAL_PROJECTIONS);
	SDL_free(GLOBAL_OBJECTS);
	SDL_free(GLOBAL_OBJECT_MAPPING);

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	SDL_Log("Window has been closed successfully.");
}

