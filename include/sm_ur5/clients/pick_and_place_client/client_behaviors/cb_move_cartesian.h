#pragma once

#include <smacc/smacc.h>
#include <sm_ur5/clients/pick_and_place_client/cl_ur5_pick_and_place.h>
#include <geometry_msgs/PoseStamped.h>

namespace sm_ur5
{
namespace cl_ur5_pick_and_place
{
class CbMoveCartesian : public smacc::SmaccClientBehavior
{
public:
  geometry_msgs::PoseStamped goal;
  geometry_msgs::PoseStamped object_pose;
  std::vector<geometry_msgs::Pose> pick_poses, place_poses;
  geometry_msgs::Pose class_pose;
  double fraction;
	// ros::NodeHandle nh__;
//  int current_state_err;
	std::string poseName;
  bool done, altered;
  CbMoveCartesian();
//	CbMovePredefined();
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
