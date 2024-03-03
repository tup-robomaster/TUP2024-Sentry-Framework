#include "../include/port_interface.hpp"
namespace rm_auto_aim
{

    PortInterface::PortInterface() : logger_(rclcpp::get_logger("serial_port"))
    {
        crc_check_ = std::make_unique<CrcCheck>();
        openPort();
    }

    bool PortInterface::openPort()
    {
        if (access("/dev/ttyACM0", 0) == -1)
        {
            RCLCPP_ERROR(logger_, "serial port not found");
            return false;
        }

        if (system(("echo " + password + " | sudo -S chmod a+rw " + "/dev/ttyACM0").c_str()) != 0) // 给超级权限
        {
            RCLCPP_ERROR(logger_, "Serial :Failed to get permission!");
            return false;
        }
        fd = open("/dev/ttyACM0", O_RDWR | O_NOCTTY); // 接入串口
        if (fd == -1)
        {
            RCLCPP_ERROR(logger_, "open port defeat");
            return false;
        }
        // 设置波特率
        struct termios Opt;
        tcgetattr(fd, &Opt);
        tcflush(fd, TCIOFLUSH);  // 清空缓冲区的内容
        cfsetispeed(&Opt, band); // 设置接受和发送的波特率
        cfsetospeed(&Opt, band);

        if (tcsetattr(fd, TCSANOW, &Opt) != 0) // 使设置立即生效
        {
            RCLCPP_WARN(logger_, "set param fail");
            return false;
        }
        tcflush(fd, TCIOFLUSH);
        // 设置数据位，停止位和校验位
        tcgetattr(fd, &Opt);
        Opt.c_cflag |= (CLOCAL | CREAD); // 接受数据
        Opt.c_cflag &= ~CSIZE;           // 设置数据位数
        Opt.c_cflag |= databits;         // 设置数据位的数量
        // 设置校验位
        Opt.c_cflag &= ~PARENB; /* Clear parity enable */
        Opt.c_iflag &= ~INPCK;  /* Enable parity checking */
        // 设置停止位
        Opt.c_cflag &= ~CSTOPB; // 1位
        // 余下设置
        tcflush(fd, TCIFLUSH);                          // 清除输入缓存区
        Opt.c_cc[VTIME] = 150;                          // 设置超时15 seconds
        Opt.c_cc[VMIN] = 0;                             // 最小接收字符
        Opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Input原始输入
        Opt.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
        Opt.c_iflag &= ~(ICRNL | IGNCR);
        Opt.c_oflag &= ~OPOST; // Output禁用输出处理

        if (tcsetattr(fd, TCSANOW, &Opt) != 0) /* Update the options and do it NOW */
        {
            RCLCPP_WARN(logger_, "set param fail");
            return false;
        }
        RCLCPP_INFO(logger_, "open port success");
        return true;
    }
    void PortInterface::closePort()
    {
        close(fd);
    }
    bool PortInterface::readData(int bytes_num)
    {
        int bytes;
        bytes = read(fd, rdata, (size_t)(bytes_num));
        if ((rdata[0] == 0xA5 || rdata[0] == 0xB5 || rdata[0] == 0xC5) && crc_check_->Verify_CRC8_Check_Sum(rdata, 3) && crc_check_->Verify_CRC16_Check_Sum(rdata, (uint32_t)(bytes_num)))
        {
            return true;
        }
        // cout<<bytes<<endl;
        // while (bytes < 64)
        // {
        //     closePort();
        // RCLCPP_ERROR(logger_, "serial port lost");
        //     openPort();
        //     if (fd != -1)
        //         break;
        // }
        return false;
    }
        void PortInterface::writeData(int bytes_num)
    {
        auto write_stauts = write(fd, Tdata, bytes_num);
        RCLCPP_WARN_THROTTLE(logger_, steady_clock_, 50, "write msg success");
        return;
    }
}