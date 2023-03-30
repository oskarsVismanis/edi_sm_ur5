#include <sm_ur5/clients/pick_and_place_client/client_behaviors/cb_move_predefined.h>
//#include <sm_ur5/clients/pick_and_place_client/cl_ur5_pick_and_place.h>
#include <sm_ur5/sm_ur5.h>

namespace sm_ur5
{
namespace cl_ur5_pick_and_place
{
// class CbMovePredefined : public smacc::SmaccClientBehavior
// {
// public:
  CbMovePredefined::CbMovePredefined(std::string pose_name, bool last) : poseName(pose_name), last_command(last)
	{
    // ptr->target_recognition_client = ptr->nh.serviceClient<vizta::GetTargetPose>(ptr->cfg.TARGET_RECOGNITION_SERVICE);
	}
//	CbMovePredefined::CbMovePredefined()
//	{
//	}
    void CbMovePredefined::onEntry()
    {
//      ClUr5PickAndPlace *pick_and_place;
      ClUr5PickAndPlace *ur5PickandPlace;
//    	this->requiresClient(pick_and_place);
      this->requiresClient(ur5PickandPlace);
//    	pick_and_place->ptr->done = pick_and_place->ptr->move_predefined(poseName);
      done = ur5PickandPlace->ptr->move_predefined(poseName);
//    	// pick_and_place->object_pose = pick_and_place->detectObject();
//    	// obj = pick_and_place->application.detect_object();
      if(done)
            {
//                ros::param::set("/sm_test/current_state_err", 0);
                ROS_INFO("Cb succeeded");
                if(last_command == true)
                {
                  ur5PickandPlace->postEventMotionExecutionSucceded();
                } else {
                  ROS_INFO("CbMovePredefined not last function so it doesn't post any Event");
                }
            }
      else
            {
              ROS_INFO("[CbMovePredefined] move_predefined either failed or robot already is at this position");
              ur5PickandPlace->postEventMotionExecutionFailed();
            }
    }
    // CbMovePredefined::onExit()
    // {
    // }
//};
} // namespace cl_pick_and_place
} // namespace sm_test
