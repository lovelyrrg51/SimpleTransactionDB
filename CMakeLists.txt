# CMake Minimum Required
cmake_minimum_required(VERSION 3.22)

# Project Name
project(Simple_Transaction)

# Include Directories
include_directories(${PROJECT_SOURCE_DIR})

# Add Executable
add_executable(transaction ${PROJECT_SOURCE_DIR}/Source/transaction.cpp)

