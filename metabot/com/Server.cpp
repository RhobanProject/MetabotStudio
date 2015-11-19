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

    void Server::loadRobot(Robot *robot)
    {
        invoke("load", robot->toJson());
    }

    void Server::updateRobot(Robot *robot)
    {
        invoke("update", robot->stateToJson());
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
