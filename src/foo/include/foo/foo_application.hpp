#ifndef FOO_FOO_APPLICATION_HPP
#define FOO_FOO_APPLICATION_HPP

#include "ros/callback_queue.h"
#include "ros/ros.h"

#include "std_msgs/Bool.h"

#include <iostream>

namespace foo
{
    class Application
    {
    public:
        static void ProcessCallbacks() { ros::spinOnce(); }
        void Sleep() { loop_rate_.sleep(); }
        void Step();

        static auto IsOk() -> bool { return ros::ok(); }

    private:
        ros::NodeHandle node_{};

        const std::string publisher_topic_name_{"/foo/bar"};
        const ros::Publisher publisher_{
            node_.advertise<std_msgs::Bool>(publisher_topic_name_, 1)};

        const std::function<void(const std_msgs::BoolConstPtr &)> subscribe_message_ =
            [&](const std_msgs::BoolConstPtr &message) -> void
        {
            std::ignore = message;

            std::cout << "foo::Application::subscribe_message_" << std::endl;

            poor_mans_sync_ = true;
        };

        const std::string subscription_topic_name_{"/foo/foo"};
        const ros::Subscriber subscriber_{
            node_.subscribe<std_msgs::Bool>(subscription_topic_name_, 1, subscribe_message_)};

        ros::Rate loop_rate_{50.0};

        bool poor_mans_sync_{false};
    };

} // namespace foo

#endif
