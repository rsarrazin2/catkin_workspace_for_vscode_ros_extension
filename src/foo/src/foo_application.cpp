#include "foo/foo_application.hpp"

#include <iostream>

namespace foo
{
    void Application::Step()
    {
        std::cout << "foo::Application::Step()" << std::endl;

        if (poor_mans_sync_)
        {
            std_msgs::Bool message{};
            message.data = 1U;
            publisher_.publish(message);
        }
    }
} // namespace foo
