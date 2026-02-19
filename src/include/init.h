#ifndef INIT
#define INIT

#include <SDL3/SDL.h>
#include "classes.h"

extern bool LIMIT_FPS;
extern float WAIT_TIME;

extern bool RUNNING;
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_Point WINDOW_SIZE; 
extern SDL_Color BACKGROUND;
extern SDL_WindowFlags WINDOW_FLAGS;
extern const char* WINDOW_TITLE;

extern SDL_Color lineColor;

//init functions
bool init(void);
void quit(void);
void frameOne(void);
void frameLast(void);

//Rendering
bool render(void);

//input
void input(void);
extern bool 
h_KEY, j_KEY, k_KEY, l_KEY,
a_KEY, w_KEY, s_KEY, d_KEY,
lShift_KEY;

extern Cuboid testCUBOID;
extern Circle testCIRCLE;
extern Sphere testSPHERE;

//Coordinates
extern Vector3 CAMERA;
//Speed
extern Vector3 ANGULAR_VELOCITY;
extern Vector3 LINEAR_SPEED;
#endif
