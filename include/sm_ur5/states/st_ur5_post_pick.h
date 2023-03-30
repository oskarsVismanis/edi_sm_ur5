#include <sm_ur5/sm_ur5.h>

namespace sm_ur5
{

// STATE DECLARATION
struct StUr5PostPick : smacc::SmaccState<StUr5PostPick, SmUr5>
{
    using SmaccState::SmaccState;

// TRANSITION TABLE
    typedef mpl::list<
    
    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5SuctionCheck, SUCCESS>,
//    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5Initial, SUCCESS>,
//    Transition<EvMoveGroupMotionExecutionCanceled<ClUr5PickAndPlace, OrUr5>, StUr5Initial, CANCELED>,
//    Transition<EvMoveGroupMotionExecutionFailed<ClUr5PickAndPlace, OrUr5>, StUr5Answer, ABORT> //retry
//    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5AboveBox, SUCCESS>,
    Transition<EvMoveGroupMotionExecutionFailed<ClUr5PickAndPlace, OrUr5>, StUr5RestartPick, ABORT>
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
      std_msgs::String command;

      pickAndPlace->cb_move_cartesian_altered = true;

//      pickandplace->application.set_vacuum_gripper(false,false,false); // just for the moment, so the gripper wouldn't be constantly running

      auto behavior = this->getOrthogonal<OrUr5>()->getClientBehavior<CbMoveCartesian>();
      behavior->goal.pose = pickAndPlace->pick_poses[2];
      ROS_INFO("Finishing StPostPick");
    }

    void onEntry()
    {
        ROS_INFO("On Entry!");
    }

    void onExit()
    {
//      ClUr5PickAndPlace *pickAndPlace;
//      this->requiresClient(pickAndPlace);
//      bool suction = pickAndPlace->application.set_vacuum_gripper(false,false,true);
//      ROS_INFO_STREAM("suction: "<< suction);
      ROS_INFO("On Exit!");
//      if (suction == false)
//      {
//        ROS_ERROR("Suction failed");
//        pickAndPlace->application.set_vacuum_gripper(false,false,false);
//        pickAndPlace->status.data = "failure_new";
//      }
//      else {
//        ROS_INFO("Suction succeeded");
//        pickAndPlace->status.data = "success";
//      }
    }
  
};
} // namespace sm_ur5
