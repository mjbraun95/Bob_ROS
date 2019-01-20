#include <ros/ros.h>
#include <ar_track_alvar_msgs/AlvarMarkers.h>
#include <bob_ros/RobotPose.h>

void poseCallback(const ar_track_alvar_msgs::AlvarMarkers::ConstPtr& msg) {
    
    if (msg->markers.size() > 0) {
        ROS_INFO("Markers found!");
        for (int i = 0; i < msg->markers.size(); i++) {
            ROS_INFO("Marker ID: %d", msg->markers[i].id);
            ROS_INFO("Confidence: %d", msg->markers[i].confidence);
            ROS_INFO("Position --> X: %.2f Y: %.2f Z: %.2f",
                    msg->markers[i].pose.pose.position.x,
                    msg->markers[i].pose.pose.position.y,
                    msg->markers[i].pose.pose.position.z);

        }
    }
}


int main(int argc, char** argv) {
    ros::init(argc, argv, "pose_decider");

    ros::NodeHandle node;

    ros::Subscriber header_sub = node.subscribe("/ar_pose_marker", 100, poseCallback);

    ros::spin();

    return 0;
}
