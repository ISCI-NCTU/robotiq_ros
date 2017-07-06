cmake_minimum_required(VERSION 2.8.2)
project(robotiq_ros)

find_package(catkin REQUIRED COMPONENTS roscpp)
find_package(Boost REQUIRED COMPONENTS system)

catkin_package(
	#INCLUDE_DIRS include 
	#LIBRARIES no
	CATKIN_DEPENDS roscpp
	DEPENDS Boost
	)

#include_directories(
#	include 
#	${catkin_INCLUDE_DIRS} 
#	${Boost_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "-std=c++11")

add_executable(robotiq_850 src/robotiq850.cpp)

add_dependencies(robotiq_850 robotiq_850_generate_messages_cpp)

target_link_libraries(robotiq_850 
	${catkin_LIBRARIES} 
	${Boost_LIBRARIES})

