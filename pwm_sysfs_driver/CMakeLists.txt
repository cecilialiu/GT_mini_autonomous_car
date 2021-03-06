cmake_minimum_required(VERSION 2.8.3)
project(pwm_sysfs_driver)

# check c++11 / c++0x
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "-std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "-std=c++0x")
else()
    message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  pluginlib
  nodelet
  racecar
)

## System dependencies are found with CMake's conventions
find_package(Boost REQUIRED COMPONENTS)

## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend and a run_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependencies might have been
##     pulled in transitively but can be declared for certainty nonetheless:
##     * add a build_depend tag for "message_generation"
##     * add a run_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   std_msgs
# )

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
 INCLUDE_DIRS include
 LIBRARIES pwm_sysfs_driver pwm_sysfs_driver_nodelet servo_driver_nodelet
 CATKIN_DEPENDS roscpp std_msgs pluginlib nodelet racecar
 DEPENDS boost
)

###########
## Build ##
###########

## pwm sysfs driver debugging
#add_definitions(-DPWM_SYSFS_DRIVER_DEBUG=1 -DPWM_SYSFS_DRIVER_WRITE_AND_VERIFY=1)

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
include_directories(
  include ${Boost_INCLUDE_DIRS} ${catkin_INCLUDE_DIRS}
)

# standalone library
add_library(pwm_sysfs_driver
  src/${PROJECT_NAME}/pwm_sysfs_driver.cpp
)
target_link_libraries(pwm_sysfs_driver)

# pwm_sysfs_driver_nodelet
add_library(pwm_sysfs_driver_nodelet
  src/pwm_sysfs_driver_node.cpp 
  src/pwm_sysfs_driver_nodelet_main.cpp
)
add_dependencies(pwm_sysfs_driver_nodelet pwm_sysfs_driver)
target_link_libraries(pwm_sysfs_driver_nodelet
  pwm_sysfs_driver
  ${catkin_LIBRARIES}
)

# pwm_sysfs_driver_node
add_executable(pwm_sysfs_driver_node
  src/pwm_sysfs_driver_node.cpp
  src/pwm_sysfs_driver_node_main.cpp
)
add_dependencies(pwm_sysfs_driver_node pwm_sysfs_driver)
target_link_libraries(pwm_sysfs_driver_node
  pwm_sysfs_driver
  ${catkin_LIBRARIES}
)

# servo_driver_nodelet
add_library(servo_driver_nodelet
  src/servo_driver_node.cpp 
  src/servo_driver_nodelet_main.cpp
)
add_dependencies(servo_driver_nodelet pwm_sysfs_driver)
target_link_libraries(servo_driver_nodelet
  pwm_sysfs_driver
  ${catkin_LIBRARIES}
)

# servo_driver_node
add_executable(servo_driver_node
  src/servo_driver_node.cpp
  src/servo_driver_node_main.cpp
)
add_dependencies(servo_driver_node pwm_sysfs_driver)
target_link_libraries(servo_driver_node
  pwm_sysfs_driver
  ${catkin_LIBRARIES}
)

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
install(TARGETS pwm_sysfs_driver
                pwm_sysfs_driver_node
                pwm_sysfs_driver_nodelet
                servo_driver_node
                servo_driver_nodelet
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

## Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

install(DIRECTORY launch
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

install(DIRECTORY plugins
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
)

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_pwm_sysfs_driver.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
