#include "update.h"
#include "init.h"

//Macro to check for errors thrown by cuda
#define CUDOS(expression) do{ \
	cudaError_t  result = expression; \
	if( result != cudaSuccess){\
		SDL_Log("CUDA Runtime Error! %s:%i:%s, Error -> %d", __FILE__, __LINE__, cudaGetErrorString(result), result);\
	}\
} while(0)

//Thread dim should be the number of points on the circle
//Block dim should be the number of circle on the sphere
__global__ void updateSphere_Kernel(Sphere* sphere, Vector3 translation, SDL_FPoint sincos1, SDL_FPoint sincos2, SDL_Point WINDOw_SIZE){
	if(blockIdx.x < sphere->count && threadIdx.x < sphere->accuracy.y)
	{
		//Translate
		sphere->circles[blockIdx.x].points[threadIdx.x].x += translation.x ;
		sphere->circles[blockIdx.x].points[threadIdx.x].y += translation.y ;
		sphere->circles[blockIdx.x].points[threadIdx.x].z += translation.z ;

		//Rotate
		float sin,cosine;
		SDL_FPoint local;

		cosine = sincos1.x;
		sin = sincos1.y;
		local = { sphere->circles[blockIdx.x].points[threadIdx.x].x - sphere->center.x, sphere->circles[blockIdx.x].points[threadIdx.x].y - sphere->center.y};
		sphere->circles[blockIdx.x].points[threadIdx.x].x = local.x * cosine - local.y * sin   + sphere->center.x;
		sphere->circles[blockIdx.x].points[threadIdx.x].y = local.x * sin    + local.y * cosine + sphere->center.y;

		cosine = sincos2.x;
		sin = sincos2.y;
		local = {sphere->circles[blockIdx.x].points[threadIdx.x].x - sphere->center.x, sphere->circles[blockIdx.x].points[threadIdx.x].z - sphere->center.z};
		sphere->circles[blockIdx.x].points[threadIdx.x].x = local.x * cosine - local.y * sin   + sphere->center.x;
		sphere->circles[blockIdx.x].points[threadIdx.x].z = local.x * sin    + local.y * cosine + sphere->center.z;

		sphere->circles[blockIdx.x].projection[threadIdx.x].position = (SDL_FPoint){ (1 + sphere->circles[blockIdx.x].points[threadIdx.x].x)/2 * WINDOw_SIZE.x, (1-sphere->circles[blockIdx.x].points[threadIdx.x].y)/2 * WINDOw_SIZE.y};
	}
}

void updateSphere_Shader(Sphere* sphere, Vector3 translation, SDL_FPoint angle){

	Vector* points cudaHostMalloc
	Sphere* gpu_sphere = NULL;
	SDL_FPoint sincos1 = { SDL_cosf(angle.x), SDL_sinf(angle.x) };
	SDL_FPoint sincos2 = { SDL_cosf(angle.y), SDL_sinf(angle.y) };
	
	//Allocate memory.
	CUDOS(cudaMalloc(&gpu_sphere, sizeof(Sphere)));
	CUDOS(cudaMalloc(&gpu_sphere->circles, sizeof(Circle) * sphere->count));

	for(int i = 0; i < sphere->count; i++){
		CUDOS(cudaMalloc(&gpu_sphere->circles[i].points, sizeof(Vector3) * sphere->accuracy.y));
		CUDOS(cudaMalloc(&gpu_sphere->circles[i].projection, sizeof(SDL_Vertex) * sphere->accuracy.y));

	//Copy memory.
		CUDOS(cudaMemcpy(gpu_sphere->circles[i].points, sphere->circles[i].points ,sizeof(Vector3) * sphere->accuracy.y, cudaMemcpyHostToDevice));
		CUDOS(cudaMemcpy(gpu_sphere->circles[i].projection, sphere->circles[i].projection ,sizeof(SDL_Vertex) * sphere->accuracy.y, cudaMemcpyHostToDevice));
	}
	gpu_sphere->accuracy = sphere->accuracy;
	gpu_sphere->center = sphere->center;
	gpu_sphere->count = sphere->count;

	//Call Kernel
	updateSphere_Kernel<<<sphere->count,sphere->accuracy.y>>>(gpu_sphere, translation, sincos1, sincos2, WINDOW_SIZE);

	
	for(int i = 0; i < sphere->count; i++){
		//Copy results
		CUDOS(cudaMemcpy(sphere->circles[i].points, gpu_sphere->circles[i].points, sizeof(Vector3) * sphere->accuracy.y, cudaMemcpyDeviceToHost));
		CUDOS(cudaMemcpy(sphere->circles[i].projection, gpu_sphere->circles[i].projection, sizeof(SDL_Vertex) * sphere->accuracy.y, cudaMemcpyDeviceToHost));
		//Free memory
		CUDOS(cudaFree(gpu_sphere->circles[i].points));
		CUDOS(cudaFree(gpu_sphere->circles[i].projection));
	}
	CUDOS(cudaFree(gpu_sphere->circles));
	CUDOS(cudaFree(gpu_sphere));

}
