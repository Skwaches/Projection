#include "update.h"
#include "init.h"

bool CRASH = false;

//Macro to check for errors thrown by cuda
#define CUDOS(expression) do{\
	cudaError_t  result = expression; \
	if( result != cudaSuccess){\
		CRASH = true;\
		SDL_Log("CUDA Runtime Error! %s:%i:%s, Error -> %d", __FILE__, __LINE__, cudaGetErrorString(result), result);\
	}\
} while(0)

__device__ SDL_FPoint rotate2_GPU(SDL_FPoint point, SDL_FPoint origin, SDL_FPoint sinCos){
	float sin,cosine;
	SDL_FPoint rotated;
	
	cosine = sinCos.x;
	sin = sinCos.y;

	SDL_FPoint local = {point.x - origin.x, point.y - origin.y};
	rotated.x = local.x * cosine - local.y * sin + origin.x;
	rotated.y = local.x * sin + local.y * cosine + origin.y;
	return rotated;
}

//Planes are xy and xz
__device__ Vector3 rotate3_GPU(Vector3 point, Vector3 origin, SDL_FPoint sinCos1, SDL_FPoint sinCos2){
	SDL_FPoint focusPoint;
	SDL_FPoint transform;
	SDL_FPoint focusOrigin;

	//Along xy plane (z remains constant)
	focusPoint = (SDL_FPoint){point.x, point.y};
	focusOrigin = (SDL_FPoint){origin.x, origin.y};
	transform = rotate2_GPU(focusPoint, focusOrigin, sinCos1);
	point = (Vector3){ transform.x, transform.y, point.z};

	//Along xz plane (y remains constant)
	focusPoint = (SDL_FPoint){point.x, point.z};
	focusOrigin = (SDL_FPoint){origin.x, origin.z}; 
	transform = rotate2_GPU(focusPoint, focusOrigin, sinCos2); point = (Vector3){ transform.x, point.y, transform.y};
	return point;
}

__device__ Vector3 translate3_GPU(Vector3 point, Vector3 translation){
	return (Vector3){
		point.x + translation.x, 
		point.y + translation.y,
		point.z + translation.z
	};
}

__device__ Vector3 scale3_GPU(Vector3 point, float factor){
	return (Vector3){
		point.x * factor, point.y * factor, point.z * factor,
	};
}

__device__ SDL_FPoint projection_GPU(
		Vector3 point, Camera camera, Vector3 cameraPosition_negate){
	//Get the position of the point relative to the camera
	Vector3 local = rotate3_GPU(point, camera.position, camera.cosSin1, camera.cosSin2);
	local = translate3_GPU( local, cameraPosition_negate);
	if(local.z == 0)
		return (SDL_FPoint){0, 0}; //FIXME Figure out what's actually supposed to happen here.
	return (SDL_FPoint){ (camera.focal * local.x/local.z) , (camera.focal * local.y/local.z) };
}

__device__ float pow_GPU(float x,int y){
	float result = 1;
	for(int i  = 0; i < y; i++)
		result *= x;
	return result;
}

__device__ SDL_FColor colorVertex(SDL_FColor baseColor, float distance, Light light){
	float factor = distance ? pow_GPU(distance,2) : 1;
	float depthFactor = light.power/factor;

	return (SDL_FColor){
		baseColor.r + depthFactor * light.color.r,
		baseColor.g + depthFactor * light.color.g,
		baseColor.b + depthFactor * light.color.b,
		baseColor.a 
	};
}


__device__ float abs_GPU(float a){
	return a < 0 ? -a : a;
}

//This uses Newton-Raphson method to get an approximation.
__device__ float sqrt_GPU( float x, float epsilon){
	float guess = x/2;
	while( abs_GPU( guess*guess - x ) > epsilon ){
		guess = guess - (guess*guess - x)/(2 * guess);
	}

	return guess;
}

__device__ float distance3_GPU( Vector3 point, Vector3 origin, float epsilon){
	Vector3 respective = translate3_GPU(point, scale3_GPU(origin, -1) );
	return sqrt_GPU(pow_GPU(respective.x, 2) + pow_GPU(respective.y, 2)+ pow_GPU(respective.z, 2), epsilon);
}

