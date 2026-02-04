#include "update.h"

Vector3 translate(Vector3 point, Vector3 translation){
	return (Vector3){
		point.x + translation.x, 
		point.y + translation.y,
		point.z + translation.z
	};
}

//Returns the rotation of a point about another (origin).
//Matrix of rotation is:
// 		cos Θ 	-sin Θ
// 		sin Θ 	cos Θ
//FIXME Consider writing a function for matrix multiplication.
//This will make it easier to implement other, more complex, transforms
SDL_FPoint rotate2(SDL_FPoint point, SDL_FPoint origin, double angle){
	float sin,cosine;
	SDL_FPoint rotated;
	
	sin = SDL_sinf(angle);
	cosine = SDL_cosf(angle);
	SDL_FPoint local = {point.x -origin.x, point.y - origin.y};
	rotated.x = local.x * cosine - local.y *sin + origin.x;
	rotated.y = local.x * sin + local.y*cosine + origin.y;
	return rotated;
}

//Planes are xy, xz and zy FIXME Splitting rotation for each plane may be cleaner. Doesn't bother me either way
Vector3 rotate3(Vector3 point, Vector3 angle, Vector3 origin){
	SDL_FPoint focusPoint, transform, focusOrigin;
	double focusAngle;

	//Along xy plane (z remains constant)
	focusPoint = (SDL_FPoint){point.x, point.y};
	focusOrigin = (SDL_FPoint){origin.x, origin.y};
	focusAngle = angle.x;
	transform = rotate2(focusPoint, focusOrigin, focusAngle);
	point = (Vector3){ transform.x, transform.y, point.z};

	//Along xz plane (y remains constant)
	focusPoint = (SDL_FPoint){point.x, point.z};
	focusOrigin = (SDL_FPoint){origin.x, origin.z};
	focusAngle = angle.y;
	transform = rotate2(focusPoint, focusOrigin, focusAngle);
	point = (Vector3){ transform.x, point.y, transform.y};

	//Along zy plane (x remains constant)
	focusPoint = (SDL_FPoint){point.z, point.y};
	focusOrigin = (SDL_FPoint){origin.z, origin.y};
	focusAngle = angle.z;
	transform = rotate2(focusPoint, focusOrigin, focusAngle);
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
	float 𝚫time = timer()/1000.0;
	SDL_GetWindowSize(window, &WINDOW_SIZE.x, &WINDOW_SIZE.y);
	int direction = lShift_KEY ? 1: -1;

	Vector3 angle = { 
		ANGULAR_VELOCITY.x * 𝚫time * direction * h_KEY,
		ANGULAR_VELOCITY.y * 𝚫time * direction * j_KEY,
		ANGULAR_VELOCITY.z * 𝚫time * direction * k_KEY,
	};

	Vector3 translation = {
		LINEAR_SPEED.x * 𝚫time * a_KEY * direction,
		LINEAR_SPEED.y * 𝚫time * w_KEY * direction,
		LINEAR_SPEED.z * 𝚫time * s_KEY * direction,
	};

	for(int i = 0; i < CUBOID_VERTICES; i++){
		testCUBOID.vertices[i] = rotate3(testCUBOID.vertices[i], angle, testCUBOID.center);
		testCUBOID.vertices[i] = translate(testCUBOID.vertices[i], translation);
		updateCuboid(&testCUBOID);
	}

	return;
}

