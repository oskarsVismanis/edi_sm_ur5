/*****************************************************************************************************************
 * ReelRobotix Inc. - Software License Agreement      Copyright (c) 2018-2020
 * 	 Authors: Pablo Inigo Blasco, Brett Aldrich
 *
 ******************************************************************************************************************/

//#include <move_group_interface_client/cl_movegroup.h>
#include <sm_ur5/clients/pick_and_place_client/cl_ur5_pick_and_place.h>

//namespace moveit
//{
//namespace planning_interface
//{
namespace sm_ur5
{
namespace cl_ur5_pick_and_place
{

ClUr5PickAndPlace::ClUr5PickAndPlace(std::string robot)

{
//    ros::WallDuration(5.0).sleep();

    ur5_status_msg.data = false;

    cv_status_msg.data = false;

    ros::NodeHandle nh_;
    ros::param::del("/ur5_nh_value");
    ros::param::del("/ur5_joint_names");
    ros::param::del("/ur5_root_link");

    try {
      ROS_INFO("start of try");
      if(ptr->cfg.init(robot))
      {
          ROS_INFO_STREAM("Parameters successfully read in ClUr5PickAndPlace");
          ROS_WARN_STREAM("Trying with group: "<<ptr->cfg.ARM_GROUP_NAME);
      }
      else
      {
          ROS_ERROR_STREAM("Parameters not found in ClUr5PickAndPlace");
          return;
      }

      nh_value = "/" + robot;

      ros::NodeHandle nh_(nh_value);

      // Launch movegroupinterface
      moveit::planning_interface::MoveGroupInterface::Options options(ptr->cfg.ARM_GROUP_NAME,robot+"/robot_description",nh_);

      ptr->move_group_ptr = MoveGroupPtr(new moveit::planning_interface::MoveGroupInterface(options));

//      ros::param::set("/ur5_nh_value", nh_value);
      ROS_INFO("try after setting MoveGroupPtr");

    } catch (std::exception const & e) {
//      e.what();
      ROS_WARN_STREAM(e.what());
      ROS_WARN_STREAM("Trying again without namespaces");
      robot = "CV";
      if(ptr->cfg.init(robot))
      {
          ROS_INFO_STREAM("Parameters successfully read in ClUr5PickAndPlace");
          ROS_INFO_STREAM("Trying with group: "<<ptr->cfg.ARM_GROUP_NAME);
      }
      else
      {
          ROS_ERROR_STREAM("Parameters not found in ClUr5PickAndPlace");
          return;
      }

      nh_value = "/";
      ros::NodeHandle nh_(nh_value);

      // Launch movegroupinterface
      moveit::planning_interface::MoveGroupInterface::Options options(ptr->cfg.ARM_GROUP_NAME,"/robot_description",nh_);

      ptr->move_group_ptr = MoveGroupPtr(new moveit::planning_interface::MoveGroupInterface(options));
    }

    ROS_INFO("Setting nh value parameter start");
    ROS_INFO_STREAM("nh_value: "<<nh_value);
    ros::param::set("/ur5_nh_value", nh_value);

    joint_names = ptr->move_group_ptr->getJointNames();
    ros::param::set("/ur5_joint_names", joint_names);

    // advertise publishers to communicate between other state machines
    ur5_pub = nh_.advertise<std_msgs::Bool>("/ur5_clear", 500);
    cv_pub = nh_.advertise<std_msgs::Bool>("/ur5_ws_clear", 500);
    pickStatus_pub = nh_.advertise<std_msgs::String>("/pick_status", 500);

    //
    ptr->marker_publisher = nh_.advertise<visualization_msgs::Marker>(ptr->cfg.MARKER_TOPIC,1);

    //Grasp action client, for object picking
    ptr->grasp_action_client_ptr = GraspActionClientPtr(new GraspActionClient(ptr->cfg.GRASP_ACTION_NAME,true));

    ptr->current_object_ptr = currentObjectPtr(new geometry_msgs::PoseStamped());

    while(ros::ok() && !ptr->grasp_action_client_ptr->waitForServer(ros::Duration(1.0)))
    {
        ROS_WARN_STREAM("Waiting for grasp action servers" << ptr->cfg.GRASP_ACTION_NAME);
    }

}

ClUr5PickAndPlace::~ClUr5PickAndPlace()
{
}

void ClUr5PickAndPlace::postEventMotionExecutionSucceded()
{
    ROS_INFO("[ClUr5PickAndPlace] Post Motion Success Event");
    postEventMotionExecutionSucceded_();
}

void ClUr5PickAndPlace::postEventMotionExecutionFailed()
{
    ROS_INFO("[ClUr5PickAndPlace] Post Motion Failure Event");
    postEventMotionExecutionFailed_();
}

void ClUr5PickAndPlace::postEventMotionExecutionCanceled()
{
    ROS_INFO("[ClUr5PickAndPlace] Post Motion Cancelation Event");
    postEventMotionExecutionCanceled_();
}

void ClUr5PickAndPlace::detectObject()
{
    object_pose = ptr->detect_object();
    ROS_INFO("Object detection finished");
}

void ClUr5PickAndPlace::determineOrientation()
{
  object_orientation = ptr->determine_object_orientation();
}

void ClUr5PickAndPlace::publishState(std::string publisher, std_msgs::Bool& status_msg)
{
  if(publisher == "ur5"){
    ur5_pub.publish(status_msg);
  }
  if(publisher == "cv"){
    cv_pub.publish(status_msg);
  }
}

void ClUr5PickAndPlace::publishPickState(std_msgs::String &status_msg)
{
  pickStatus_pub.publish(status_msg);
}
//void ClUr5PickAndPlace::publishPickState(std::map<std::string,int> &status_msg)
//{
//  pickStatus_pub.publish(status_msg);
//}

} // namespace cl_move_group_interface
} // namespace sm_ur5
