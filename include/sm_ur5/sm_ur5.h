#ifndef SM_UR5_H_
#define SM_UR5_H_
#include <smacc/smacc.h>
#include <move_arms/pick_and_place.h>

// CLIENTS
#include <sm_ur5/clients/pick_and_place_client/cl_ur5_pick_and_place.h>

// ORTHOGONALS
#include <sm_ur5/orthogonals/or_ur5.h>

//CLIENT BEHAVIORS
#include <sm_ur5/clients/pick_and_place_client/client_behaviors/cb_move_predefined.h>
#include <sm_ur5/clients/pick_and_place_client/client_behaviors/cb_move_cartesian.h>
#include <sm_ur5/clients/pick_and_place_client/client_behaviors/cb_publish_state.h>

using namespace boost;
using namespace smacc;

namespace sm_ur5
{

//ur5 cycle states
class StUr5Initial;
class StUr5StartPick;
class StUr5PrePick;
class StUr5PickCb;
class StUr5PostPick;
class StUr5SuctionCheck;
class StUr5Answer;
class StUr5PrePlace;
class StUr5PlaceCb;
class StUr5PostPlace;
//ur5 helper states
class StUr5RestartPick;
class StUr5AboveBox; // unused
//ur5 extra states (here for reference)
class StUr5Pick; // divided into PrePick->PickCb->PostPick->SuctionCheck->Answer
class StUr5Place; // divided into PrePlace->PlaceCb->PostPlace

//--------------------------------------------------------------------
//STATE_MACHINE
struct SmUr5
    : public smacc::SmaccStateMachineBase<SmUr5, StUr5Initial>
{
    using SmaccStateMachineBase::SmaccStateMachineBase;

    virtual void onInitialize() override
    {
        ROS_INFO("CREATING ORTHOGONALS");
        this->createOrthogonal<OrUr5>();
    }
};
} // namespace sm_ur5

//ur5 cycle states
#include <sm_ur5/states/st_ur5_initial.h>
#include <sm_ur5/states/st_ur5_start_pick.h>
#include <sm_ur5/states/st_ur5_pre_pick.h>
#include <sm_ur5/states/st_ur5_pick_cb.h>
#include <sm_ur5/states/st_ur5_post_pick.h>
#include <sm_ur5/states/st_ur5_suction_check.h>
#include <sm_ur5/states/st_ur5_answer.h>
#include <sm_ur5/states/st_ur5_pre_place.h>
#include <sm_ur5/states/st_ur5_place_cb.h>
#include <sm_ur5/states/st_ur5_post_place.h>
//ur5 helper states
#include <sm_ur5/states/st_ur5_restart_pick.h>
#include <sm_ur5/states/st_ur5_above_box.h> // unused
//ur5 extra states (unused)
#include <sm_ur5/states/st_ur5_pick.h>
#include <sm_ur5/states/st_ur5_place.h>

#endif /* SM_UR5_H_ */
