cmake_minimum_required(VERSION 3.0.2)
project(foo)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  roslaunch
  roslib
  rostest
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

catkin_package(
  CATKIN_DEPENDS 
    roscpp
    roslib
)

set(node_name ${PROJECT_NAME}_node)
add_executable(${node_name} src/foo_node.cpp)
add_dependencies(${node_name} ${catkin_EXPORTED_TARGETS})
target_link_libraries(${node_name} ${catkin_LIBRARIES})
roslaunch_add_file_check(launch/${node_name}.launch)

set(test_name ${PROJECT_NAME}_test)
add_rostest_gmock(${test_name} test/${test_name}.test test/${test_name}.cpp)
roslaunch_add_file_check(test/${test_name}.test)

install(TARGETS ${PROJECT_NAME}_node
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)
