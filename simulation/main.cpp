#include <sstream>
#include <deque>
#include <map>
#include <unistd.h>
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/gazebo_client.hh>
#include "motion.h"
#include <Robot.h>
#include <Backend.h>

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

class GazeboRobot
{
    public:
        GazeboRobot(std::string name_, std::string world_="default")
            : name(name_), world(world_), worldNode(new gazebo::transport::Node()), factor(1)
            {
                // Initializing world node
                worldNode->Init(world);

                // Creating the publisher
                jointPublisher = worldNode->Advertise<gazebo::msgs::JointCmd>(
                        std::string("~/") + name + "/joint_cmd");
                jointPublisher->WaitForConnection();

                // Subscribe to world stats
                statsSub = worldNode->Subscribe("~/world_stats", &GazeboRobot::getStats, this);
            }

        void getStats(ConstWorldStatisticsPtr &_msg)
        {
            auto simTime = gazebo::msgs::Convert(_msg->sim_time());
            auto realTime = gazebo::msgs::Convert(_msg->real_time());

            simTimes.push_back(simTime.Double());
            if (simTimes.size() > 30) simTimes.pop_front();
            realTimes.push_back(realTime.Double());
            if (realTimes.size() > 30) realTimes.pop_front();

            double simTotal=0, realTotal=0;
            for (int k=0; k<simTimes.size(); k++) {
                simTotal += simTimes[k]-simTimes.front();
                realTotal += realTimes[k]-realTimes.front();
            }

            if (realTotal > 0) {
                factor = simTotal/(float)realTotal;
                // std::cout << factor << std::endl;
            }
        }

        void setJoint(std::string joint, float target)
        {
            auto &msg = getJointMessage(joint);
            msg.mutable_position()->set_target(target);
            jointPublisher->Publish(msg, true);
            std::cout << target << " ";
        }

        float factor;

    protected:
        gazebo::transport::NodePtr worldNode;
        gazebo::transport::PublisherPtr jointPublisher;
        gazebo::transport::SubscriberPtr statsSub;
        std::string name, world;
        std::deque<double> simTimes;
        std::deque<double> realTimes;
        std::map<std::string, gazebo::msgs::JointCmd> jointMessages;

        gazebo::msgs::JointCmd &getJointMessage(std::string joint)
        {
            if (!jointMessages.count(joint)) {
                jointMessages[joint] = gazebo::msgs::JointCmd();
                jointMessages[joint].set_name(name + std::string("::") + joint);
            }

            return jointMessages[joint];
        }
};

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
        robot.writeSDF("/tmp/metabot");

        std::cout << "Loading the model..." << std::endl;
        boost::shared_ptr<sdf::SDF> sdf(new sdf::SDF());
        sdf::init(sdf);
        sdf::readFile("/tmp/metabot/robot.sdf", sdf);
        gazebo::math::Pose pose;
        pub = node->Advertise<gazebo::msgs::Factory>("~/factory");
        sdf::ElementPtr modelElem = sdf->Root()->GetElement("model");
        modelElem->GetAttribute("name")->SetFromString("metabot");
        pub->WaitForConnection();
        gazebo::msgs::Factory msg;
        msg.set_sdf(sdf->ToString());
        gazebo::msgs::Set(msg.mutable_pose(), pose.Ign());
        pub->Publish(msg, true);

        GazeboRobot metabot("metabot");
        motion_init();

        float t = 0.0;
        while (true) {
            t += 0.02*motion_get_f()*metabot.factor;
            while (t > 1.0) t -= 1.0;
            motion_tick(t);
            for (int k=0; k<4; k++) {
                metabot.setJoint(joints[k*3], DEG2RAD(l1[k]));
                metabot.setJoint(joints[k*3+1], -DEG2RAD(l2[k]));
                metabot.setJoint(joints[k*3+2], DEG2RAD(l3[k]));
            }
            std::cout << std::endl;
            usleep(20000);
        }

        gazebo::client::shutdown();
    } catch (std::string err) {
        std::cerr << "Error: " << err << std::endl;
    }
}
