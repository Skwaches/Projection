#include "update.h"

Vector3 translate(Vector3 point, Vector3 translation){
	return (Vector3){
		point.x + translation.x, 
		point.y + translation.y,
		point.z + translation.z
	};
}

//Returns the rotation of a point.
//Matrix of rotation is:
// 		cos Θ 	-sin Θ
// 		sin Θ 	cos Θ
//FIXME Consider writing a function for matrix multiplication.
//This will make it easier to implement other, more complex, transforms
SDL_FPoint rotate2(SDL_FPoint point, double angle){
	float sin,cosine;
	SDL_FPoint rotated;
	
	//xy rotation (z is constant)
	sin = SDL_sinf(angle);
	cosine = SDL_cosf(angle);
	rotated.x = point.x*cosine - point.y*sin;
	rotated.y = point.x*sin + point.y*cosine;
	return rotated;
}

//Planes are xy, xz and zy
//FIXME Splitting rotation for each plane may be cleaner.
//Doesn't bother me either way
Vector3 rotate3(Vector3 point, Vector3 angle){
	SDL_FPoint focusPoint, transform;
	double focusAngle;

	//Along xy plane (z remains constant)
	focusPoint = (SDL_FPoint){point.x, point.y};
	focusAngle = angle.x;
	transform = rotate2(focusPoint, focusAngle);
	point = (Vector3){ transform.x, transform.y, point.z};

	//Along xz plane (y remains constant)
	focusPoint = (SDL_FPoint){point.x, point.z};
	focusAngle = angle.y;
	transform = rotate2(focusPoint, focusAngle);
	point = (Vector3){ transform.x, point.y, transform.y};

	//Along zy plane (x remains constant)
	focusPoint = (SDL_FPoint){point.z, point.y};
	focusAngle = angle.z;
	transform = rotate2(focusPoint, focusAngle);
	point = (Vector3){ point.x, transform.y, transform.x};

	return point;
}

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
	float 𝚫time = timer();
	SDL_GetWindowSize(window, &WINDOW_SIZE.x, &WINDOW_SIZE.y);

	int direction = lShift_KEY ? -1: 1;
	Vector3 angle = { 
		//Reverse when shift.
		ANGULAR_VELOCITY.x * 𝚫time/1000 * (int)(h_KEY)* direction, 
		ANGULAR_VELOCITY.y * 𝚫time/1000 * (int)(j_KEY)* direction, 
		ANGULAR_VELOCITY.z * 𝚫time/1000 * (int)(k_KEY)* direction
	};

	Vector3 translation = {
		LINEAR_SPEED.x * 𝚫time/1000 * a_KEY,
		LINEAR_SPEED.y * 𝚫time/1000 * s_KEY * direction,
		LINEAR_SPEED.z * 𝚫time/1000 * w_KEY * direction
	};

	for(int i = 0; i < CUBOID_VERTICES; i++){
		testCUBOID.vertices[i] = rotate3(testCUBOID.vertices[i], angle);
		testCUBOID.vertices[i] = translate(testCUBOID.vertices[i], translation);
		updateCuboid(&testCUBOID);
	}
	return;
}

