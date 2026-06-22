#ifndef INIT
#define INIT

#include <SDL3/SDL.h>
#include "classes.h"

extern bool RUNNING;

//FPS
extern bool LIMIT_FPS;
extern float CURRENT_FPS;
extern float WAIT_TIME;

//Window
extern SDL_Window* window;
extern SDL_Point WINDOW_SIZE; 
extern SDL_WindowFlags WINDOW_FLAGS;
extern const char* WINDOW_TITLE;

//init functions
void init(void);
void quit(void);
bool frameOne(void);
void frameLast(void);

//Shader initialisaton
#ifdef __cplusplus
extern "C" {
#endif

void quit_Shader();
bool init_Shader(int point_length, int objectNO);

#ifdef __cplusplus
}
#endif

//Error Checking
void SDL_CHECK(bool success);
void SPACE_CHECK(int spots);

//Rendering
extern SDL_Renderer* renderer;
extern Vector3* GLOBAL_POINTS;
extern SDL_Vertex* GLOBAL_PROJECTIONS;
extern int* GLOBAL_OBJECT_MAPPING;//This is a shit workaround so I can label assign all the points to an item
extern int GLOBAL_MAX_POINTS;
extern int GLOBAL_FREE_INDEX;
bool render(void);


//Input
void input(void);
extern bool 
h_KEY, j_KEY, k_KEY, l_KEY,
a_KEY, w_KEY, s_KEY, d_KEY,
lShift_KEY;
extern bool INPUT_RECEIVED;

//Test Objects
extern Cuboid testCUBOID;
extern Circle testCIRCLE;
extern Sphere testSPHERE;
extern Light testLIGHT;

extern ObjectProperties* GLOBAL_OBJECTS;
extern int GLOBAL_MAX_OBJECTS;
extern int GLOBAL_FREE_OBJECT_INDEX;

//Colors
extern SDL_Color lineColor;
extern SDL_Color BACKGROUND;

//Coordinates
extern Camera MAIN_CAMERA;

//Movement speeds
extern SDL_FPoint ANGULAR_VELOCITY;
extern Vector3 LINEAR_SPEED;

#endif
