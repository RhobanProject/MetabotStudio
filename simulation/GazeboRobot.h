#pragma once
#include <gazebo/transport/transport.hh>
#include <gazebo/msgs/msgs.hh>
#include <gazebo/gazebo_client.hh>

class GazeboRobot
{
    public:
        GazeboRobot(std::string name_, std::string world_="default");
        void getStats(ConstWorldStatisticsPtr &_msg);
        void setJoint(std::string joint, float target);

        float factor;

    protected:
        gazebo::transport::NodePtr worldNode;
        gazebo::transport::PublisherPtr jointPublisher;
        gazebo::transport::SubscriberPtr statsSub;
        std::string name, world;
        std::deque<double> simTimes;
        std::deque<double> realTimes;
        std::map<std::string, gazebo::msgs::JointCmd> jointMessages;

        gazebo::msgs::JointCmd &getJointMessage(std::string joint);
};

