#include <sm_ur5/sm_ur5.h>

namespace sm_ur5
{

// STATE DECLARATION
struct StUr5PrePlace : smacc::SmaccState<StUr5PrePlace, SmUr5>
{
    using SmaccState::SmaccState;

// TRANSITION TABLE
    typedef mpl::list<
    
//    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5Pick, SUCCESS>,
  Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5PlaceCb, SUCCESS>,
  Transition<EvMoveGroupMotionExecutionFailed<ClUr5PickAndPlace, OrUr5>, StUr5PrePlace, ABORT>
    
    >reactions;

    
// STATE FUNCTIONS
    static void staticConfigure()
    {
//      ClUr5PickAndPlace *pickAndPlace;
      std::string nh_value;
      ros::param::get("/ur5_nh_value", nh_value);
      ros::NodeHandle nh(nh_value);
      std::map<std::string,std::string> current_object_param;
      int obj_class;

      // in case something is broken here, comment out \/ and uncomment
      nh.getParam("current_object/class",obj_class);


      // \/ this one
//      ros::param::get("/ur5/current_object/class", obj_class);
//      ros::param::get(nh_value+"current_object/class", obj_class);


      ROS_ERROR("Object class according to ur5 pre-place:: %i",obj_class);
      ROS_ERROR_STREAM("Object class according to ur5 pre-place: " << std::to_string(obj_class));
      if(obj_class == 1){
        configure_orthogonal<OrUr5, CbMovePredefined>("place_bottle", true);
      }
      else{
        configure_orthogonal<OrUr5, CbMovePredefined>("place_can", true);
      }
    }

    void runtimeConfigure()
    {
      ClUr5PickAndPlace *pickAndPlace;
      this->requiresClient(pickAndPlace);
      ROS_ERROR("Object class according to ur5 pre-place pointer: %i",pickAndPlace->application.object_class);
      auto behavior = this->getOrthogonal<OrUr5>()->getClientBehavior<CbMovePredefined>();
      if(pickAndPlace->application.object_class == 1){
         behavior->poseName = "place_bottle";
      }
      else if(pickAndPlace->application.object_class == 2){
        behavior->poseName = "place_can";
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
