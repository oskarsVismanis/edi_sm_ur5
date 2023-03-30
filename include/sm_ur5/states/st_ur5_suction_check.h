#include <sm_ur5/sm_ur5.h>

namespace sm_ur5
{

// STATE DECLARATION
struct StUr5SuctionCheck : smacc::SmaccState<StUr5SuctionCheck, SmUr5>
{
    using SmaccState::SmaccState;

// TRANSITION TABLE
    typedef mpl::list<
    
//    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5Place, SUCCESS>,
//    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5Initial, SUCCESS>,
//    Transition<EvMoveGroupMotionExecutionCanceled<ClUr5PickAndPlace, OrUr5>, StUr5Initial, CANCELED>,
//    Transition<EvMoveGroupMotionExecutionFailed<ClUr5PickAndPlace, OrUr5>, StUr5Answer, ABORT> //retry
//    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5Answer, SUCCESS>,
    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5Answer, SUCCESS>,
    Transition<EvMoveGroupMotionExecutionFailed<ClUr5PickAndPlace, OrUr5>, StUr5RestartPick, ABORT>
    >reactions;

// STATE FUNCTIONS
    static void staticConfigure()
    {
//      configure_orthogonal<OrUr5, CbMoveCartesian>();
    }

    void runtimeConfigure()
    {
      ClUr5PickAndPlace *pickAndPlace;
      this->requiresClient(pickAndPlace);

//      pickandplace->ptr->moveit_visual_tools_->loadRemoteControl();
//      pickandplace->ptr->moveit_visual_tools_cam->prompt("Press 'next' in the RvizVisualToolsGui");

      ROS_INFO("First Function");

      bool suction = pickAndPlace->application.set_vacuum_gripper(false,false,true);
      ROS_INFO_STREAM("suction: "<< suction);
//            suction = true;
      if (suction == false)
      {
        ROS_ERROR("Suction failed");
        pickAndPlace->application.set_vacuum_gripper(false,false,false);
        pickAndPlace->status.data = "failure_new";
        pickAndPlace->postEventMotionExecutionFailed();
      } else {
        ROS_INFO("Suction succeeded");
        pickAndPlace->status.data = "success";
        pickAndPlace->postEventMotionExecutionSucceded();
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
} // namespace sm_ur5
