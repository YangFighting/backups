#include <geometry_msgs/Twist.h>
#include <ros/ros.h>
#include <string.h>
#include <tf/transform_listener.h>

int main(int argc, char **argv) {
  ros::init(argc, argv, "get_odom_poses");

  ros::NodeHandle node;

  tf::TransformListener listener;
  std::string base_frame = "base_link";
  std::string odom_frame = "odom";
  ros::Rate rate(10.0);
  while (node.ok()) {
    tf::StampedTransform transform;

    try {

      listener.lookupTransform(base_frame, odom_frame, ros::Time(0), transform);
    } catch (tf::TransformException &ex) {
      ROS_ERROR("%s", ex.what());
      ros::Duration(1.0).sleep();
      continue;
    }

    double yaw = tf::getYaw(transform.getRotation());
    double x = transform.getOrigin().x();
    double y = transform.getOrigin().y();
    ROS_INFO("x: %.3f y: %.3f the: %.3f rad (%.3f deg )", x, y, yaw,yaw/M_PI*180.0);

    rate.sleep();
  }
  return 0;
};