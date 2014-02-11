cmake_minimum_required(VERSION 2.8.3)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)   #for executables

#project name
set(project_name ants)
project(${project_name})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -O3")

file(GLOB_RECURSE source_files "src/*.cpp")
file(GLOB_RECURSE header_files "src/*.h")

add_executable(${project_name} ${header_files} ${source_files})

find_package(Featherkit REQUIRED COMPONENTS ui rendering structure messaging)
find_package(SDL REQUIRED)
find_package(GLM REQUIRED)

if(FEATHERKIT_FOUND)
    include_directories(${FEATHERKIT_INCLUDE_DIRS})
    target_link_libraries(${project_name} ${FEATHERKIT_LIBRARIES})
endif()

if(SDL_FOUND)
    include_directories(${SDL_INCLUDE_DIR})
    target_link_libraries(${project_name} ${SDL_LIBRARIES})
endif()

if(GLM_FOUND)
    include_directories(${GLM_INCLUDE_DIR})
endif()

target_link_libraries(${project_name} GL GLU GLEW sfml-window sfml-system)
