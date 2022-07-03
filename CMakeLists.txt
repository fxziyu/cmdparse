cmake_minimum_required(VERSION 3.6)

project(cmdparse-v1.0)

set(cmdparse_dir ${PROJECT_SOURCE_DIR}/cmdparse)
include_directories(${cmdparse_dir})

add_subdirectory(demo/cmd_at)
add_subdirectory(demo/cmd_line)
