import rclpy
from rclpy.node import Node
from auto_aim_interfaces.msg import Uart
import subprocess
import signal

class BagRecorder(Node):
    def __init__(self):
        super().__init__('bag_recorder')
        self.subscription = self.create_subscription(
            Uart,
            '/uart',  # 订阅UART消息的话题名称
            self.uart_callback,
            10
        )
        self.bag_process = None

    def uart_callback(self, msg):
        stage = msg.stage
        if stage == 4:
            # 启动录制
            if self.bag_srocess is None:
                self.start_bag_recording()
        elif stage == 5:
            # 停止录制
            if self.bag_process is not None:
                self.stop_bag_recording()

    def start_bag_recording(self):
        # 构建录制命令，假设要录制的topic名称为 topic1 和 topic2
        record_command = ['ros2', 'bag', 'record', '/livox/imu', '/livox/lidar']
        
        # 启动ROS 2 Bag录制
        self.bag_process = subprocess.Popen(record_command)

    def stop_bag_recording(self):
        # 发送SIGINT信号给进程，模拟Ctrl+C
        if self.bag_process is not None:
            self.bag_process.send_signal(signal.SIGINT)

def main():
    rclpy.init()
    node = BagRecorder()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()
