cmake_minimum_required(VERSION 3.6)

project(cmd_at)

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -Wall") 

add_executable(${PROJECT_NAME} main.c ${cmdparse_dir}/cmdparse.c)