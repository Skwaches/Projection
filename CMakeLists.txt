cmake_minimum_required(VERSION 4.2)
project(projection)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Set output directory for build objects.
#CMAKE_BINARY_DIR is the directory from which the command was run (I think)
set(VENDOR_DIR "../Vendor")

option(VENDORED "Use personal, vendored libraries" ON)
if(VENDORED)
	add_subdirectory("${VENDOR_DIR}/SDL" SDL)
else()
	find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif()

# Make library from c files
file(GLOB cFiles "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/*c")
add_library(personal ${cFiles})
# Link files
target_link_libraries(
	personal 
	PUBLIC
	SDL3::SDL3
)
# Guide include search
target_include_directories(personal PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/include")

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE personal)


