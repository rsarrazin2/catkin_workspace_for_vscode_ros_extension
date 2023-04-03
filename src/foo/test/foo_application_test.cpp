#include "foo/foo_application.hpp"

#include <atomic>

#include <gtest/gtest.h>

namespace
{
    const char *const kTestNodeName{"foo_application_test"};

    const char *const kTesteeInputTopicName{"/foo/foo"};
    const char *const kTesteeOutputTopicName{"/foo/bar"};
} // namespace

namespace infrastructure_provider
{
    auto GetNode() -> ros::NodeHandle &
    {
        static ros::NodeHandle node_handle{};

        return node_handle;
    }

    auto GetPublisher() -> const ros::Publisher &
    {
        assert(ros::ok());

        static const ros::Publisher kPublisher{
            GetNode().advertise<std_msgs::Bool>(kTesteeInputTopicName, 1)};
        return kPublisher;
    }

    auto WaitForCallbacks(const std::int32_t number_of_callbacks_to_wait_for) -> void
    {
        for (std::int32_t i{0}; i < number_of_callbacks_to_wait_for; i++)
        {
            while (ros::getGlobalCallbackQueue()->isEmpty())
            {
                ros::WallDuration(0, 1000).sleep();
            }
            ros::getGlobalCallbackQueue()->callOne();
        }

        ASSERT_TRUE(ros::getGlobalCallbackQueue()->isEmpty());
    }

    auto WaitForTheTesteeToSubscribe() -> void
    {
        ASSERT_TRUE(ros::ok());

        while (GetPublisher().getNumSubscribers() == 0U)
        {
            ros::spinOnce();
        }
    }

    auto HasCallbackBeenInvoked() -> std::atomic<bool> &
    {
        static std::atomic<bool> has_callback_been_invoked{false};
        return has_callback_been_invoked;
    }

    auto WaitForCallbackToBeInvoked() -> void
    {
        while (!HasCallbackBeenInvoked())
        {
            ros::spinOnce();
        }

        HasCallbackBeenInvoked() = false;
    }

    auto GetExpectedMessage() -> std_msgs::Bool &
    {
        static std_msgs::Bool message{};
        return message;
    }
} // namespace infrastructure_provider

namespace
{
    auto Callback(const std_msgs::BoolConstPtr &actual_message) -> void
    {
        const auto &expected_message = infrastructure_provider::GetExpectedMessage();

        EXPECT_EQ(actual_message->data, expected_message.data);

        infrastructure_provider::HasCallbackBeenInvoked() = true;
    }
} // namespace

struct TestSuite : ::testing::Test
{
    void SetUp() override
    {
        ASSERT_TRUE(ros::ok());

        infrastructure_provider::WaitForTheTesteeToSubscribe();
    }

    void PublishAndExpect(const std_msgs::Bool &message_to_be_published,
                          const std_msgs::Bool &expected_message)
    {
        infrastructure_provider::GetExpectedMessage() = expected_message;

        infrastructure_provider::GetPublisher().publish(message_to_be_published);
        // infrastructure_provider::WaitForCallbacks(1);

        while (ros::getGlobalCallbackQueue()->isEmpty())
        {
            application.Sleep();
        }

        application.ProcessCallbacks();
        application.Step();

        // Note: If the tested node doesn't send any message, the test will eventually fail on a 1-minute timeout.
        infrastructure_provider::WaitForCallbackToBeInvoked();
    }

    foo::Application application{};
};

TEST_F(TestSuite, misc)
{
    std_msgs::Bool expected_message{};
    expected_message.data = 1U;

    std_msgs::Bool message_to_publish{};
    message_to_publish.data = 0U;

    PublishAndExpect(message_to_publish, expected_message);
}

auto main(int argc, char *argv[]) -> int
{
    testing::InitGoogleTest(&argc, argv);

    ros::init(argc, argv, kTestNodeName);

    const ros::Subscriber subscriber{
        infrastructure_provider::GetNode().subscribe(kTesteeOutputTopicName, 1U, Callback)};

    return RUN_ALL_TESTS();
}
