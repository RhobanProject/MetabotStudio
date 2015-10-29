#include <sstream>
#include <deque>
#include <map>
#include <unistd.h>
#include <Robot.h>
#include <Backend.h>
#include "Controller.h"
#include "GazeboRobot.h"

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
    "double_u_2_joint",
    "side_to_side_3_joint",
    "arm_leg_4_joint",
    "double_u_5_joint",
    "side_to_side_6_joint",
    "arm_leg_7_joint",
    "double_u_8_joint",
    "side_to_side_9_joint",
    "arm_leg_10_joint",
    "double_u_11_joint",
    "side_to_side_12_joint",
    "arm_leg_13_joint",
};

int main(int _argc, char **_argv)
{
    try {
        std::cout << "Loading the robot..." << std::endl;
        Metabot::Backend backend("xl-320");
        backend.load();
        Metabot::Robot robot(&backend);
        robot.loadFromFile("/home/gregwar/Metabot/robots/metabot.robot");
        robot.compile();
        
        std::cout << "Connecting to the simulator..." << std::endl;
        gazebo::client::setup(_argc, _argv);
        
        gazebo::transport::NodePtr node(new gazebo::transport::Node());
        gazebo::transport::PublisherPtr pub;
        node->Init("default");

        std::cout << "Removing model..." << std::endl;
        gazebo::msgs::Request *msg_delete = gazebo::msgs::CreateRequest("entity_delete", "metabot");
        pub = node->Advertise<gazebo::msgs::Request>("~/request");
        std::cout << "Wait..." << std::endl;
        pub->WaitForConnection();
        std::cout << "Publish..." << std::endl;
        pub->Publish(*msg_delete, true);
        delete msg_delete;

        std::cout << "Exporting it to SDF..." << std::endl;
        robot.writeSDF("/home/gregwar/.gazebo/models/metabot/");

        std::cout << "Loading the model..." << std::endl;
        boost::shared_ptr<sdf::SDF> sdf(new sdf::SDF());
        sdf::init(sdf);
        sdf::readFile("/home/gregwar/.gazebo/models/metabot/robot.sdf", sdf);
        gazebo::math::Pose pose;
        pub = node->Advertise<gazebo::msgs::Factory>("~/factory");
        sdf::ElementPtr modelElem = sdf->Root()->GetElement("model");
        modelElem->GetAttribute("name")->SetFromString("metabot");
        pub->WaitForConnection();
        gazebo::msgs::Factory msg;
        msg.set_sdf(sdf->ToString());
        pose += gazebo::math::Pose(0, 0, 0.05, 0, 0, 0);
        gazebo::msgs::Set(msg.mutable_pose(), pose.Ign());
        pub->Publish(msg, true);

        GazeboRobot metabot("metabot");
        Controller controller;
        controller.dx = 30;

        float t = 0.0;
        while (true) {
            t += 0.02*controller.freq*metabot.factor;
            while (t > 1.0) t -= 1.0;
            auto angles = controller.compute(t);
            for (int k=0; k<4; k++) {
                metabot.setJoint(joints[k*3], -DEG2RAD(angles.l1[k]));
                metabot.setJoint(joints[k*3+1], -DEG2RAD(angles.l2[k]));
                metabot.setJoint(joints[k*3+2], DEG2RAD(angles.l3[k]));
            }
            std::cout << std::endl;
            usleep(20000);
        }

        gazebo::client::shutdown();
    } catch (std::string err) {
        std::cerr << "Error: " << err << std::endl;
    }
}
