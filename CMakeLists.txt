# Minimal cmake version required
cmake_minimum_required(VERSION 3.1)

# Project title
project(findContiguousRegion)

# Language version required: C++ 11
set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Source files
set(SRCS
		task_Svitla.cpp
    )

#  Executable created from ${SRCS}
add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
