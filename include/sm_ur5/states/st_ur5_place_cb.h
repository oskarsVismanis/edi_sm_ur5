#include <sm_ur5/sm_ur5.h>

namespace sm_ur5
{

// STATE DECLARATION
struct StUr5PlaceCb : smacc::SmaccState<StUr5PlaceCb, SmUr5>
{
    using SmaccState::SmaccState;

// TRANSITION TABLE
    typedef mpl::list<
    
//    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5Place, SUCCESS>,
    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5PostPlace, SUCCESS>,
//    Transition<EvMoveGroupMotionExecutionCanceled<ClUr5PickAndPlace, OrUr5>, StUr5Initial, CANCELED>,
    Transition<EvMoveGroupMotionExecutionFailed<ClUr5PickAndPlace, OrUr5>, StUr5PlaceCb, ABORT> //retry
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

      geometry_msgs::Pose place_lin_pose;

      place_lin_pose = pickAndPlace->application.move_group_ptr->getCurrentPose(pickAndPlace->application.cfg.TCP_LINK_NAME).pose;
      //waypoints.push_back(place_lin_pose);

      place_lin_pose.position.z -= pickAndPlace->application.cfg.APPROACH_DISTANCE;
//      pickAndPlace->application.set_vacuum_gripper(false,false,false);
      auto behavior = this->getOrthogonal<OrUr5>()->getClientBehavior<CbMoveCartesian>();
      behavior->goal.pose = place_lin_pose;
      ROS_INFO("Finishing StPlaceCb");
    }

    void onEntry()
    {
        ROS_INFO("On Entry!");

    }

    void onExit()
    {
        ROS_INFO("On Exit!");
        ros::param::set("/ur5_clear", true);
//        ros::param::set("/ur5_post_clear", true);
    }
  
};
} // namespace sm_ur5
