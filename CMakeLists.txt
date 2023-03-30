cmake_minimum_required(VERSION 2.8.3)
project(sm_ur5)

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
  smacc 
  ros_timer_client
  move_group_interface_client 
  ## move_base_z_client_plugin 
  moveit_ros_planning_interface 
  control_msgs
  multi_robot_control
  moveit_visual_tools
  robotiq_2f_gripper_control
)

###################################
## catkin specific configuration ##
###################################
catkin_package(
  CATKIN_DEPENDS
  multi_robot_control
)

###########
## Build ##
###########
set(CMAKE_CXX_STANDARD 17)
add_compile_options(-std=c++11) #workaround for ubuntu 16.04, to extinguish

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ executable
add_executable(${PROJECT_NAME}_node 
                    src/sm_ur5_node.cpp
                    src/clients/pick_and_place_client/cl_ur5_pick_and_place.cpp
                    src/clients/pick_and_place_client/client_behavors/cb_move_predefined.cpp
                    src/clients/pick_and_place_client/client_behavors/cb_move_cartesian.cpp
                    src/clients/pick_and_place_client/client_behavors/cb_publish_state.cpp
                    )

## Specify libraries to link a library or executable target against
target_link_libraries(
  ${PROJECT_NAME}_node
  ${catkin_LIBRARIES}
  ${Boost_LIBRARIES}
)

#############
## Install ##
#############

## Mark executables for installation
install(TARGETS ${PROJECT_NAME}_node
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

## Mark other files for installation (e.g. launch and config files, etc.)
install(FILES
  launch/sm_ur5.launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
)

## install(DIRECTORY
##     launch/
##     DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
## )

install(DIRECTORY
  config/
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/config
)
