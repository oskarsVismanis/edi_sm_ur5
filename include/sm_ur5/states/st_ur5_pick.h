#include <sm_ur5/sm_ur5.h>

namespace sm_ur5
{

// STATE DECLARATION
struct StUr5Pick : smacc::SmaccState<StUr5Pick, SmUr5>
{
    using SmaccState::SmaccState;

// TRANSITION TABLE
    typedef mpl::list<
    
    Transition<EvMoveGroupMotionExecutionSucceded<ClUr5PickAndPlace, OrUr5>, StUr5Place, SUCCESS>,
    Transition<EvMoveGroupMotionExecutionCanceled<ClUr5PickAndPlace, OrUr5>, StUr5Initial, CANCELED>,
    Transition<EvMoveGroupMotionExecutionFailed<ClUr5PickAndPlace, OrUr5>, StUr5Pick, ABORT> //retry
    
    >reactions;

// STATE FUNCTIONS
    static void staticConfigure()
    {
//        configure_orthogonal<OrTimer, CbTimerCountdownLoop>(3);  // EvTimer triggers each 3 client ticks
//        configure_orthogonal<OrTimer, CbTimerCountdownOnce>(5); // EvTimer triggers once at 10 client ticks
    }

    void runtimeConfigure()
    {
      ClUr5PickAndPlace *pickandplaceSystem;
      this->requiresClient(pickandplaceSystem);

      pick_and_place::PickAndPlace app = pickandplaceSystem->application;

      int choice = 0; // 0 = param (from sm_cv) (needed for post_office) // 1 = straight from detect_object
      geometry_msgs::PoseStamped object_pose, object_orientation, object_pose_param, object;

      if(choice == 0)
      {
          std::map<std::string,std::string> current_object_param;
          ros::NodeHandle nh("/ur5");

          object_pose_param.header.frame_id = "ur5/base_link";

          if (nh.getParam("current_object",current_object_param))
          {

            object_pose_param.pose.position.x = std::stod(current_object_param["x"]);
            object_pose_param.pose.position.y = std::stod(current_object_param["y"]);
            object_pose_param.pose.position.z = std::stod(current_object_param["z"]);

            object_pose_param.pose.orientation.x = std::stod(current_object_param["qx"]);
            object_pose_param.pose.orientation.y = std::stod(current_object_param["qy"]);
            object_pose_param.pose.orientation.z = std::stod(current_object_param["qz"]);
            object_pose_param.pose.orientation.w = std::stod(current_object_param["qw"]);

            ROS_INFO_STREAM("[getParam] current object pose: "<<object_pose_param);
            object = object_pose_param;
    //        ros::shutdown();;

          } else {
            ROS_WARN("No parameter found");
            ros::shutdown();
          }
        }
//      }

      if(choice == 1)
      {
        pickandplaceSystem->detectObject();
        pickandplaceSystem->determineOrientation();

        object_pose = pickandplaceSystem->object_pose;
        ROS_ERROR_STREAM("Class in pick state: " << pickandplaceSystem->application.object_class);
        object_orientation = pickandplaceSystem->object_orientation;

        ROS_INFO_STREAM(object_orientation);

        std_msgs::String base_link;
        base_link.data = "ur5/base_link";

        geometry_msgs::PoseStamped object = app.transform_pose(object_orientation, base_link.data);

//        object = object_orientation; // object_pose
      }

      if(std::isnan(object.pose.position.x) || object.pose.position.x == 0)
      {
        ROS_ERROR_STREAM(object<< "is nan \n RESTARTING!");
        pickandplaceSystem->postEventMotionExecutionFailed();
      } else {
        std::vector<geometry_msgs::Pose> pick_poses = pickandplaceSystem->application.create_pick_moves(object);
        std_msgs::String status, command;
//        std::map<std::string,int> status_msg; // [0] pick success, [1] workspace altered

        ROS_WARN("Moving above box");
//        bool success = pickandplaceSystem->application.move_predefined(pickandplaceSystem->application.cfg.ABOVE_BOX_WAIT_POSE);

        app.moveit_visual_tools_cam->prompt("Press 'next' in the RvizVisualToolsGui window");
        double fraction = pickandplaceSystem->application.pickup_box(pick_poses[0], object);
        ROS_WARN("Prepick");
        if (fraction != 1)
        {
          ROS_ERROR("Prepick failed");
          // send signal to sm_cv about pick status
          ros::Duration(1.0).sleep();
          status.data = "failure";
//          status_msg["success"] = 0;
//          status_msg["new"] = 0;
          pickandplaceSystem->publishPickState(status);
//          pickandplaceSystem->publishPickState(status_msg);
          // receive command from sm_cv about next action
          command = *ros::topic::waitForMessage<std_msgs::String>("/robot_command");
          if(command.data == "initial")
          {
            pickandplaceSystem->postEventMotionExecutionCanceled(); // go to initial state
          } else {
            pickandplaceSystem->postEventMotionExecutionFailed(); // try again
          }
          // end of communication with sm_cv
        } else {
          app.moveit_visual_tools_cam->prompt("Press 'next' in the RvizVisualToolsGui window");
          pickandplaceSystem->application.set_vacuum_gripper(false,true,false);
          pickandplaceSystem->application.set_vacuum_gripper(true,false,false);
          fraction = pickandplaceSystem->application.pickup_box(pick_poses[1], object);//, class_pose);
          ROS_WARN("Pick");
          if (fraction != 1)
          {
            pickandplaceSystem->application.set_vacuum_gripper(false,false,false);
            bool success = pickandplaceSystem->application.move_predefined(pickandplaceSystem->application.cfg.ABOVE_BOX_WAIT_POSE);
            ROS_ERROR("Pick failed");
            // send signal to sm_cv about pick status
            status.data = "failure_new";
//          status_msg["success"] = 0;
//          status_msg["new"] = 1;
            pickandplaceSystem->publishPickState(status);
//          pickandplaceSystem->publishPickState(status_msg);
            // receive command from sm_cv about next action
            command = *ros::topic::waitForMessage<std_msgs::String>("/robot_command");
            if(command.data == "initial")
            {
              pickandplaceSystem->postEventMotionExecutionCanceled(); // go to initial state
            } else {
              pickandplaceSystem->postEventMotionExecutionFailed(); // try again
            }
            // end of communication with sm_cv
          } else {
  //          app.set_vacuum_gripper(false,true,false);
  //          app.set_vacuum_gripper(true,false,false);
            app.moveit_visual_tools_cam->prompt("Press 'next' in the RvizVisualToolsGui window");
            fraction = pickandplaceSystem->application.pickup_box(pick_poses[2], object);//, class_pose);
            ROS_WARN("Postpick");
            if (fraction != 1)
            {
              pickandplaceSystem->application.set_vacuum_gripper(false,false,false);
              bool success = pickandplaceSystem->application.move_predefined(pickandplaceSystem->application.cfg.ABOVE_BOX_WAIT_POSE);
              ROS_ERROR("Postpick failed");
              // send signal to sm_cv about pick status
              status.data = "failure_new";
//            status_msg["success"] = 0;
//            status_msg["new"] = 1;
              pickandplaceSystem->publishPickState(status);
//            pickandplaceSystem->publishPickState(status_msg);
              // receive command from sm_cv about next action
              command = *ros::topic::waitForMessage<std_msgs::String>("/robot_command");
              if(command.data == "initial")
              {
                pickandplaceSystem->postEventMotionExecutionCanceled(); // go to initial state
              } else {
                pickandplaceSystem->postEventMotionExecutionFailed(); // try again
              }
              // end of communication with sm_cv
            } else {
              bool suction = pickandplaceSystem->application.set_vacuum_gripper(false,false,true);
              ROS_INFO_STREAM("suction: "<< suction);
  //            suction = true;
              if (suction == false)
              {
                pickandplaceSystem->application.set_vacuum_gripper(false,false,false);
                bool success = pickandplaceSystem->application.move_predefined(pickandplaceSystem->application.cfg.ABOVE_BOX_WAIT_POSE);
                ros::Duration(2.0).sleep();
                ROS_ERROR("Suction failed");
                // send signal to sm_cv about pick status
                status.data = "failure_new";
//              status_msg["success"] = 0;
//              status_msg["new"] = 1;
                pickandplaceSystem->publishPickState(status);
//              pickandplaceSystem->publishPickState(status_msg);
                // receive command from sm_cv about next action
                command = *ros::topic::waitForMessage<std_msgs::String>("/robot_command");
                if(command.data == "initial")
                {
                  pickandplaceSystem->postEventMotionExecutionCanceled(); // go to initial state
                } else {
                  pickandplaceSystem->postEventMotionExecutionFailed(); // try again
                }
                // end of communication with sm_cv
              } else {
                app.moveit_visual_tools_cam->prompt("Press 'next' in the RvizVisualToolsGui window");
                ROS_ERROR("Pick-up successful");
                // send signal to sm_cv about pick status
                status.data = "success";
//              status_msg["success"] = 1;
//              status_msg["new"] = 0;
                pickandplaceSystem->publishPickState(status);
//              pickandplaceSystem->publishPickState(status_msg);
                pickandplaceSystem->postEventMotionExecutionSucceded();
              }
            }
          }
        }
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
//}
} // namespace sm_ur5
