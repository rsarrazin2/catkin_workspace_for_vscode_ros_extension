cmake_minimum_required(VERSION 3.8)
project(foo)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Werror -Wall -Wextra -Wpedantic -Wconversion)
endif()

find_package(
  catkin REQUIRED
  COMPONENTS roscpp
             roslaunch
             rostest
             std_msgs)

catkin_package(
  CATKIN_DEPENDS
  roscpp
  std_msgs)

set(application_name ${PROJECT_NAME}_application)
set(node_name ${PROJECT_NAME}_node)
set(test_name ${PROJECT_NAME}_application_test)

add_library(${application_name} src/${application_name}.cpp)
target_include_directories(${application_name} PUBLIC include
                                                      ${catkin_INCLUDE_DIRS})
target_link_libraries(${application_name} ${catkin_LIBRARIES})

add_executable(${node_name} src/${node_name}.cpp)
target_link_libraries(${node_name} ${application_name})

add_rostest_gtest(${test_name} test/${test_name}.test
                  test/${test_name}.cpp)

target_link_libraries(${test_name} ${application_name})

roslaunch_add_file_check(launch/${node_name}.launch)
roslaunch_add_file_check(test/${test_name}.test)

install(TARGETS ${node_name}
        RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
