#include "GazeboRobot.h"

GazeboRobot::GazeboRobot(std::string name_, std::string world_)
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

void GazeboRobot::getStats(ConstWorldStatisticsPtr &_msg)
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

void GazeboRobot::setJoint(std::string joint, float target)
{
    auto &msg = getJointMessage(joint);
    msg.mutable_position()->set_target(target);
    jointPublisher->Publish(msg, true);
    std::cout << target << " ";
}

gazebo::msgs::JointCmd &GazeboRobot::getJointMessage(std::string joint)
{
    if (!jointMessages.count(joint)) {
        jointMessages[joint] = gazebo::msgs::JointCmd();
        jointMessages[joint].set_name(name + std::string("::") + joint);
    }

    return jointMessages[joint];
}