//This function returns the usual coordinate of a point on the screen. 
//Only use this with a projected coordinate
__device__ SDL_FPoint convertScreen_GPU(SDL_FPoint point, SDL_Point windowSize){
	return (SDL_FPoint){ 
		(1 + point.x)/2 * windowSize.x,  
		(1 - point.y)/2 * windowSize.y
	};
}

__global__ void transform_Kernel(
		Vector3* points, SDL_Vertex* projections, ObjectProperties* objects,
		int* objectMapping, Camera camera, Vector3 mainCamera_Negate,
		SDL_Point windowSize, int length)
{
	int workingIndex = blockIdx.x * blockDim.x + threadIdx.x; 
	if(workingIndex < length ){
		ObjectProperties *object =  &objects[objectMapping[workingIndex]];
		
		//Transform
		points[workingIndex] = translate3_GPU(points[workingIndex], object->translation);	
		points[workingIndex] = rotate3_GPU(points[workingIndex], object->origin, object->cosSin1, object->cosSin2);

		//Projection
		projections[workingIndex].position = projection_GPU( points[workingIndex], camera, mainCamera_Negate);
		projections[workingIndex].position = convertScreen_GPU(projections[workingIndex].position, windowSize);
	}
}

//Allocate data
Vector3 *gpu_points = NULL; 
SDL_Vertex *gpu_projections = NULL; 
ObjectProperties *gpu_objects = NULL;
int* gpu_objectMapping = NULL;
size_t pointSpace,projectionSpace,objectSpace,objectMappingSpace;
int length;

bool init_Shader(int point_length, int objectNO){
	length = point_length;
	if(!point_length || !objectNO)
		return false;
	pointSpace = sizeof(Vector3) * length;
	projectionSpace = sizeof(SDL_Vertex) * length;
	objectSpace = sizeof(ObjectProperties) * objectNO;
	objectMappingSpace = sizeof(int) * length;

	CUDOS(cudaMalloc(&gpu_points, pointSpace));
	CUDOS(cudaMalloc(&gpu_projections, projectionSpace));
	CUDOS(cudaMalloc(&gpu_objects, objectSpace));
	CUDOS(cudaMalloc(&gpu_objectMapping, objectMappingSpace));
	return !CRASH;
}

void quit_Shader(){
	//Free memory
	cudaFree(gpu_points);
	cudaFree(gpu_projections);
	cudaFree(gpu_objects);
	cudaFree(gpu_objectMapping);
}

void transform_Shader(
		Vector3* points, SDL_Vertex* projections, Camera mainCamera,
		ObjectProperties* objects, int* objectMapping,
		SDL_Point windowSize){

	//Camera 
	Vector3  mainCamera_Negate = {
		-mainCamera.position.x,
		-mainCamera.position.y,
		-mainCamera.position.z,
	};

	//Copy data
	CUDOS(cudaMemcpy(gpu_points, points, pointSpace, cudaMemcpyHostToDevice));
	CUDOS(cudaMemcpy(gpu_projections, projections, projectionSpace, cudaMemcpyHostToDevice));
	CUDOS(cudaMemcpy(gpu_objects, objects, objectSpace, cudaMemcpyHostToDevice));
	CUDOS(cudaMemcpy(gpu_objectMapping, objectMapping, objectMappingSpace, cudaMemcpyHostToDevice));

	//Call the Kernel
	int threadsPerBlock = 256;
	int blocks = ( threadsPerBlock + length - 1 )/threadsPerBlock;
	transform_Kernel<<<blocks, threadsPerBlock>>>(
			gpu_points, gpu_projections, gpu_objects,
			gpu_objectMapping, mainCamera, mainCamera_Negate,
			windowSize, length);
	CUDOS(cudaGetLastError());
	CUDOS(cudaDeviceSynchronize());//Wait for kernel to finish.
								   
	//Copy back answer.
	CUDOS(cudaMemcpy(points, gpu_points,  pointSpace, cudaMemcpyDeviceToHost));
	CUDOS(cudaMemcpy(projections, gpu_projections,  projectionSpace, cudaMemcpyDeviceToHost));
}
