#include "GazeboRobot.h"
#include "util.h"

    GazeboRobot::GazeboRobot(std::string name_, std::string world_)
: name(name_), world(world_), worldNode(new gazebo::transport::Node()), factor(1),
    monitorThread(NULL), isMonitoring(false)
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
    // std::cout << target << " ";
}

gazebo::msgs::JointCmd &GazeboRobot::getJointMessage(std::string joint)
{
    if (!jointMessages.count(joint)) {
        jointMessages[joint] = gazebo::msgs::JointCmd();
        jointMessages[joint].set_name(name + std::string("::") + joint);
    }

    return jointMessages[joint];
}

bool GazeboRobot::getPose()
{
    boost::shared_ptr<gazebo::msgs::Response> response = gazebo::transport::request(
            world, "entity_info", name);
    gazebo::msgs::Model modelMsg;

    if (response->has_serialized_data() &&
            !response->serialized_data().empty() &&
            modelMsg.ParseFromString(response->serialized_data()))
    {   
        auto pose = modelMsg.mutable_pose()->mutable_position();
        points.push_back(std::pair<float, Metabot::Point3>(t, Metabot::Point3(pose->x(), pose->y(), pose->z())));
        return true;
    } else {
        return false;
    }
}

void GazeboRobot::waitForAvailable()
{
    while (!getPose()) {
        usleep(50000);
    }
}

void GazeboRobot::tick(float elapsed)
{
    t += factor*elapsed;
}

void GazeboRobot::monitor()
{
    t = 0.0;
    isMonitoring = true;
    points.clear();
    monitorThread = new std::thread([this]() {
        while (this->isMonitoring) {
            this->getPose();
        }
    });
}

void GazeboRobot::stopMonitoring()
{
    if (monitorThread) {
        isMonitoring = false;
        monitorThread->join();
        monitorThread = NULL;
    }

    for (auto entry : points) {
        auto t = entry.first;
        auto point = entry.second;
        std::cout << t << " " << point.x << " " << point.y << " " << point.z << std::endl;
    }
}

void GazeboRobot::unload()
{
    gazebo::transport::PublisherPtr pub;
    while (getPose()) {
        gazebo::msgs::Request *msg_delete = gazebo::msgs::CreateRequest("entity_delete", "metabot");
        pub = worldNode->Advertise<gazebo::msgs::Request>("~/request");
        pub->WaitForConnection();
        pub->Publish(*msg_delete, true);
        delete msg_delete;
        usleep(50000);
    }
}

void GazeboRobot::load(Metabot::Robot &robot)
{
    unload();
    robot.writeSDF(homeDir(".gazebo/models/metabot/"));

    gazebo::transport::PublisherPtr pub;
    boost::shared_ptr<sdf::SDF> sdf(new sdf::SDF());
    sdf::init(sdf);
    sdf::readFile(homeDir(".gazebo/models/metabot/robot.sdf"), sdf);
    gazebo::math::Pose pose;
    pub = worldNode->Advertise<gazebo::msgs::Factory>("~/factory");
    sdf::ElementPtr modelElem = sdf->Root()->GetElement("model");
    modelElem->GetAttribute("name")->SetFromString(name);
    pub->WaitForConnection();
    gazebo::msgs::Factory msg;
    msg.set_sdf(sdf->ToString());
    pose += gazebo::math::Pose(0, 0, 0.1, 0, 0, 0);
    gazebo::msgs::Set(msg.mutable_pose(), pose.Ign());
    pub->Publish(msg, true);
    
    waitForAvailable();
    monitor();
}
