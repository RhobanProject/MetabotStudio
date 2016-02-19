#include <zmq.h>
#include "Server.h"

namespace Metabot
{
    Server::Server()
    {
        context = zmq_ctx_new();
        pub = zmq_socket(context, ZMQ_PUB);
        zmq_bind(pub, "tcp://*:8374");
    }

    Server::~Server()
    {
        zmq_close(pub);
        zmq_ctx_destroy(context);
    }

    void Server::loadRobot(Robot *robot)
    {
        invoke("load", robot->toJson());
    }

    void Server::updateRobot(Robot *robot)
    {
        invoke("update", robot->stateToJson());
    }
            
    void Server::updateMarker(double x, double y)
    {
        Json::Value marker(Json::arrayValue);
        marker[0] = x;
        marker[1] = y;
        invoke("marker", marker);
    }
            
    void Server::disableMarker()
    {
        Json::Value marker(Json::arrayValue);
        invoke("marker", marker);
    }

    void Server::invoke(std::string command, Json::Value arguments)
    {
        // Generating JSON
        Json::Value request(Json::arrayValue);
        request[0] = command;
        request[1] = arguments;

        Json::FastWriter writer;
        std::string data = writer.write(request);

        // Sending ZMQ message
        zmq_msg_t message;
        zmq_msg_init_size(&message, data.size());
        memcpy(zmq_msg_data(&message), data.c_str(), data.size());
        zmq_msg_send(&message, pub, 0);
        zmq_msg_close(&message);
    }
}
