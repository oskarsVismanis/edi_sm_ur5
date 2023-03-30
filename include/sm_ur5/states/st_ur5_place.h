#include <sm_ur5/sm_ur5.h>

namespace sm_ur5
{

// STATE DECLARATION
struct StUr5Place : smacc::SmaccState<StUr5Place, SmUr5>
{
    using SmaccState::SmaccState;

// TRANSITION TABLE
    typedef mpl::list<
    
    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5Initial, SUCCESS>, //StUr5Pick
//    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5StartPose, SUCCESS>,
    Transition<EvMoveGroupMotionExecutionFailed<ClUr5PickAndPlace, OrUr5>, StUr5Place, ABORT> //retry
    
    >reactions;

    
// STATE FUNCTIONS
    static void staticConfigure()
    {
//        configure_orthogonal<OrTimer, CbTimerCountdownLoop>(3);  // EvTimer triggers each 3 client ticks
//        configure_orthogonal<OrTimer, CbTimerCountdownOnce>(5); // EvTimer triggers once at 10 client ticks
    }

    void runtimeConfigure()
    {
      ClUr5PickAndPlace *pickAndPlace;
      this->requiresClient(pickAndPlace);

      pick_and_place::PickAndPlace app = pickAndPlace->application;

//      geometry_msgs::PoseStamped object_pose = app.detect_object();

//      geometry_msgs::Pose class_pose = app.move_group_ptr->getCurrentPose(app.cfg.TCP_LINK_NAME).pose;

//      std::vector<geometry_msgs::Pose> pick_poses = app.create_pick_moves(object_pose);

//      double fraction = app.pickup_box(pick_poses, object_pose, class_pose);

      geometry_msgs::PoseStamped object_pose = pickAndPlace->object_pose;//app.detect_object();

      int object_class = app.object_class;
      geometry_msgs::PoseStamped object_orientation;

      app.place_box(object_pose, object_class, object_orientation);

//      app.set_vacuum_gripper(false,false,false);

//      app.move_predefined(app.cfg.WAIT_POSE_NAME);

//      ros::Duration(2.0).sleep();

//      pickandplaceSystem->onMotionExecutionSuccedded(&StInitialPose::throwSequenceFinishedEvent, this);

      pickAndPlace->ur5_status_msg.data = true;
//      ur5PickandPlaceSystem->publishState(ur5PickandPlaceSystem->ur5_status_msg);
      pickAndPlace->ptr->moveit_visual_tools_->loadRemoteControl();
      pickAndPlace->ptr->moveit_visual_tools_cam->prompt("Press 'next' in the RvizVisualToolsGui");

      pickAndPlace->postEventMotionExecutionSucceded();
//      pickandplaceSystem->postEventMotionExecutionFailed();

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
