# Minimal cmake version required
cmake_minimum_required(VERSION 3.1)

# Project title
project(findContiguousRegion)

# Language version required: C++ 11
set(CMAKE_CXX_STANDARD 11)
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
#SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Source files
set(SRCS
		task_Svitla.cpp
    )

#  Executable created from ${SRCS}
add_executable(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})
