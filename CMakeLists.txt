cmake_minimum_required(VERSION 4.2 )
project(projection LANGUAGES C CUDA)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(VENDOR_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../.Vendor)

# Find dependencies
option(VENDORED "Use personal, vendored libraries" ON)
if(VENDORED)
	add_subdirectory(${VENDOR_DIR}/SDL SDL)
else()
	find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif()

# Create executable
set(executable ${PROJECT_NAME})

# Make library from files
add_executable(${executable}
	src/main.c
	src/circle.c
	src/config.c
	src/cuboid.c
	src/render.c
	src/update.c
	src/update.cu
	src/init.c
	src/input.c
)

# Guide include search
target_include_directories( ${executable} 
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include)

# We need to explicitly state that we need all CUDA files in the 
# promise library to be built with -dc as the member functions 
# could be called by other libraries and the executable
set_property(TARGET ${executable} PROPERTY CUDA_SEPARABLE_COMPILATION ON)

# Link main library to exe_Name
target_link_libraries(${executable} PRIVATE SDL3::SDL3)


