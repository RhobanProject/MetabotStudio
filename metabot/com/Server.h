#pragma once
#include "Robot.h"

namespace Metabot
{
    class Server
    {
        public:
            Server();
            virtual ~Server();
            void loadRobot(Robot *robot);
            void updateRobot(Robot *robot);
            void updateMarker(double x, double y);
            void disableMarker();

        protected:
            void *context;
            void *pub;

            void invoke(std::string command, Json::Value arguments=Json::Value(Json::nullValue));
    };
}
