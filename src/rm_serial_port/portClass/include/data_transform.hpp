#ifndef SERIAL_PORT__DATA_TRANSFORM_HPP_
#define SERIAL_PORT__DATA_TRANSFORM_HPP_
#include <string.h>

// c++
#include <iostream>
#include <vector>
#include <string>

// opencv
#include <opencv2/opencv.hpp>

// ros
#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>

// eigen
#include <Eigen/Core>

// #include "../../global_user/include/global_user/global_user.hpp"
#include "crc_check.hpp"
using namespace std;
namespace rm_auto_aim
{
    /**
     * @brief 模式选择（取消视觉，自瞄，英雄吊射，小符，大符，哨兵, 前哨站旋转模式）
     *
     */
    typedef struct VisionAimData
    {
        double timestamp;
        float pitch_angle; // 俯仰角
        float yaw_angle;   // 偏航角
        float distance;    // 目标距离
        int isSwitched;    // 目标是否发生切换
        int isFindTarget;  // 当识别的图片范围内有目标且电控发来的信号不为0x00（关闭视觉）置为1，否则置0
        int isSpinning;    // 目标是否处于陀螺状态
        int isPrediction;  // 当前预测器是否处于预测状态
        int isShooting;    // 是否开火（自动开火控制）
        Eigen::Vector3d meas_tracking_point;
        Eigen::Vector3d pred_aiming_point;
    } VisionAimData;

    typedef struct VisionNavData
    {
        Eigen::Vector3f linear_velocity;  // 线速度
        Eigen::Vector3f angular_velocity; // 角速度
    } VisionNavData;

    class DataTransform
    {
    public:
        DataTransform();
        ~DataTransform();
        // 接收
        void getQuatData(uchar *raw_data, vector<float> &quat);
        void getGyroData(uchar *raw_data, vector<float> &gyro);
        void getAccData(uchar *raw_data, vector<float> &acc);
        void getBulletSpeed(uchar *raw_data, float &bullet_speed);
        void getShootDelay(uchar *raw_data, float &shoot_delay);
        // 发送转换
        void transformData(int mode, const VisionAimData &data, uchar *trans_data);
        void transformData(int mode, const VisionNavData &data, uchar *trans_data);

        std::unique_ptr<CrcCheck> crc_check_;
        rclcpp::Logger logger_;
        rclcpp::Clock steady_clock_{RCL_STEADY_TIME};

    private:
        float ucharRaw2Float(uchar *data); // 将4个uchar合并成一个float
        bool ucharRaw2FloatVector(uchar *data, int bytes, vector<float> &vec);
        // 发送的函数
        bool float2UcharRawArray(float float_data[], int num, uchar *raw_data);
        uchar *float2UcharRaw(float float_data);
    };
}

#endif