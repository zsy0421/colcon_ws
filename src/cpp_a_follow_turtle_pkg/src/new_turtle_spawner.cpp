#include <rclcpp/rclcpp.hpp>
#include <turtlesim/srv/spawn.hpp>

using namespace std;

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("turtle_spawner");
    RCLCPP_INFO(node->get_logger(), "create turtle spawner");

    auto spawnerClient = node->create_client<turtlesim::srv::Spawn>("/spawn");

    auto request = std::make_shared<turtlesim::srv::Spawn::Request>();
    request->name = "turtle2";
    request->x = 1.5;
    request->y = 2.0;
    request->theta = 0.0;

    while (!spawnerClient->wait_for_service(1s)) {
        if (!rclcpp::ok()) {
            RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service");
            return -1;
        }
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service not available, waiting again...");
    }

    auto result = spawnerClient->async_send_request(request);
    if (rclcpp::spin_until_future_complete(node, result) == rclcpp::FutureReturnCode::SUCCESS) {
        auto r = result.get();
        RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "result got, %s created successfully!", r->name.c_str());
    } else {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "failed to get result, turtle created failed!");
    }

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
