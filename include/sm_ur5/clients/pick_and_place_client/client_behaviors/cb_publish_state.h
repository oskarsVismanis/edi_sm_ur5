#pragma once

#include <smacc/smacc.h>
#include <sm_ur5/clients/pick_and_place_client/cl_ur5_pick_and_place.h>
//#include <geometry_msgs/PoseStamped.h>

namespace sm_ur5
{
namespace cl_ur5_pick_and_place
{
class CbPublishState : public smacc::SmaccClientBehavior
{
public:
//	geometry_msgs::PoseStamped obj;
  ros::NodeHandle nh;
//  int current_state_err;
//	std::string poseName;
  bool status;
  std::string publisher;
//  ur5_
  CbPublishState(std::string publisher_msg, bool status_msg);
    virtual void runtimeConfigure() override;
    virtual void onEntry() override;
    // {
    // 	ClPickAndPlace* pick_and_place;
    // 	this->requiresClient(pick_and_place);
    // 	pick_and_place->ptr->move_predefined(pick_and_place->ptr->cfg.WAIT_POSE_NAME);
    // 	pick_and_place->postEventMotionExecutionSucceded();
    // }

    // virtual void onExit() override;
    // {
    // }
};
} // namespace cl_ur5_pick_and_place
} // namespace sm_ur5
