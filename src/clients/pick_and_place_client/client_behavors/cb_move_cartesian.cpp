#include <sm_ur5/clients/pick_and_place_client/client_behaviors/cb_move_cartesian.h>
//#include <sm_ur5/clients/pick_and_place_client/cl_ur5_pick_and_place.h>
#include <sm_ur5/sm_ur5.h>

namespace sm_ur5
{
namespace cl_ur5_pick_and_place
{
// class CbMovePredefined : public smacc::SmaccClientBehavior
// {
// public:
  CbMoveCartesian::CbMoveCartesian()//std::string pose_name, bool last) : poseName(pose_name), last_command(last)
	{
	}
//	CbMovePredefined::CbMovePredefined()
//	{
//	}
    void CbMoveCartesian::onEntry()
    {
      ClUr5PickAndPlace *pickandplace;
      this->requiresClient(pickandplace);
//    	pick_and_place->ptr->done = pick_and_place->ptr->move_predefined(poseName);
      fraction = pickandplace->ptr->pickup_box(goal.pose, goal);
//    	// pick_and_place->object_pose = pick_and_place->detectObject();
//    	// obj = pick_and_place->application.detect_object();
      if(fraction == 1)
            {
                ROS_INFO("CbMoveCartesian succeeded");
                pickandplace->status.data = "success";
                pickandplace->postEventMotionExecutionSucceded();
            }
      else
            {
                if(pickandplace->cb_move_cartesian_altered == true)
                {
                  ROS_WARN("Workspace altered");
                  pickandplace->status.data = "failure_new";
                } else {
                  ROS_WARN("Workspace is the same");
                  pickandplace->status.data = "failure";
                }

                pickandplace->postEventMotionExecutionFailed();
            }
    }
    // CbMovePredefined::onExit()
    // {
    // }
//};
} // namespace cl_pick_and_place
} // namespace sm_test
