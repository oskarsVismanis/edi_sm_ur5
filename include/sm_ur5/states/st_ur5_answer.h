#include <sm_ur5/sm_ur5.h>

namespace sm_ur5
{

// STATE DECLARATION
struct StUr5Answer : smacc::SmaccState<StUr5Answer, SmUr5>
{
    using SmaccState::SmaccState;

// TRANSITION TABLE
    typedef mpl::list<
    
    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5PrePlace, SUCCESS>,
//    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5PostPick, SUCCESS>,
    Transition<EvMoveGroupMotionExecutionCanceled<ClUr5PickAndPlace, OrUr5>, StUr5Initial, CANCELED>,
    Transition<EvMoveGroupMotionExecutionFailed<ClUr5PickAndPlace, OrUr5>, StUr5PrePick, ABORT> //retry
//    Transition<EvMoveGroupMotionExecutionFailed<ClUr5PickAndPlace, OrUr5>, StUr5PickCb, ABORT> //retry
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

      std_msgs::String command;

//      pickandplace->ptr->moveit_visual_tools_->loadRemoteControl();
//      pickandplace->ptr->moveit_visual_tools_cam->prompt("Press 'next' in the RvizVisualToolsGui");

      ROS_ERROR_STREAM("Answering StPickSupport with: "<<pickAndPlace->status.data);

      if(pickAndPlace->status.data != "success")
      {
        ros::Duration(0.6).sleep(); // a pause for the moment, so robot would be out of workspace for new image capture ?needs to be a separate state in the future?
        pickAndPlace->publishPickState(pickAndPlace->status);
        command = *ros::topic::waitForMessage<std_msgs::String>("/robot_command");
        ROS_ERROR("Waiting for response from SmCv");
        if(command.data == "initial")
        {
          ROS_ERROR("Pick canceled");
          pickAndPlace->postEventMotionExecutionCanceled(); // go to initial state
        } else {
          ROS_ERROR("Pick restarted");
          pickAndPlace->postEventMotionExecutionFailed(); // try again
        }
      } else {
        pickAndPlace->publishPickState(pickAndPlace->status);
        ROS_ERROR("Pick succeeded!");
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
