#include <rclcpp/rclcpp.hpp>
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <geometry_msgs/msg/twist.hpp>
#include <geometry_msgs/msg/transform_stamped.h>

using namespace std;

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("turtle2_controller");

    shared_ptr<tf2_ros::Buffer> buffer = make_shared<tf2_ros::Buffer>(node->get_clock());
    tf2_ros::TransformListener tfListener(*buffer);

    auto velPub = node->create_publisher<geometry_msgs::msg::Twist>("/turtle2/cmd_vel", 1000);
    rclcpp::Rate rate(10);

    while (rclcpp::ok()) {
        try {
            geometry_msgs::msg::TransformStamped transStamped = buffer->lookupTransform("turtle2", "turtle1", tf2::TimePointZero);
            geometry_msgs::msg::Twist twist;
            twist.linear.x = 0.5 * sqrt(pow(transStamped.transform.translation.x, 2) + pow(transStamped.transform.translation.y, 2));
            twist.angular.z = 4 * atan2(transStamped.transform.translation.y, transStamped.transform.translation.x);
            velPub->publish(twist);
        } catch (tf2::TransformException &e) {
            RCLCPP_WARN(node->get_logger(), "warning %s", e.what());
        }
        rate.sleep();
    }

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
