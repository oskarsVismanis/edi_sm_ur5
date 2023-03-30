#include <sm_ur5/sm_ur5.h>

namespace sm_ur5
{

// STATE DECLARATION
struct StUr5PickCb : smacc::SmaccState<StUr5PickCb, SmUr5>
{
    using SmaccState::SmaccState;

// TRANSITION TABLE
    typedef mpl::list<
    
//    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5Place, SUCCESS>,
    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5PostPick, SUCCESS>,
//    Transition<EvMoveGroupMotionExecutionCanceled<ClUr5PickAndPlace, OrUr5>, StUr5Initial, CANCELED>,
    Transition<EvMoveGroupMotionExecutionFailed<ClUr5PickAndPlace, OrUr5>, StUr5RestartPick, ABORT> //retry
//    Transition<EvMoveGroupMotionExecutionFailed<ClUr5PickAndPlace, OrUr5>, StUr5PickCb, ABORT> //retry
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

      pickAndPlace->application.set_vacuum_gripper(false,true,false);
      pickAndPlace->application.set_vacuum_gripper(true,false,false);

      auto behavior = this->getOrthogonal<OrUr5>()->getClientBehavior<CbMoveCartesian>();
      behavior->goal.pose = pickAndPlace->pick_poses[1];
      ROS_INFO("Finishing StPickCb");
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
