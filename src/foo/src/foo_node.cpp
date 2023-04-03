#include "foo/foo_application.hpp"

auto main(int argc, char *argv[]) -> int
{
    const std::string node_name{"foo_node"};
    ros::init(argc, argv, node_name);

    foo::Application application{};

    while (application.IsOk())
    {
        application.ProcessCallbacks();
        application.Step();
        application.Sleep();
    }

    return 0;
}
