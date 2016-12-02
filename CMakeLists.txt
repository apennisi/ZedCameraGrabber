project(zedcameragrabber)

SET(execName grabber)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-std=c++11 -g -O3")

SET(VERSION_REQ_CUDA "8.0")

find_package(ZED REQUIRED)
find_package(Eigen3 REQUIRED)

##For Jetson, OpenCV4Tegra is based on OpenCV2.4
exec_program(uname ARGS -p OUTPUT_VARIABLE CMAKE_SYSTEM_NAME2)

set(CUDA_USE_STATIC_CUDA_RUNTIME OFF)

find_package(OpenCV 3.1 REQUIRED)
find_package(CUDA ${VERSION_REQ_CUDA} REQUIRED)
#find_package(PCL 1.7 REQUIRED)
#include_directories(${PCL_INCLUDE_DIRS})
#link_directories(${PCL_LIBRARY_DIRS})
#add_definitions(${PCL_DEFINITIONS})

#list(REMOVE_ITEM PCL_LIBRARIES "vtkproj4")

include_directories(${CUDA_INCLUDE_DIRS})
include_directories(${ZED_INCLUDE_DIRS})
include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${PROJECT_BINARY_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
#set( EIGEN3_INCLUDE_DIR "$ENV{EIGEN3_INCLUDE_DIR}" )
#include_directories ( ${EIGEN3_INCLUDE_DIR} )

link_directories(${ZED_LIBRARY_DIR})
#link_directories(${OpenCV_LIBRARY_DIRS})
#link_directories(${CUDA_LIBRARY_DIRS})

SET(SRC_FOLDER src)
SET(EXECUTABLE_OUTPUT_PATH ../bin)

FILE(GLOB_RECURSE SRC_FILES "${SRC_FOLDER}/*.cpp")

add_executable(${execName} ${SRC_FILES})

#add_definitions(-std=c++0x -g -O3)

TARGET_LINK_LIBRARIES( ${execName}
                         -lm -lpthread ${ZED_LIBRARIES}
                        ${OpenCV_LIBRARIES} #${PCL_LIBRARIES}
			${CUDA_CUDA_LIBRARY} ${CUDA_CUDART_LIBRARY} ${CUDA_npp_LIBRARY} 
                        )
