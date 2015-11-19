#include <unistd.h>
#include <zmq.h>
#include "Client.h"
            
namespace Metabot
{
    Client::Client()
        : robot(NULL), debug(false)
    {
        context = zmq_ctx_new();
        client = zmq_socket(context, ZMQ_SUB);
        zmq_connect(client, "tcp://127.0.0.1:8374");
        zmq_setsockopt(client, ZMQ_SUBSCRIBE, "", 0);

        thread = new std::thread([this]() {
            this->loop();
        });
    }

    void Client::loop()
    {
        int code;
        while (true) {
            zmq_msg_t message;
            zmq_msg_init(&message);
            if ((code=zmq_msg_recv(&message, client, 0)) > 0) {
                std::string data((char*)zmq_msg_data(&message), zmq_msg_size(&message));
                if (debug) {
                    std::cout << data << std::endl;
                }
                Json::Value json;
                Json::Reader reader;
                if (reader.parse(data, json)) {
                    if (json.isArray() && json.size()==2 && json[0].isString()) {
                        process(json[0].asString(), json[1]);
                    }
                }
            } else {
                printf("ZMQ Error: %s\n", zmq_strerror(zmq_errno()));
                usleep(1000000);
            }
            zmq_msg_close(&message);
        }
    }

    void Client::process(std::string command, Json::Value arguments)
    {
        if (command == "load") {
            auto oldRobot = robot;
            auto newRobot = new Robot;
            lock();
            try {
                newRobot->fromJson(arguments);
                robot = newRobot;
                robot->toBullet();

                if (oldRobot != NULL) {
                    delete oldRobot;
                }
            } catch (...) {
            }
            unlock();
        }

        if (command == "update") {
            if (robot != NULL) {
                robot->stateFromJson(arguments);
            }
        }
    }
}
