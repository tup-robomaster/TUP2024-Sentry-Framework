#ifndef SERIAL_PORT__SERIAL_PORT_HPP_
#define SERIAL_PORT__SERIAL_PORT_HPP_

// linux
#include <atomic>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <termios.h> //
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/netlink.h>
#include <string>
// c++
#include <iostream>
#include <vector>
#include <atomic>
#include <cstdint> // 包含所需的头文件uchar
// ros
#include <rclcpp/rclcpp.hpp>
#include "auto_aim_interfaces/msg/serial.hpp"
#include "auto_aim_interfaces/msg/gimbal.hpp"
#include "port_interface.hpp"
#include "data_transform.hpp"
using namespace std;
namespace rm_auto_aim
{
    class Serialport
    {
    public:
        mutex mutex_;
        atomic<int> mode_;
        std::unique_ptr<PortInterface> port_interface_;
        std::unique_ptr<DataTransform> data_transform_;
        rclcpp::Logger logger_;
        rclcpp::Clock steady_clock_{RCL_STEADY_TIME};
        Serialport();
        void receiveData(auto_aim_interfaces::msg::Serial &serial_msg);
        void sendDataA(auto_aim_interfaces::msg::Gimbal::SharedPtr &gimbal_msg);
        void sendDataB(geometry_msgs::msg::Twist::SharedPtr &msg);

    };
}
#endif