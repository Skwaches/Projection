#include "update.h"

//Macro to check for errors thrown by cuda
#define CUDOS(expression) do{ \
	cudaError_t  result = expression; \
	if( result != cudaSuccess){\
		SDL_Log("CUDA Runtime Error! %s:%i:%s, Error -> %d", __FILE__, __LINE__, cudaGetErrorString(result), result);\
	}\
} while(0)

__global__ void translate3_Kernel(Vector3* vectors, Vector3* translation, Vector3* results, int size, bool uniqueTranslations){
	int workingIndex = blockIdx.x * blockDim.x + threadIdx.x; 
	if(workingIndex < size ){
		Vector3 translator = uniqueTranslations ? translation[workingIndex]: *translation;
		results[workingIndex].x = vectors[workingIndex].x + translator.x; 
		results[workingIndex].y = vectors[workingIndex].y + translator.y; 
		results[workingIndex].z = vectors[workingIndex].z + translator.z; 
	}
}
void translate3_Shader(Vector3* points, Vector3* translation, Vector3* results, int size, bool unique){
	//Allocate data
	Vector3 *gpu_points = NULL, *gpu_results = NULL, *gpu_translation;
	int translations = unique ? size : 1;

	CUDOS(cudaMalloc(&gpu_points,  sizeof(Vector3) * size));
	CUDOS(cudaMalloc(&gpu_results, sizeof(Vector3) * size));
	CUDOS(cudaMalloc(&gpu_translation, sizeof(Vector3) * translations));

	//Copy data
	CUDOS(cudaMemcpy(gpu_points,      points,  sizeof(Vector3) * size, cudaMemcpyHostToDevice));
	CUDOS(cudaMemcpy(gpu_translation, translation, sizeof(Vector3) * translations, cudaMemcpyHostToDevice));
	CUDOS(cudaMemset(gpu_results,     0, sizeof(Vector3) * size));

	//Call the Kernel
	int threadsPerBlock = 256;
	int blocks = ( threadsPerBlock + size - 1 )/size;
	translate3_Kernel<<<blocks, threadsPerBlock>>>(gpu_points, gpu_translation, gpu_results, size, unique);
	CUDOS(cudaGetLastError());
	CUDOS(cudaDeviceSynchronize());//Wait for kernel to finish.
								   
	//Copy back answer.
	CUDOS(cudaMemcpy(results, gpu_results, sizeof(Vector3) * size, cudaMemcpyDeviceToHost));

	//Free memory
	cudaFree(gpu_points);
	cudaFree(gpu_results);
	cudaFree(gpu_translation);
}


__global__ void transform2_Kernel(SDL_FPoint* vectors, SDL_FPoint translation, int size){
	int workingIndex = blockIdx.x * blockDim.x + threadIdx.x; 
	if(workingIndex < size){
		vectors[workingIndex].x += translation.x;
		vectors[workingIndex].y += translation.y;
	}
}

void translate2_Shader(SDL_FPoint* points, SDL_FPoint translation, int size){
	
	//Allocate data
	SDL_FPoint* gpu_points = NULL;
	CUDOS(cudaMalloc(&gpu_points, sizeof(SDL_FPoint) * size));

	//Copy data
	CUDOS(cudaMemcpy(gpu_points, points, sizeof(SDL_FPoint) * size, cudaMemcpyHostToDevice));

	//Call the Kernel
	int threadsPerBlock = 256;
	int blocks = ( threadsPerBlock + size - 1 )/size;
	transform2_Kernel<<<blocks, threadsPerBlock>>>(gpu_points, translation, size);
	CUDOS(cudaGetLastError());
	CUDOS(cudaDeviceSynchronize());//Wait for kernel to finish.

	//Copy back answer.
	CUDOS(cudaMemcpy(points, gpu_points, sizeof(SDL_FPoint) * size, cudaMemcpyDeviceToHost));

	//Free memory
	cudaFree(gpu_points);
}

void rotate2_Shader(SDL_FPoint* points, SDL_FPoint origin, float angle, int size);
void rotate3_Shader(Vector3* points, Vector3 origin, Vector3 angle, int size);

void scale2_Shader(SDL_FPoint* points, float factor, int size);
void scale3_Shader(Vector3* points, float factor, int size);
void projection_Shader(Vector3* points, int size);
void convertScreen_Shader(SDL_FPoint* points, int size);
