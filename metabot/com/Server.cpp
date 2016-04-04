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

    void Server::init()
    {
        disableMarker();
        removeShapes();
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

    void Server::addShape(int id, int type, TransformMatrix matrix, std::vector<double> params)
    {
        Json::Value args(Json::arrayValue);
        args[0] = id;
        args[1] = type;
        args[2] = matrix.toJson();

        Json::Value paramsJson(Json::arrayValue);
        for (int k=0; k<params.size(); k++) {
            paramsJson[k] = params[k];
        }
        args[3] = paramsJson;
        invoke("addShape", args);
    }

    void Server::updateShape(int id, TransformMatrix matrix)
    {
        Json::Value args(Json::arrayValue);
        args[0] = id;
        args[1] = matrix.toJson();

        invoke("updateShape", args);
    }

    void Server::removeShapes()
    {
        invoke("removeShapes");
    }
}
