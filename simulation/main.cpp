#include <getopt.h>
#include <sstream>
#include <deque>
#include <map>
#include <unistd.h>
#include <Robot.h>
#include <Backend.h>
#include "Controller.h"
#include "GazeboRobot.h"
#include "util.h"

#include <iostream>

#ifndef DEG2RAD
#define DEG2RAD(x) (((x)/180.0)*M_PI)
#endif
#ifndef RAD2DEG
#define RAD2DEG(x) (((x)/M_PI)*180.0)
#endif

void cb(ConstWorldStatisticsPtr &_msg)
{
    // Dump the message contents to stdout.
    std::cout << _msg->DebugString();
}
/*
// Create our node for communication
gazebo::transport::NodePtr node(new gazebo::transport::Node());
node->Init();

// Listen to Gazebo world_stats topic
gazebo::transport::SubscriberPtr sub = node->Subscribe("~/world_stats", cb);

// Busy wait loop...replace with your own code as needed.
while (true)
gazebo::common::Time::MSleep(10);

// Make sure to shut everything down.
gazebo::client::shutdown();
*/

/*
   std::string worldName = "default";
   std::string modelName = "metabot";

   while (1) {
   auto response = gazebo::transport::request(
   worldName, "entity_info", modelName);
   gazebo::msgs::Model modelMsg;

   if (response->has_serialized_data() &&
   !response->serialized_data().empty() &&
   modelMsg.ParseFromString(response->serialized_data()))
   {    
   auto pose = gazebo::msgs::ConvertIgn(modelMsg.pose());
   std::cout << gazebo::msgs::Convert(modelMsg.pose()).pos.x << std::endl;
   } 
   }
   */

#define JOINTS 12
std::string joints[JOINTS] = {
    "joint_2",
    "joint_3",
    "joint_4",
    "joint_5",
    "joint_6",
    "joint_7",
    "joint_8",
    "joint_9",
    "joint_10",
    "joint_11",
    "joint_12",
    "joint_13",
};

static void usage()
{
    std::cout << "Metabot simulation (w/ Gazebo)" << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "  -r [file.robot]: The robot file to simulate" << std::endl;
    exit(1);
}

int main(int argc, char *argv[])
{
    int index;
    std::string robotFile = "";

    while ((index = getopt(argc, argv, "r:")) != -1) {
        switch (index) {
            case 'r':
                robotFile = std::string(optarg);
                break;
            case 'h':
                usage();
                break;
        }
    }

    if (robotFile == "") {
        usage();
    }

    try { 
        gazebo::client::setup(argc, argv);
        
        gazebo::transport::NodePtr node(new gazebo::transport::Node());
        gazebo::transport::PublisherPtr pub;
        node->Init("default");

        // Loading the robot
        GazeboRobot metabot("metabot");
        Metabot::Robot robot;
        robot.loadFromFile(robotFile);
        robot.compile();
        metabot.load(robot);

        // Initializing the controller
        Controller controller;
        controller.dx = 70;
        controller.freq = 2.0;

        float t = 0.0;
        while (t < 60) {
            metabot.tick(0.02);
            t += 0.02*controller.freq*metabot.factor;
            auto angles = controller.compute(t);
            for (int k=0; k<4; k++) {
                metabot.setJoint(joints[k*3], -DEG2RAD(angles.l1[k]));
                metabot.setJoint(joints[k*3+1], DEG2RAD(angles.l2[k]));
                metabot.setJoint(joints[k*3+2], -DEG2RAD(angles.l3[k]));
            }
            // std::cout << std::endl;
            usleep(20000);
        }
        metabot.stopMonitoring();

        gazebo::client::shutdown();
    } catch (std::string err) {
        std::cerr << "Error: " << err << std::endl;
    }
}
