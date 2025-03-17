#include <rclcpp/rclcpp.hpp>
#include <turtlesim/msg/pose.hpp>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <string>
#include <iostream>

using namespace std;

rclcpp::Node::SharedPtr node;
string turtleName;

void CallBack(const turtlesim::msg::Pose::SharedPtr msg) {
    static auto tfBr = std::make_shared<tf2_ros::TransformBroadcaster>(node);
    geometry_msgs::msg::TransformStamped tfStamped;

    tfStamped.header.stamp = rclcpp::Time();
    tfStamped.header.frame_id = "world";
    tfStamped.child_frame_id = turtleName;
    tfStamped.transform.translation.x = msg->x;
    tfStamped.transform.translation.y = msg->y;
    tfStamped.transform.translation.z = 0.0;
    tf2::Quaternion qtn;
    qtn.setRPY(0.0, 0.0, msg->theta);
    tfStamped.transform.rotation.x = qtn.x();
    tfStamped.transform.rotation.y = qtn.y();
    tfStamped.transform.rotation.z = qtn.z();
    tfStamped.transform.rotation.w = qtn.w();

    tfBr->sendTransform(tfStamped);
}

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);
    node = rclcpp::Node::make_shared("turtle_pub");

    if (argc != 5) {
        RCLCPP_INFO(node->get_logger(), "args not correct!");
        return 0;
    } else {
        turtleName = argv[1];
    }

    auto pose_sub = node->create_subscription<turtlesim::msg::Pose>(turtleName + "/pose", 100, std::bind(&CallBack, std::placeholders::_1));
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
