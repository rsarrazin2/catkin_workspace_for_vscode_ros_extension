#include <ros/ros.h>

#include <iostream>

int main(int argc, char **argv)
{
    ros::init(argc, argv, "foo_node");

    std::cout << "Foo's main()" << std::endl;

    return 0;
}
