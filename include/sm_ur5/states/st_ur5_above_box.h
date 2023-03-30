#include <sm_ur5/sm_ur5.h>

namespace sm_ur5
{

// STATE DECLARATION
struct StUr5AboveBox : smacc::SmaccState<StUr5AboveBox, SmUr5>
{
    using SmaccState::SmaccState;

// TRANSITION TABLE
    typedef mpl::list<
    
//    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5Pick, SUCCESS>,
  Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5Answer, SUCCESS>,
  Transition<EvMoveGroupMotionExecutionFailed<ClUr5PickAndPlace, OrUr5>, StUr5RestartPick, ABORT>
    
    >reactions;

    
// STATE FUNCTIONS
    static void staticConfigure()
    {
      configure_orthogonal<OrUr5, CbMovePredefined>("above_box", true);
    }

    void runtimeConfigure()
    {
//      ClUr5PickAndPlace *pickandplace;
//      this->requiresClient(pickandplace);
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
