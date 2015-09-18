#include "ros/ros.h"
#include <std_msgs/Float64.h>
#include <geometry_msgs/PointStamped.h>
#include <tf/transform_listener.h>
#include <nav_msgs/OccupancyGrid.h>

ros::Publisher waypoint_pub;

geometry_msgs::PointStamped goal;
geometry_msgs::PointStamped pose;
geometry_msgs::PointStamped waypoint;
bool planning=false;
bool planned=false;

void stop(){
	waypoint=pose;
}

void plan(){
	planning=true;
	stop();
	//TODO

	planned=true;
	planning=false;
}

bool checkFeasibility(){
	if(!planned)
		return true;
	//TODO

	//remove first waypoint if nescessary
	if(sqrt((pose.point.x-waypoint.point.x)*(pose.point.x-waypoint.point.x)+(pose.point.y-waypoint.point.y)*(pose.point.y-waypoint.point.y))<0.5){

	}
	//check feasibility

}

void sendWaypoint(){
	if(planned)
		waypoint_pub.publish(waypoint);
}

void poseCallback(const geometry_msgs::PointStamped::ConstPtr& msg){
	if(planning)
		return;
	//update local pose
	 pose.header.frame_id=msg->header.frame_id;
	 pose.header.stamp=ros::Time(0);
	 pose.point.x=msg->point.x;
	 pose.point.y=msg->point.y;

	if(checkFeasibility()){
		plan();
	}
	sendWaypoint();
}

void mapCallback(const nav_msgs::OccupancyGrid::ConstPtr& msg){
	if(planning)
		return;
	//inflate

	//update local map

	if(checkFeasibility()){
		plan();
	}
	sendWaypoint();
}

void goalCallback(const geometry_msgs::PointStamped::ConstPtr& msg){
	if(planning)
		return;
	//update local goal
	std::cout << "receiving goal update " << msg->point.x << "," << msg->point.y << std::endl;
	goal.header.frame_id=msg->header.frame_id;
	goal.header.stamp=ros::Time(0);
	goal.point.x=msg->point.x;
	goal.point.y=msg->point.y;

	plan();
	sendWaypoint();
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "waypoint_tracker");
  ros::NodeHandle n;
  ros::NodeHandle nh_rel=ros::NodeHandle("~");

  waypoint_pub = n.advertise<geometry_msgs::PointStamped>("/waypoint", 10);

  ros::Subscriber goal_sub = n.subscribe("/goal_pose", 10, goalCallback);
  ros::Subscriber pose_sub = n.subscribe("/slam_out_pose", 10, poseCallback);
  ros::Subscriber map_sub = n.subscribe("/map", 10, mapCallback);



  ros::spin();

  return 0;
}
