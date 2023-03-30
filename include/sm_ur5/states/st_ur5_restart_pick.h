#include <sm_ur5/sm_ur5.h>

namespace sm_ur5
{

// STATE DECLARATION
struct StUr5RestartPick : smacc::SmaccState<StUr5RestartPick, SmUr5>
{
    using SmaccState::SmaccState;

// TRANSITION TABLE
    typedef mpl::list<
    
//    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5Pick, SUCCESS>,
    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5Answer, SUCCESS>,
    Transition<EvMoveGroupMotionExecutionFailed<ClUr5PickAndPlace, OrUr5>, StUr5RestartPick, ABORT> //retry
    
    >reactions;

    
// STATE FUNCTIONS
    static void staticConfigure()
    {
//      configure_orthogonal<OrUr5, CbMovePredefined>("above_box_wait", true);
      configure_orthogonal<OrUr5, CbMovePredefined>("above_post_wait", true); // for post office
    }

    void runtimeConfigure()
    {
//      /*
      ClUr5PickAndPlace *pickAndPlace;
      this->requiresClient(pickAndPlace);
//      pickAndPlace->cb_move_cartesian_altered = false;
      pickAndPlace->ptr->set_vacuum_gripper(false,false,false);
//      pickandplace->ptr->moveit_visual_tools_->loadRemoteControl();
//      pickandplace->ptr->moveit_visual_tools_cam->prompt("Press 'next' in the RvizVisualToolsGui");

    }

    void onEntry()
    {
        ROS_INFO("On Entry!");
    }

    void onExit()
    {
        ROS_INFO("On Exit!");
        ros::Duration(1).sleep(); // for post office
        ros::param::set("/ur5_post_clear", true); //only used for post office demo, doesn't affect anything else
    }
  
};
} // namespace sm_ur5
