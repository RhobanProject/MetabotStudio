#include "GazeboRobot.h"
#include "verbose.h"
#include "util.h"

GazeboRobot *g_robot;
void on_response(ConstResponsePtr &response)
{
    g_robot->requestResponse(response);
}

    GazeboRobot::GazeboRobot(std::string name_, std::string world_)
: name(name_), world(world_), worldNode(new gazebo::transport::Node()), factor(1),
    monitorThread(NULL), isMonitoring(false)
{
    g_robot = this;

    // Initializing world node
    worldNode->Init(world);

    // Subscribe to world stats
    statsSub = worldNode->Subscribe("~/world_stats", &GazeboRobot::getStats, this);

    // Subscribing to responses
    // responseSub = worldNode->Subscribe("~/response", &GazeboRobot::requestResponse);
    responseSub = worldNode->Subscribe("~/response", &on_response);

    // Requester
    requestPub = worldNode->Advertise<gazebo::msgs::Request>("~/request");
    requestPub->WaitForConnection();
}

void GazeboRobot::requestResponse(ConstResponsePtr &response)
{
    gazebo::msgs::Model modelMsg;
    if (response->has_serialized_data() &&
            !response->serialized_data().empty() &&
            modelMsg.ParseFromString(response->serialized_data()))
    {   
        auto pose = modelMsg.mutable_pose()->mutable_position();
        points.push_back(std::pair<float, Metabot::Point3>(t, Metabot::Point3(pose->x(), pose->y(), pose->z())));
        // std::cout << pose->x() << ", " << pose->y() << ", " << pose->z() << std::endl;
        available = true;
    } else {
        available = false;
    }
}

void GazeboRobot::requestPose()
{
    gazebo::msgs::Request *request = gazebo::msgs::CreateRequest("entity_info", name);
    requestPub->Publish(*request, true);
    delete request;
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
    std::cout << "GETPOSE REQ" << std::endl;
    boost::shared_ptr<gazebo::msgs::Response> response = gazebo::transport::request(
            world, "entity_info", name);
    gazebo::msgs::Model modelMsg;
    std::cout << "/GETPOSE REQ" << std::endl;

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
    available = false;
    while (!available) {
        requestPose();
        usleep(300000);
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
            this->requestPose();
            usleep(300000);
        }
    });
}

double GazeboRobot::stopMonitoring()
{
    if (isVerbose()) std::cout << "* Stopping monitor" << std::endl;
    if (monitorThread) {
        isMonitoring = false;
        monitorThread->join();
        monitorThread = NULL;
    }

    double d = 0.0;
    for (auto entry : points) {
        auto t = entry.first;
        auto point = entry.second;
        // std::cout << t << " " << point.x << " " << point.y << " " << point.z << std::endl;
        d = sqrt(point.x*point.x+point.y*point.y+point.z*point.z);
    }

    return d;
}

void GazeboRobot::unload()
{
    if (isVerbose()) std::cout << "* Unloading the robot" << std::endl;
    gazebo::transport::PublisherPtr pub;
    while (available) {
        gazebo::msgs::Request *msg_delete = gazebo::msgs::CreateRequest("entity_delete", "metabot");
        pub = worldNode->Advertise<gazebo::msgs::Request>("~/request");
        pub->WaitForConnection();
        pub->Publish(*msg_delete, true);
        delete msg_delete;
        requestPose();
        usleep(100000);
    }
}

void GazeboRobot::load(Metabot::Robot &robot)
{
    unload();
    robot.writeSDF(homeDir(".gazebo/models/metabot/"));

    // Reading SDF file
    if (isVerbose()) std::cout << "* Reading SDF file" << std::endl;
    gazebo::transport::PublisherPtr pub;
    boost::shared_ptr<sdf::SDF> sdf(new sdf::SDF());
    sdf::init(sdf);
    sdf::readFile(homeDir(".gazebo/models/metabot/robot.sdf"), sdf);
    gazebo::math::Pose pose;

    // Loading it
    pub = worldNode->Advertise<gazebo::msgs::Factory>("~/factory");
    sdf::ElementPtr modelElem = sdf->Root()->GetElement("model");
    modelElem->GetAttribute("name")->SetFromString(name);
    pub->WaitForConnection();
    gazebo::msgs::Factory msg;
    msg.set_sdf(sdf->ToString());
    pose += gazebo::math::Pose(0, 0, 0.1, 0, 0, 0);
    gazebo::msgs::Set(msg.mutable_pose(), pose.Ign());
    if (isVerbose()) std::cout << "* Publishing to factory" << std::endl;
    pub->Publish(msg, true);

    // Waiting the robot to be available in the simulator
    if (isVerbose()) std::cout << "* Waiting to become available" << std::endl;
    usleep(100000);
    waitForAvailable();

    // Creating the publisher
    if (isVerbose()) std::cout << "* Making joint publisher" << std::endl;
    jointPublisher = worldNode->Advertise<gazebo::msgs::JointCmd>(
            std::string("~/") + name + "/joint_cmd");
    jointPublisher->WaitForConnection();
    
    usleep(100000);
    if (isVerbose()) std::cout << "* Monitoring" << std::endl;
    monitor();
}
