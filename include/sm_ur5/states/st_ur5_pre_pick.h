#include <sm_ur5/sm_ur5.h>

namespace sm_ur5
{

// STATE DECLARATION
struct StUr5PrePick : smacc::SmaccState<StUr5PrePick, SmUr5>
{
    using SmaccState::SmaccState;

// TRANSITION TABLE
    typedef mpl::list<
    
//    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5Place, SUCCESS>,
    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5PickCb, SUCCESS>,
//    Transition<EvMoveGroupMotionExecutionCanceled<ClUr5PickAndPlace, OrUr5>, StUr5Initial, CANCELED>,
//    Transition<EvMoveGroupMotionExecutionFailed<ClUr5PickAndPlace, OrUr5>, StUr5PrePick, ABORT> //retry
    Transition<EvMoveGroupMotionExecutionFailed<ClUr5PickAndPlace, OrUr5>, StUr5RestartPick, ABORT> //retry
    >reactions;

// STATE FUNCTIONS
    static void staticConfigure()
    {
      configure_orthogonal<OrUr5, CbMoveCartesian>();
    }

    void runtimeConfigure()
    {
      ClUr5PickAndPlace *pickAndPlace;
      this->requiresClient(pickAndPlace);

      pickAndPlace->cb_move_cartesian_altered = false;

      // start of ?CbGetPose?
      std::map<std::string,std::string> current_object_param;
      geometry_msgs::PoseStamped object_pose_param, object;
      ros::NodeHandle nh(pickAndPlace->nh_value);

//      std::vector<std::string> linkNames = pickAndPlace->application.move_group_ptr->getLinkNames();

      object_pose_param.header.frame_id = pickAndPlace->rootLink_name;//pickAndPlace->application.move_group_ptr->getCurrentState()->getRobotModel()->getRootLinkName();//"ur5/base_link";

      if (nh.getParam("current_object",current_object_param))
      {

        object_pose_param.pose.position.x = std::stod(current_object_param["x"]);
        object_pose_param.pose.position.y = std::stod(current_object_param["y"]);
        object_pose_param.pose.position.z = std::stod(current_object_param["z"]);

        object_pose_param.pose.orientation.x = std::stod(current_object_param["qx"]);
        object_pose_param.pose.orientation.y = std::stod(current_object_param["qy"]);
        object_pose_param.pose.orientation.z = std::stod(current_object_param["qz"]);
        object_pose_param.pose.orientation.w = std::stod(current_object_param["qw"]);

//        ROS_INFO_STREAM("getting object class");
        ROS_INFO_STREAM(current_object_param["class"]);
//        if(current_object_param["class"] != '1'){
//          current_object_param["class"] = 1;
//        }
        pickAndPlace->application.object_class = std::stoi(current_object_param["class"]);

        ROS_INFO_STREAM("[getParam] current object pose: "<<object_pose_param);
        ROS_ERROR("Object class according to Pre-pick: %i", pickAndPlace->application.object_class);
        object = object_pose_param;
      }
      else
      {
        ROS_WARN("No parameter found");
        ros::shutdown();
      }

      if(std::isnan(object.pose.position.x) || object.pose.position.x == 0)
      {
        ROS_ERROR_STREAM(object<< "is nan \n RESTARTING!");
        pickAndPlace->postEventMotionExecutionFailed();
      }
      else
      {
        pickAndPlace->pick_poses = pickAndPlace->application.create_pick_moves(object);
        auto behavior = this->getOrthogonal<OrUr5>()->getClientBehavior<CbMoveCartesian>();
        behavior->goal.pose = pickAndPlace->pick_poses[0];
        ROS_INFO("Finishing StPrePick");
      }
    }

    void onEntry()
    {
        ROS_INFO("On Entry!");
    }

    void onExit()
    {
        ROS_INFO("On Exit!");
    }
  
};
//}
} // namespace sm_ur5
