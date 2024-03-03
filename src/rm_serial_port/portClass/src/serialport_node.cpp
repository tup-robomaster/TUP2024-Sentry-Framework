#include "../include/serialport_node.hpp"
namespace rm_auto_aim
{
    SerialPortNode::SerialPortNode(const rclcpp::NodeOptions &options) : Node("serial_port", options)
    {

        RCLCPP_WARN(this->get_logger(), "Serialport node...");
        serial_port_ = std::make_unique<Serialport>();
        rclcpp::QoS qos(0);
        qos.keep_last(5);
        qos.reliable();
        qos.durability();
        qos.deadline();
        auto_aim_sub_ = this->create_subscription<auto_aim_interfaces::msg::Gimbal>(
            "/armor_processor/gimbal_msg",
            qos,
            std::bind(&SerialPortNode::gimbalMsgCallback, this, _1));

        sentry_twist_sub_ = this->create_subscription<geometry_msgs::msg::Twist>(
            "/cmd_vel",
            qos,
            std::bind(&SerialPortNode::sentryNavCallback, this, _1));

        serial_msg_pub_ = this->create_publisher<auto_aim_interfaces::msg::Serial>("/serial_port/serial_msg", qos);
        loop_send_thread_ = std::make_unique<std::thread>(&SerialPortNode::serialMsgLoopsend, this);
    }

    SerialPortNode::~SerialPortNode()
    {
    }

    void SerialPortNode::gimbalMsgCallback(auto_aim_interfaces::msg::Gimbal::SharedPtr gimbal_msg)
    {
        cout << 1 << endl;
        serial_mutex_.lock();
        serial_port_->sendDataA(gimbal_msg);
        serial_mutex_.unlock();
    }

    void SerialPortNode::serialMsgLoopsend()
    {
        while (rclcpp::ok())
        {
            serial_port_->receiveData(serial_msg_);
            serial_msg_pub_->publish(std::move(serial_msg_));

            geometry_msgs::msg::Twist::SharedPtr msg;
        }
    }

    void SerialPortNode::sentryNavCallback(geometry_msgs::msg::Twist::SharedPtr msg)
    {
        serial_mutex_.lock();

        serial_port_->sendDataB(msg);

        serial_mutex_.unlock();
    }

} // namespace rm_auto_aim
int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<rm_auto_aim::SerialPortNode>());
    rclcpp::shutdown();
    return 0;
}

#include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(rm_auto_aim::SerialPortNode)