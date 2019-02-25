#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include <signal.h>
ros::Publisher cmdVelPub;
void shutdown(int sig) {
  cmdVelPub.publish(geometry_msgs::Twist()); //使机器人停止运动
  ROS_INFO("move_turtle_anticlockwise_rotate_value ended!");
  ros::shutdown();
}
int main(int argc, char **argv) {
  ros::init(argc, argv,
            "anticlockwise_rotate_value"); //初始化ROS,它允许ROS通过命令行进行名称重映射
  ros::NodeHandle node("~");
  cmdVelPub = node.advertise<geometry_msgs::Twist>(
      "/mobile_base/commands/velocity",
      1);                   //在/mobile_base/commands/velocity
                            //topic上发布一个geometry_msgs/Twist的消息
  double rate = 50;
  ros::Rate loopRate(rate); //ros::Rate对象可以允许你指定自循环的频率

  signal(SIGINT, shutdown);
  ROS_INFO("move_turtle_anticlockwise_rotate_value cpp start...");

  float angular_speed = 0.5;    // Set the rotation angle to Pi radians (180 degrees)

  float goal_angle;

  if (node.getParam("goal_angle", goal_angle)) {
    goal_angle = goal_angle/180*M_PI;
    ROS_INFO("getParam goal_angle: %.3f rad (%.3f deg )", goal_angle,
             goal_angle / M_PI * 180);
  } else {
    goal_angle = M_PI;
    ROS_INFO("default Param goal_angle: %.3f rad (%.3f deg )", goal_angle,
             goal_angle / M_PI * 180);
  }

  float angular_duration = goal_angle / angular_speed;
  geometry_msgs::Twist speed; // 控制信号载体 Twist message
  int ticks;
  if (ros::ok()) {
    speed.linear.x = 0; // 设置线速度为0m/s，正为前进，负为后退
    speed.angular.z = angular_speed; // 设置角速度为0.1 rad/s，正为左转，负为右转
    
    ticks = int(angular_duration * rate);
    for (int i = 0; i < ticks; i++) {
      cmdVelPub.publish(speed); // 将刚才设置的指令发送给机器人
      loopRate.sleep();
    }
    cmdVelPub.publish(geometry_msgs::Twist());
  }
  return 0;
}
