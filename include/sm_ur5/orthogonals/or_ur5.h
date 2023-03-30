#pragma once

//#include <sm_ur5/sm_ur5.h>
//#include <smacc/smacc.h>
//#include <smacc/smacc_orthogonal.h>
#include <sm_ur5/clients/pick_and_place_client/cl_ur5_pick_and_place.h>

using namespace sm_ur5::cl_ur5_pick_and_place;

namespace sm_ur5
{
//namespace cl_ur5_pick_and_place
//{
class OrUr5 : public smacc::Orthogonal<OrUr5>
{
public:
    virtual void onInitialize() override
    {
        auto pickAndPlaceClient = this->createClient<ClUr5PickAndPlace>("ur5");//("ur5_arm");
        pickAndPlaceClient->initialize();

    }
};
} // namespace sm_ur5
