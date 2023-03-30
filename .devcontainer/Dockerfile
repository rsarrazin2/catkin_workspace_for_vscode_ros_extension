ARG VARIANT="focal"
FROM mcr.microsoft.com/vscode/devcontainers/base:0-${VARIANT}

# Configure CMake according to https://apt.kitware.com/ (since standard Ubuntu registries don't offer newer versions).
RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /usr/share/keyrings/kitware-archive-keyring.gpg >/dev/null \
    && echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ focal main' | sudo tee /etc/apt/sources.list.d/kitware.list >/dev/null \
# Update & Install all the packages
    && apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -y install --no-install-recommends kitware-archive-keyring \
    && export CMAKE_VERSION=3.24.1-0kitware1ubuntu20.04.1 \
    && apt-get -y install --no-install-recommends cmake-data=$CMAKE_VERSION cmake=$CMAKE_VERSION \
# Python 3 + pip
    && apt-get -y install --no-install-recommends python3 python3-pip \
# make
    make \
# The build uses the following elements.
    gcc g++ gdb

# Install ROS1 noetic according to http://wiki.ros.org/Installation/Ubuntu
RUN echo "deb http://packages.ros.org/ros/ubuntu $(lsb_release -sc) main" > /etc/apt/sources.list.d/ros-latest.list \
    && curl -s https://raw.githubusercontent.com/ros/rosdistro/master/ros.asc | apt-key add - \
    && export DEBIAN_FRONTEND=noninteractive && apt-get update \
    && apt-get -y install --no-install-recommends ros-noetic-desktop-full

# Install catkin, ROS1's build system
RUN export DEBIAN_FRONTEND=noninteractive && apt-get update \
    && apt-get -y install --no-install-recommends python3-catkin-tools python3-osrf-pycommon

# Patch Google Test to avoid deprecation warning when building with catkin
RUN sed -i.bkp "s/cmake_minimum_required(.*)/cmake_minimum_required(VERSION 3.8)/" /usr/src/googletest/CMakeLists.txt \
    && sudo sed -i.bkp "s/cmake_minimum_required(.*)/cmake_minimum_required(VERSION 3.8)/" /usr/src/googletest/googlemock/CMakeLists.txt \
    && sudo sed -i.bkp "s/cmake_minimum_required(.*)/cmake_minimum_required(VERSION 3.8)/" /usr/src/googletest/googletest/CMakeLists.txt
