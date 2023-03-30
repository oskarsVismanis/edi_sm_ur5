#include <sm_ur5/clients/pick_and_place_client/client_behaviors/cb_publish_state.h>
#include <sm_ur5/sm_ur5.h>

namespace sm_ur5
{
namespace cl_ur5_pick_and_place
{
// class CbMovePredefined : public smacc::SmaccClientBehavior
// {
// public:
  CbPublishState::CbPublishState(std::string publisher_msg, bool status_msg) : status(status_msg), publisher(publisher_msg)//std::string pose_name) : poseName(pose_name)
	{
    // ptr->target_recognition_client = ptr->nh.serviceClient<vizta::GetTargetPose>(ptr->cfg.TARGET_RECOGNITION_SERVICE);
	}
//  CbPublishState::CbPublishState()
//	{
//	}

    void CbPublishState::runtimeConfigure()
    {
    }
    void CbPublishState::onEntry()
    {
//      ClUr5PickAndPlace *pick_and_place;

      ClUr5PickAndPlace *ur5PickandPlace;
//    	this->requiresClient(pick_and_place);
      this->requiresClient(ur5PickandPlace);

      ur5PickandPlace->ptr->moveit_visual_tools_cam->loadRemoteControl();
      ur5PickandPlace->ptr->moveit_visual_tools_cam->prompt("Press 'next' in the RvizVisualToolsGui to publish ");

      if(publisher == "ur5"){

        ROS_INFO("[CbPublishState]Publishing from ur5_pub to \"/ur5_clear\"");
        ur5PickandPlace->ur5_status_msg.data = status;
        ur5PickandPlace->ur5_pub.publish(ur5PickandPlace->ur5_status_msg);
      }
      if(publisher == "cv"){
        ROS_INFO("[CbPublishState]Publishing from cv_pub to \"/ur5_ws_clear\"");
        ur5PickandPlace->cv_status_msg.data = status;
        ur5PickandPlace->cv_pub.publish(ur5PickandPlace->cv_status_msg);
      }


//    	pick_and_place->ptr->done = pick_and_place->ptr->move_predefined(poseName);
//      done = ur5PickandPlace->ptr->move_predefined(poseName);
//    	// pick_and_place->object_pose = pick_and_place->detectObject();
//    	// obj = pick_and_place->application.detect_object();
//      if(done)
//            {
////                ros::param::set("/sm_test/current_state_err", 0);
//                ROS_INFO("Cb succeeded");
//                ur5PickandPlace->postEventMotionExecutionSucceded();
//            }
//      else
//            {
////                ros::param::get("/sm_test/current_state_err", ur5PickandPlace->current_state_err);
////                if(ur5PickandPlace->current_state_err > 1)
////                {
////                    ur5PickandPlace->postEventMotionExecutionCycleFailed();
////                }
////                else
////                {
////                    ros::param::set("/sm_test/current_state_err", ur5PickandPlace->current_state_err + 1);
//                    ur5PickandPlace->postEventMotionExecutionFailed();
////                }
//            }
    }
    // CbMovePredefined::onExit()
    // {
    // }
//};
} // namespace cl_pick_and_place
} // namespace sm_test
