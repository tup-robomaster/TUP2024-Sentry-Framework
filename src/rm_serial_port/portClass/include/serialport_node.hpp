#ifndef SERIAL_PORT__SERIAL_PORT_NODE_HPP_
#define SERIAL_PORT__SERIAL_PORT_NODE_HPP_
#include <iostream>
#include <rclcpp/rclcpp.hpp>
#include <rmw/qos_profiles.h>
#include <rclcpp/qos.hpp>
#include <rclcpp/publisher.hpp>
#include <rclcpp/subscription.hpp>
#include <rclcpp/duration.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <std_msgs/msg/float64.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <message_filters/subscriber.h>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2_ros/transform_broadcaster.h>
#include <geometry_msgs/msg/transform_stamped.hpp>
#include "serialport.hpp"
using namespace std::placeholders;
namespace rm_auto_aim
{
    class SerialPortNode : public rclcpp::Node
    {
        std::unique_ptr<Serialport> serial_port_;
        mutex serial_mutex_;

        // 发送serialmsg的线程和发布者
        std::unique_ptr<std::thread> loop_send_thread_;
        auto_aim_interfaces::msg::Serial serial_msg_;
        rclcpp::Publisher<auto_aim_interfaces::msg::Serial>::SharedPtr serial_msg_pub_;
        // gimbalmsg接收者
        rclcpp::Subscription<auto_aim_interfaces::msg::Gimbal>::SharedPtr auto_aim_sub_;

        rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr sentry_twist_sub_;

    public:
        SerialPortNode(const rclcpp::NodeOptions &options = rclcpp::NodeOptions());
        ~SerialPortNode();
        void gimbalMsgCallback(auto_aim_interfaces::msg::Gimbal::SharedPtr msg);
        void serialMsgLoopsend();
        // sentry
        void sentryNavCallback(geometry_msgs::msg::Twist::SharedPtr msg);
    };
}
#endif