#ifndef SERIAL_PORT__PORT_INTERFACE_HPP_
#define SERIAL_PORT__PORT_INTERFACE_HPP_
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
#include "crc_check.hpp"
using namespace std;
// const std::vector<std::string> DEFAULT_PORT = {"ttyUSB", "ttyACM"}; // 默认串口名
const int band = B115200;
const int databits = CS8;
const int stopbits = 1;
const int parity = 'N';

constexpr int MAX_ITER = 20; // 默认串口最大编号
namespace rm_auto_aim
{
    class PortInterface // 直接和ubuntu文件相连
    {
        string password = "sys666";
        std::unique_ptr<CrcCheck> crc_check_;
        rclcpp::Logger logger_;
        rclcpp::Clock steady_clock_{RCL_STEADY_TIME};

    public:
        int fd;
        unsigned char rdata[64];          // raw_data接收数据
        unsigned char Tdata[64] = {0x00}; // 发送数据包A
        // unsigned char Bdata[64] = {0x00}; // 发送数据包B
        PortInterface();
        // 收发
        bool readData(int bytes_num = 64);
        void writeData(int bytes_num = 64);
        // 开合
        bool openPort();
        void closePort();
    };
}
#endif