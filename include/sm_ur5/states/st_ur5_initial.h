//#include <move_arms/pick_and_place.h>
#include <sm_ur5/sm_ur5.h>

namespace sm_ur5
{

// STATE DECLARATION
struct StUr5Initial : smacc::SmaccState<StUr5Initial, SmUr5>
{
    using SmaccState::SmaccState;

// TRANSITION TABLE
    typedef mpl::list<
    
//    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5Pick, SUCCESS>,
    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5StartPick, SUCCESS>,
    Transition<EvMoveGroupMotionExecutionFailed<ClUr5PickAndPlace, OrUr5>, StUr5Initial, ABORT>
    >reactions;
    
// STATE FUNCTIONS
    static void staticConfigure()
    {
    }

    void runtimeConfigure()
    {
//      /*
      ClUr5PickAndPlace *pickAndPlace;
      this->requiresClient(pickAndPlace);
      double base_angle = 0.0; // to be removed later
      bool success, clear;

      pickAndPlace->application.moveit_visual_tools_->loadRemoteControl();
//      ROS_INFO_STREAM("getRootLink(): "<<pickAndPlace->application.move_group_ptr->getCurrentState()->getRobotModel()->getRootLinkName());

//      std::vector<std::string> jointNames = pickAndPlace->application.move_group_ptr->getJointNames();

      pickAndPlace->rootLink_name = pickAndPlace->application.move_group_ptr->getCurrentState()->getRobotModel()->getRootLinkName();
      ros::param::set("/ur5_root_link", pickAndPlace->rootLink_name);

      base_angle = *pickAndPlace->application.move_group_ptr->getCurrentState()->getJointPositions(pickAndPlace->joint_names[0]);//"ur5/shoulder_pan_joint");
      ROS_INFO_STREAM("Base Angle: " << base_angle);
      if(base_angle > -2.5){
        success = pickAndPlace->application.move_predefined(pickAndPlace->application.cfg.WAIT_POSE_NAME);
//        success = pickAndPlace->application.move_predefined("above_post_wait"); // for post office
      }
      else if(base_angle < -2.5){
//        success = pickAndPlace->application.move_predefined(pickAndPlace->application.cfg.WAIT_POSE_NAME);
        success = pickAndPlace->application.move_predefined("wait_can"); //
//        success = pickAndPlace->application.move_predefined("above_post_wait"); // for post office
      }
      if(success)
      {
//        clear = true;
        ros::param::set("/ur5_clear", true);
        ros::param::set("/ur5_post_clear", true);
        ros::param::set("/ur5_home", true);
      } else {
        ros::param::set("/ur5_clear", false);
        ros::param::set("/ur5_post_clear", false);
        ros::param::set("/ur5_home", false);
//        clear = false;
      }

      pickAndPlace->ur5_status_msg.data = true;
      pickAndPlace->cv_status_msg.data = true;

//      ros::param::set("/ur5_clear", clear);

//      ROS_WARN_STREAM("publishing true for ur5_ws_clear");
//      ur5PickandPlaceSystem->publishState("cv", ur5PickandPlaceSystem->cv_status_msg);
//      ur5PickandPlaceSystem->publishState(ur5PickandPlaceSystem->ur5_status_msg);

//      ros::Duration(10.0).sleep();

//      auto g_names = ur5.move_group_ptr->getJointModelGroupNames();//getJointNames();

//      ROS_WARN_STREAM("getJointModelGroupNames: " << g_names[0] << "\n");
//      auto j_names = ur5.move_group_ptr->getJointNames();

//      ROS_WARN_STREAM("getJointNames: " << j_names[0] << "\n" << j_names[1] << "\n"
//                                        << j_names[2] << "\n" << j_names[3] << "\n"
//                                        << j_names[4] << "\n" << j_names[5]);

//      ur5.moveit_visual_tools_cam->prompt("Press 'next' in the RvizVisualToolsGui window to start cycle");
      pickAndPlace->publishState("cv", pickAndPlace->cv_status_msg);

//      while(ur5PickandPlaceSystem->getState() != true)
//      {
//        ROS_WARN_STREAM("Waiting for ur5e to clear workspace");
//      }
      while(pickAndPlace->getState() != true)
      {
        ROS_WARN_STREAM("Waiting for ur5e to clear workspace");
      }

      pickAndPlace->application.moveit_visual_tools_cam->prompt("Press 'next' in the RvizVisualToolsGui window to start ur5 cycle");

      pickAndPlace->ur5_status_msg.data = false;
      pickAndPlace->publishState("ur5", pickAndPlace->ur5_status_msg);
      pickAndPlace->cv_status_msg.data = false;
      pickAndPlace->publishState("cv", pickAndPlace->cv_status_msg);
      pickAndPlace->postEventMotionExecutionSucceded();
//*/
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
} // namespace sm_ur5
