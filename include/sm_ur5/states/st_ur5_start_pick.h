#include <sm_ur5/sm_ur5.h>

namespace sm_ur5
{

// STATE DECLARATION
struct StUr5StartPick : smacc::SmaccState<StUr5StartPick, SmUr5>
{
    using SmaccState::SmaccState;

// TRANSITION TABLE
    typedef mpl::list<
    
//    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5Pick, SUCCESS>,
    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5PrePick, SUCCESS>,
    Transition<EvMoveGroupMotionExecutionFailed<ClUr5PickAndPlace, OrUr5>, StUr5StartPick, ABORT> //retry
    
    >reactions;

    
// STATE FUNCTIONS
    static void staticConfigure()
    {
      configure_orthogonal<OrUr5, CbMovePredefined>("above_box_wait", true);
    }

    void runtimeConfigure()
    {
//      /*
      ClUr5PickAndPlace *pickAndPlace;
      this->requiresClient(pickAndPlace);
      pickAndPlace->cb_move_cartesian_altered = false;
//      bool clear = false;
      ros::param::set("/ur5_post_clear", false);
      ros::param::set("/ur5_clear", false);
      ros::param::set("/ur5_home", false);
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
