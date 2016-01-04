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

        protected:
            void *context;
            void *pub;

            void invoke(std::string command, Json::Value arguments=Json::Value(Json::nullValue));
    };
}
