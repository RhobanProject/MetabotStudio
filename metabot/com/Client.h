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
            struct Shape {
                TransformMatrix matrix;
                #define COM_SHAPE_SPHERE 0
                int type;
                std::vector<double> params;
            };

            Client();
    
            void loop();
            void process(std::string command, Json::Value arguments);

            // The robot state
            Robot *robot;

            // Current marker
            bool hasMarker;
            double markerX, markerY;
            
            // Shapes
            std::map<int, Shape> shapes;

            bool debug;

        protected:
            std::thread *thread;
            void *context;
            void *client;
    };
}
