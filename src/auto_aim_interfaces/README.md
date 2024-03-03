# `auto-aim-interfaces`
全局自定义消息包
## `Custom messages`


| Message           |          Topic              |   Description |
|  ---              |          ---                |      ---      |
| Armors.msg        | /armor_detector/armor_msg   | 目标装甲板消息 
| Gimbal.msg        | /armor_processor/gimbal_msg | 云台信息
| Serial.msg        | /serial_msg                 | 串口消息
| State.msg         | /armor_processor/state_msg  | 可视化消息
| Armor.msg         | sub msg from Armors.msg     | 单个装甲板信息
| Point2f.msg       | sub msg                     | 点，二维向量

## `Introduction msg`
> - `Point2f.msg` 二维浮点
> ---
> - `Armor.msg` 装甲板信息
>> - number 装甲板数字
>> - type 装甲板大\小
>> - distance_to_image_center 装甲板到光心的距离
>> - pose 存储装甲板坐标，位姿
> ---
> - `Armors.msg` 
>> - header 
>> - get_armor 是否得到装甲板
>> - armors 多个装甲板信息
> ---
> - `Gimbal.msg`
>> - header
>> - pitch 
>> - yaw
>> - distance
>> - is_target 
>> - is_shooting
>> - …… 
> ---
> - `Serial.msg`
>> - header
>> - imu 陀螺仪位姿信息
>> - mode 
>> - bullet_speed 射速/已弃用
>> - shoot_delay 射速延迟
> ---
> - `State.msg`
>> - mode 模式
>> - center 车辆中心
>> - predict 预测角度
>> - calculate 预测击打点
>> - angle 跟随角度
>> - state 预测信息
> ---
