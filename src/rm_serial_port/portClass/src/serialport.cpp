#include "../include/serialport.hpp"
namespace rm_auto_aim
{
    Serialport::Serialport() : logger_(rclcpp::get_logger("serial_port"))
    {
        port_interface_ = std::make_unique<PortInterface>();
        data_transform_ = std::make_unique<DataTransform>();
    }
    void Serialport::receiveData(auto_aim_interfaces::msg::Serial &serial_msg)
    {
        bool is_receive_data = false;
        int time = 0;
        while (!is_receive_data) // 接收一次数据
        {

            // time++;
            // while (time > 10)
            // {
            //     RCLCPP_ERROR(logger_, "serial lost");
            //     port_interface_->closePort();
            //     if (port_interface_->openPort())
            //     {
            //         time = 0;
            //         break;
            //     }
            // }
            mutex_.lock();
            is_receive_data = port_interface_->readData();
            mutex_.unlock();
            if (!is_receive_data)
            {

                usleep(1000);
                continue;
            }
        }
        RCLCPP_INFO_THROTTLE(logger_, steady_clock_, 50, "get data success");
        // 取出信息
        u_char flag = port_interface_->rdata[0];
        u_char mode = port_interface_->rdata[1];
        mode_ = mode;
        RCLCPP_INFO_THROTTLE(logger_, steady_clock_, 50, "mode%d", mode);
        if (flag == 0xA5 || flag == 0xC5)
        {
            if (flag == 0xA5)
            {
                serial_msg.imu.header.frame_id = "right";
            }
            else
            {
                serial_msg.imu.header.frame_id = "left";
            }
            std::vector<float> quat;  // 四元数
            std::vector<float> gyro;  // 角速度
            std::vector<float> acc;   // 加速度
            float bullet_speed = 0.0; // 弹速
            float shoot_delay = 0.0;  // 射击延迟
            data_transform_->getQuatData(&port_interface_->rdata[3], quat);
            data_transform_->getGyroData(&port_interface_->rdata[19], gyro);
            data_transform_->getAccData(&port_interface_->rdata[31], acc);
            data_transform_->getBulletSpeed(&port_interface_->rdata[43], bullet_speed);
            data_transform_->getShootDelay(&port_interface_->rdata[47], shoot_delay);
            ///
            rclcpp::Time now = steady_clock_.now();
            // serial_msg.header.frame_id = "serial";
            serial_msg.header.stamp = now;
            serial_msg.imu.header.stamp = now;
            serial_msg.mode = mode;
            serial_msg.bullet_speed = bullet_speed;
            serial_msg.shoot_delay = shoot_delay;
            serial_msg.imu.orientation.w = quat[0];
            serial_msg.imu.orientation.x = quat[1];
            serial_msg.imu.orientation.y = quat[2];
            serial_msg.imu.orientation.z = quat[3];
            serial_msg.imu.angular_velocity.x = gyro[0];
            serial_msg.imu.angular_velocity.y = gyro[1];
            serial_msg.imu.angular_velocity.z = gyro[2];
            serial_msg.imu.linear_acceleration.x = acc[0];
            serial_msg.imu.linear_acceleration.y = acc[1];
            serial_msg.imu.linear_acceleration.z = acc[2];
        }
    }
    void Serialport::sendDataA(auto_aim_interfaces::msg::Gimbal::SharedPtr &gimbal_msg)
    {
        int mode = mode_;
        VisionAimData vision_data;
        vision_data =
            {
                (steady_clock_.now().nanoseconds() / 1e6),
                (float)gimbal_msg->pitch,
                (float)gimbal_msg->yaw,
                (float)gimbal_msg->distance,
                (gimbal_msg->is_switched || gimbal_msg->is_spinning_switched),
                gimbal_msg->is_target,
                gimbal_msg->is_spinning,
                gimbal_msg->is_prediction,
                gimbal_msg->is_shooting,
                {gimbal_msg->meas_point_cam.x, gimbal_msg->meas_point_cam.y, gimbal_msg->meas_point_cam.z},
                {gimbal_msg->pred_point_cam.x, gimbal_msg->pred_point_cam.y, gimbal_msg->pred_point_cam.z}};
        data_transform_->transformData(mode, vision_data, port_interface_->Tdata);
        if (gimbal_msg->header.frame_id == "left")
            port_interface_->Tdata[0] = 0xA5;
        else
            port_interface_->Tdata[0] = 0xC5;
        mutex_.lock();
        port_interface_->writeData();
        mutex_.unlock();
    }
    void Serialport::sendDataB(geometry_msgs::msg::Twist::SharedPtr &msg)
    {
        int mode = mode_;
        VisionNavData vision_data;
        vision_data.linear_velocity[0] =7; //msg->linear.x;
        vision_data.linear_velocity[1] = 7;//msg->linear.y;
        vision_data.linear_velocity[2] = 7;//msg->linear.z;

        vision_data.angular_velocity[0] =7; //msg->angular.x;
        vision_data.angular_velocity[1] =7; //msg->angular.y;       
        vision_data.angular_velocity[2] =7;// msg->angular.z;
        data_transform_->transformData(mode, vision_data, port_interface_->Tdata);

        mutex_.lock();
        port_interface_->writeData();
        mutex_.unlock();
    }

}