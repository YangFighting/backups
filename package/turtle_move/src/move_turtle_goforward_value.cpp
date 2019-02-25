
#include "ros/ros.h"
#include <geometry_msgs/Twist.h>
#include <signal.h>
#include <string.h>

ros::Publisher cmdVelPub;

void shutdown(int sig) {
  cmdVelPub.publish(geometry_msgs::Twist()); //使机器人停止运动
  ROS_INFO("move_turtle_goforward_value.cpp ended!");
  ros::shutdown();
}

int main(int argc, char **argv) {

  ros::init(argc, argv, "goforward_value");
  ros::NodeHandle node("~"); // Publisher to control the robot's speed
  std::string topic = "/mobile_base/commands/velocity";
  cmdVelPub = node.advertise<geometry_msgs::Twist>(
      topic, 1); // How fast will we update the robot's movement?

  double rate = 10;         // Set the equivalent ROS rate variable
  ros::Rate loopRate(rate); // execute a shutdown function when exiting
  signal(SIGINT, shutdown);
  ROS_INFO("move_turtle_goforward_value.cpp start...");

  // Set the forward linear speed to 0.1 meters per second
  float linear_speed = 0.1;

  float goal_distance;
  if (node.getParam("goal_distance", goal_distance)) {
    ROS_INFO("get Param goal_distance: %.3f", goal_distance);
  } else {
    goal_distance = 1.0;

    ROS_INFO("default Param goal_distance: %.3f", goal_distance);
  }

  // Set the travel distance to 1.0 meters
  // float goal_distance = 1.0; // How long should it take us to get there?
  float linear_duration =
      goal_distance /
      linear_speed; // Set the rotation speed to 1.0 radians per second

  int ticks;
  geometry_msgs::Twist speed; // 控制信号载体 Twist message
  if (ros::ok()) {
    speed.linear.x = linear_speed; // 设置线速度，正为前进，负为后退 // Move
                                   // forward for a time to go 1 meter
    speed.angular.z = 0;
    ticks = int(linear_duration * rate);

    for (int i = 0; i < ticks; i++) {
      
      cmdVelPub.publish(speed); // 将刚才设置的指令发送给机器人
      loopRate.sleep();
    } // Stop the robot before the rottion
    cmdVelPub.publish(geometry_msgs::Twist());
  }
  return 0;
}
