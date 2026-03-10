cmake_minimum_required(VERSION 4.2 )
project(projection LANGUAGES C CXX CUDA)

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
set(exe_Name ${PROJECT_NAME})
add_executable(${exe_Name} src/main.c)

# Make library from files
file(GLOB cFiles ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/*c)
file(GLOB cudaFiles ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/*cu)
add_library(promise ${cFiles} ${cudaFiles} )

# We need to explicitly state that we need all CUDA files in the 
# promise library to be built with -dc as the member functions 
# could be called by other libraries and the executable
set_target_properties(promise PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

#Link libs to main library.
target_link_libraries(
	promise
	PUBLIC
	SDL3::SDL3
)
# Guide include search
target_include_directories(promise PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/include)

# Link main library to exe_Name
target_link_libraries(${exe_Name} PRIVATE promise)


