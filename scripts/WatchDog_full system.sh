#!/bin/bash
export ROS_HOME="/home/tup/TUP2024-Sentry-Framework"
cd /home/tup/TUP2024-Sentry-Framework
# 设置需要启动和监视的ros命令
declare -a commands=(
  "ros2 launch ig_lio_c map_mapping_launch.py"
  "ros2 launch livox_ros_driver2 msg_MID360_launch.py"
  "ros2 launch nav2_bringup base_robot_launch.py"
	"ros2 launch sentry_decision_v2 launch.py"
	"ros2 run rm_serial_port rm_serial_port_node"
  )

# 定义函数来启动ros命令
start_commands() {
  for cmd in "${commands[@]}"
  do
	gnome-terminal -- bash -c "  source /opt/ros/galactic/setup.bash;cd $(pwd);source install/setup.bash;$cmd;exec bash;"
  done
}

# 定义函数来监视ros进程是否在运行
watch_commands() {
  while true
  do
    for cmd in "${commands[@]}"
    do
      process_name=$(echo $cmd | awk '{print $1}') # 提取进程名
      pgrep -f "$cmd"
      if ! pgrep -f "$cmd" > /dev/null
      then
        echo "$cmd 未在运行, 重新启动"
	gnome-terminal -- bash -c "  source /opt/ros/galactic/setup.bash;cd $(pwd);source install/setup.bash;$cmd;exec bash;"
      fi
    done
    sleep 10 # 设置睡眠时间，以便在下一次检查之前给进程足够的时间运行
  done
}

# 启动ros命令和监视进程
start_commands
watch_commands

