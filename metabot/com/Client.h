#pragma once

#include <thread>
#include <mutex>
#include "Robot.h"
#include <json/json.h>
#include <zmq.h>

namespace Metabot
{
    class Client : public std::mutex
    {
        public:
            Client();
    
            void loop();
            void process(std::string command, Json::Value arguments);

            Robot *robot;
            bool debug;

        protected:
            std::thread *thread;
            void *context;
            void *client;
    };
}
