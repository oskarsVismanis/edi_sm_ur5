#include <sm_ur5/sm_ur5.h>

//--------------------------------------------------------------------
int main(int argc, char **argv)
{
    ros::init(argc, argv, "sm_ur5");
    ros::NodeHandle nh;
//    ros::AsyncSpinner spinner(0);
//    spinner.start();

    smacc::run<sm_ur5::SmUr5>();
}
