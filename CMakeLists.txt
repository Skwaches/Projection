cmake_minimum_required(VERSION 4.2)
project(projection)

option(VENDORED "Use personal, vendored libraries" ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#Set output directory for build objects.
#CMAKE_BINARY_DIR is the directory from which the command was run (I think)
set(VENDOR_DIR "../Vendor")
if(VENDORED)
	add_subdirectory("${VENDOR_DIR}/SDL" SDL)
else()
	find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
endif()

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)


