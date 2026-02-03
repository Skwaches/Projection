#include <SDL3/SDL.h>
//Values for x and y should be between -1 to 1;
typedef struct Vector3 {
	float x,y,z;
}Vector3;

typedef struct Cuboid {
	Vector3 vertices[8];
	Vector3 projection[8];
	Vector3 screen[16];
}Cuboid;

bool running = false;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Point WINDOW_SIZE = {2560, 1440};
SDL_Color BACKGROUND = {0, 0 , 0, SDL_ALPHA_OPAQUE};
SDL_WindowFlags WINDOW_FLAGS = SDL_WINDOW_FULLSCREEN;
const char* WINDOW_TITLE = "Cartesian";

SDL_Color color = {230, 77, 228, SDL_ALPHA_OPAQUE};

Vector3 dimensions = {3, 3, 3};
Vector3 topBackLeft = {-0.6, 0.8, 4};
Vector3 corners[8];
Vector3 verticesPath[16];
SDL_FPoint pathProjection[16];
SDL_FPoint pathScreenProjection[16];

// Creates the front and back faces of a cube when given the "back-left" point 
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

Vector3 translate(Vector3 point, Vector3 translation){
	return (Vector3){
		point.x + translation.x, 
		point.y + translation.y,
		point.z + translation.z
	};
}
// Returns the path needed to follow in order to render a cuboid.
// FIXME This path may not be optimal. Switching to edges may be smarter. Do this!
void linePath(Vector3 path[16],Vector3 vertices[8]){
	int indexer = 0;

	for(int i = 0; i < 4; i++)
		path[indexer++] = vertices[i];
	path[indexer++] = vertices[0];

	for(int i = 4; i < 8; i++)
		path[indexer++] = vertices[i];
	path[indexer++] = vertices[4];

	path[indexer++] = vertices[7];
	path[indexer++] = vertices[3];
	path[indexer++] = vertices[2];
	path[indexer++] = vertices[6];
	path[indexer++] = vertices[5];
	path[indexer]   = vertices[1];
}

#define FPS 60
#define WAIT_TIME (Uint64)(1000/FPS)
#define LIMIT_FPS true

//This function projects a 3d Vector to 2d space using the theoretical coordinate system.
//If you wish to do something to a point I suggest doing it BEFORE projecting it.
SDL_FPoint projection(Vector3 point){
	if(point.z == 0)
		return (SDL_FPoint){point.x, point.y};
	return (SDL_FPoint){ (point.x/point.z) , (point.y/point.z) };
}

//This function returns the usual coordinate of a point on the screen.
//Only use this with a projected coordinate
SDL_FPoint convertScreen(SDL_FPoint point){
	return (SDL_FPoint){ (1 + point.x)/2 * WINDOW_SIZE.x, (1-point.y)/2 * WINDOW_SIZE.y};
}

bool init(void){
	if(!SDL_Init(SDL_INIT_VIDEO))
		return false;
	if(!SDL_CreateWindowAndRenderer(WINDOW_TITLE, WINDOW_SIZE.x, WINDOW_SIZE.y, WINDOW_FLAGS, &window, &renderer))
		return false;
	
	//Create the cube vertices
	vertexes(corners, topBackLeft, dimensions);
	//Create path from the vertices
	linePath(verticesPath, corners);
	for(int i = 0; i<16; i++){
		//Project path
		pathProjection[i] = projection(verticesPath[i]);
		//Convert path to screen coordinates
		pathScreenProjection[i] = convertScreen(pathProjection[i]);
	}
	return true;
}

void input(){
	SDL_Event event;
	while(SDL_PollEvent(&event)){
		switch(event.type){
			case SDL_EVENT_QUIT:
				running = false;
			default:
				break;
		}
	}
}

//Returns the time past since it was last ran in milliseconds
//Don't run this twice.
float timer(){
	static Uint64 lastFrameTime = 0;
	Uint64 currentFrameTime = SDL_GetTicks();
	float 𝚫Time = lastFrameTime? currentFrameTime - lastFrameTime: 0;
	lastFrameTime = SDL_GetTicks();
	return 𝚫Time;
}

void update(){
	if(LIMIT_FPS)
		SDL_Delay(WAIT_TIME);
	float 𝚫Time = timer();
	SDL_GetWindowSize(window, &WINDOW_SIZE.x, &WINDOW_SIZE.y);
	return;
}

bool setColor(SDL_Color color){
	return SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}
void render(){
	setColor(BACKGROUND);
	SDL_RenderClear(renderer);
	setColor(color);
	SDL_RenderLines(renderer,pathScreenProjection,16);
	SDL_RenderPresent(renderer);
}

void quit(){
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	SDL_Log("Window has been closed successfully.");
}
int main(int argc, char** argv){
	running = init();
	while(running){
		input();
		update();
		render();
	}
	quit();
	return 0;
}
