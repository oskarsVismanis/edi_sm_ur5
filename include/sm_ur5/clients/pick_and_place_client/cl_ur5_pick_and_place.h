#pragma once

#include <smacc/smacc_client.h>
#include <smacc/smacc_signal.h>
#include <smacc/smacc.h>
#include <moveit/move_group_interface/move_group_interface.h>
#include <moveit/planning_scene_interface/planning_scene_interface.h>
#include <geometry_msgs/Vector3.h>
#include <geometry_msgs/Transform.h>
//#include <sm_ur5/sm_ur5.h>

#include <move_arms/pick_and_place.h>

namespace sm_ur5
{
namespace cl_ur5_pick_and_place
{

template <typename TSource, typename TOrthogonal>
struct EvMoveGroupMotionExecutionSucceded : sc::event<EvMoveGroupMotionExecutionSucceded<TSource, TOrthogonal>>
{
};

template <typename TSource, typename TOrthogonal>
struct EvMoveGroupMotionExecutionFailed : sc::event<EvMoveGroupMotionExecutionFailed<TSource, TOrthogonal>>
{
};

template <typename TSource, typename TOrthogonal>
struct EvMoveGroupMotionExecutionCanceled : sc::event<EvMoveGroupMotionExecutionCanceled<TSource, TOrthogonal>>
{
};

class ClUr5PickAndPlace : public smacc::ISmaccClient//, MoveGroupInterface //, public smacc::ISmaccUpdatable
{
private:
  std::function<void()> postEventMotionExecutionSucceded_;
  std::function<void()> postEventMotionExecutionFailed_;
  std::function<void()> postEventMotionExecutionCanceled_;

  smacc::SmaccSignal<void()> onSucceded_;
  smacc::SmaccSignal<void()> onFailed_;
  smacc::SmaccSignal<void()> onCanceled_;

public:

  //    Adding pick and place header //

  std::string nh_value, rootLink_name;

  std::vector<std::string> joint_names, link_names;

  pick_and_place::PickAndPlace application;

  pick_and_place::PickAndPlace* ptr = &application;

  geometry_msgs::PoseStamped object_pose, object_orientation;

  std::vector<geometry_msgs::Pose> pick_poses;

  bool cb_move_cartesian_altered;

  std_msgs::Bool ur5_status,
                 ur5e_status,
                 ur5_status_msg,
                 cv_status_msg;

  std_msgs::String pickStatus_msg, status;

  ros::Publisher ur5_pub,
                 cv_pub,
                 pickStatus_pub;

  ClUr5PickAndPlace(std::string robot);

  virtual ~ClUr5PickAndPlace();

  void detectObject();

  void determineOrientation();

  void publishState(std::string publisher, std_msgs::Bool& status_msg);

  void publishPickState(std_msgs::String& status_msg);

  bool getState()
  {
    ptr->ur5e_status_ptr = ros::topic::waitForMessage<std_msgs::Bool>("/ur5e_clear");
    ur5e_status = *ptr->ur5e_status_ptr;
    return ur5e_status.data;
  }

  void postEventMotionExecutionSucceded();
  void postEventMotionExecutionFailed();
  void postEventMotionExecutionCanceled();

  template <typename TOrthogonal, typename TSourceObject>
  void onOrthogonalAllocation()
  {
    postEventMotionExecutionSucceded_ = [=]() {
      this->onSucceded_();
      this->postEvent<EvMoveGroupMotionExecutionSucceded<TSourceObject, TOrthogonal>>();
    };

    postEventMotionExecutionFailed_ = [=]() {
      this->onFailed_();
      this->postEvent<EvMoveGroupMotionExecutionFailed<TSourceObject, TOrthogonal>>();
    };

    postEventMotionExecutionCanceled_ = [=]() {
      this->onCanceled_();
      this->postEvent<EvMoveGroupMotionExecutionCanceled<TSourceObject, TOrthogonal>>();
    };
  }

    template <typename TCallback, typename T>
    boost::signals2::connection onMotionExecutionSuccedded(TCallback callback, T *object)
    {
      return this->getStateMachine()->createSignalConnection(onSucceded_, callback, object);
    }

    template <typename TCallback, typename T>
    boost::signals2::connection onMotionExecutionFailed(TCallback callback, T *object)
    {
      return this->getStateMachine()->createSignalConnection(onFailed_, callback, object);
    }

    template <typename TCallback, typename T>
    boost::signals2::connection onMotionExecutionCanceled(TCallback callback, T *object)
    {
      return this->getStateMachine()->createSignalConnection(onCanceled_, callback, object);
    }

protected:
    //{"/ur5"};
//    ros::NodeHandle nh;
};
} // namespace cl_perception_system
} // namespace sm_pick_n_place
